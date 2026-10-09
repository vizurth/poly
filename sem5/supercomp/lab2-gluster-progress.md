# Лабораторная №2: журнал выполнения GlusterFS

Этот файл — журнал именно нашего стенда: что уже сделано, какие команды запускались и что происходит внутри. Общий план лабораторной находится отдельно в [lab2-gluster-debian-guide.md](lab2-gluster-debian-guide.md).

## Текущая схема

| ВМ | Роль | Адрес внутренней сети |
|---|---|---|
| `manage` | Клиент GlusterFS | `10.10.10.10` |
| `cn01` | Сервер хранения и узел управления пулом | `10.10.10.11` |
| `cn02` | Сервер хранения | `10.10.10.12` |

У `cn01` и `cn02` есть по отдельному виртуальному диску на 10 ГБ. Хотя внутри обеих машин он называется `/dev/sdb`, это разные виртуальные диски: каждый подключён только к своей ВМ. Системный диск — `/dev/sda`; его не размечаем и не форматируем.

## Что уже сделано

### 1. Подняли сеть между ВМ

У ВМ есть NAT-сеть для интернета и внутренний интерфейс для общения друг с другом. На внутреннем интерфейсе заданы адреса `10.10.10.10`–`10.10.10.12`; пинги между `manage`, `cn01` и `cn02` прошли.

В `/etc/hosts` добавлены имена `manage`, `cn01`, `cn02`. Поэтому в командах Gluster можно указывать имена узлов, а не только IP-адреса.

### 2. Установили GlusterFS

- На `manage` установлен только пакет `glusterfs-client`.
- На `cn01` и `cn02` установлены `glusterfs-server`; служба `glusterd` запущена.

Сервер хранит кирпичи и управляет GlusterFS. Клиент подключает готовый том как файловую систему.

### 3. Объединили серверы в пул

На `cn01` выполнили:

```bash
sudo gluster peer probe cn02
sudo gluster peer status
```

`peer probe` попросил `cn01` установить связь с `cn02`. В ответе `peer status` было `Peer in Cluster (Connected)`. Это означает, что серверы знают друг о друге и готовы создавать общие тома. Само по себе добавление в пул ещё не создаёт общее файловое пространство.

### 4. Подготовили диски и LVM на серверах хранения

На каждой из `cn01` и `cn02` проверили диск командой `lsblk`. Новый пустой диск на 10 ГБ показался как `/dev/sdb`. Затем на обеих ВМ выполнили:

```bash
sudo apt install -y lvm2 thin-provisioning-tools xfsprogs
sudo pvcreate /dev/sdb
sudo vgcreate vg /dev/sdb
sudo lvcreate --type thin-pool -L 8G -n pool vg
sudo lvcreate -V 7G -T vg/pool -n brick
sudo mkfs.xfs /dev/vg/brick
sudo mkdir -p /snapbrick
sudo mount /dev/vg/brick /snapbrick
sudo mkdir -p /snapbrick/brick
```

Что означают ступени LVM:

| Название | Что это | Зачем нужно |
|---|---|---|
| `/dev/sdb` | Второй виртуальный диск конкретной ВМ | Даёт серверу отдельное место под данные Gluster. |
| PV | Физический том LVM, созданный `pvcreate` | Помечает диск как устройство, которым может управлять LVM. |
| VG `vg` | Группа томов, созданная `vgcreate` | Объединяет доступное место диска в пул LVM. Имя `vg` одинаково на обеих ВМ, но сами группы независимы. |
| Thin-pool `pool` | Тонкий пул LVM, созданный `lvcreate --type thin-pool` | Выделяет физическое место для thin-томов и позволяет экономно расходовать пространство. |
| Thin LV `brick` | Логический том на 7 ГБ, созданный `lvcreate -T` | На нём лежит файловая система XFS; Gluster получает из неё каталог-кирпич. Размер 7 ГБ — виртуальная ёмкость, данные занимают место в thin-pool по мере записи. |
| `/snapbrick` | Точка монтирования XFS | Делает содержимое LV доступным в дереве каталогов Linux. |
| `/snapbrick/brick` | Каталог-кирпич GlusterFS | Именно этот каталог Gluster добавляет в свой том. |

Путь целиком выглядит так:

```text
отдельный виртуальный диск
  └─ PV → VG vg → thin-pool pool → thin LV brick
       └─ XFS смонтирована в /snapbrick
            └─ каталог /snapbrick/brick передан GlusterFS
```

Диски `cn01` и `cn02` напрямую друг к другу не подключаются. Каждый сервер локально хранит свой кирпич, а GlusterFS передаёт данные между ними по внутренней сети.

На обеих машинах `findmnt /snapbrick` показал источник `/dev/mapper/vg-brick` с файловой системой `xfs`. Повторный `mount` сообщил `already mounted` — это нормально: файловая система уже была смонтирована.

### 5. Создали первый GlusterFS-том: replica

На `cn01` выполнили:

```bash
sudo gluster volume create replica replica 2 cn01:/snapbrick/brick cn02:/snapbrick/brick
sudo gluster volume start replica
sudo gluster volume info replica
```

Здесь первое `replica` — имя тома. Второе `replica 2` задаёт тип и число копий. Пути после этого — два кирпича, по одному на каждом сервере.

Gluster сообщил `volume create: replica: success` и `volume start: replica: success`. В `volume info` показаны:

- `Type: Replicate`;
- `Status: Started`;
- `Number of Bricks: 1 x 2 = 2`;
- `cn01:/snapbrick/brick` и `cn02:/snapbrick/brick`.

При создании Gluster предупредил, что двухкопийная реплика может столкнуться с split-brain, если узлы разойдутся во мнениях о правильной версии файла. Для учебного показа продолжаем; arbiter проверим отдельным этапом.

## 6. Проверили запись с клиента

На `manage` подключаем том и пишем в него файл:

```bash
sudo mkdir -p /mnt/replica
sudo mount -t glusterfs cn01:/replica /mnt/replica
echo "hello gluster" | sudo tee /mnt/replica/test.txt
cat /mnt/replica/test.txt
```

Запись через `/mnt/replica` идёт на том GlusterFS, а не напрямую на отдельный сервер. Для проверки репликации затем на **обеих** машинах `cn01` и `cn02` читаем локальные копии:

```bash
sudo cat /snapbrick/brick/test.txt
```

Проверка прошла: клиент записал `hello gluster`, а `sudo cat /snapbrick/brick/test.txt` на обоих серверах показал тот же текст. Значит, GlusterFS принял запись на смонтированный том и создал копию в кирпиче каждого узла.

## 7. Создали distributed-том с одним кирпичом

Чтобы не смешивать тестовые данные с replica-томом, создали отдельный каталог на системном диске обеих машин:

```bash
sudo mkdir -p /data/gluster/dist/brick
```

На `cn01` создали и запустили том:

```bash
sudo gluster volume create dist cn01:/data/gluster/dist/brick force
sudo gluster volume start dist
sudo gluster volume info dist
```

`volume info` подтвердил `Type: Distribute`, `Status: Started` и один кирпич `cn01:/data/gluster/dist/brick`. Команды `volume create` и `volume start` на `cn02` вернули `already exists` и `already started`: это ожидаемо, так как том создаётся один раз с управляющего узла `cn01`, а конфигурация доступна всему пулу.

На `manage` подключили `dist` и создали 10 тестовых файлов в `/mnt/dist`. Пока у тома был один кирпич, все файлы хранились на `cn01`.

### 8. Добавили второй кирпич и запустили rebalance

На `cn01` добавили второй кирпич, который подготовили каталогом `/data/gluster/dist/brick` на `cn02`:

```bash
sudo gluster volume add-brick dist cn02:/data/gluster/dist/brick force
sudo gluster volume info dist
sudo gluster volume rebalance dist start
sudo gluster volume rebalance dist status
```

`volume info` показал два кирпича, а `add-brick` завершился успешно. Первый статус rebalance был `in progress`. При повторной проверке Gluster сообщил `success`, статус `completed`, 4 перенесённых файла (28 байт), 10 просканированных объектов и 0 ошибок. Затем проверили количество файлов: на клиенте `/mnt/dist` — 10, на кирпиче `cn01` — 6, на кирпиче `cn02` — 4. Все файлы на месте, ошибок rebalance не было.

Удаление кирпича `cn02` завершено: `remove-brick ... start` перенёс все 4 файла (28 байт), `status` показал `completed` и 0 ошибок, а `commit` завершился `success`. `volume info dist` теперь показывает один кирпич — `cn01:/data/gluster/dist/brick`. Во время миграции тестовые файлы не изменялись, поэтому предупреждение Gluster о записях во время миграции к ним не относится.

Проверили результат: клиентский mount показывает 10 файлов, на кирпиче `cn01` тоже 10, а команда поиска файлов в удалённом каталоге на `cn02` ничего не вывела. Данные остались доступны после удаления кирпича.

## Следующий шаг: arbiter-том

Для демонстрации arbiter подготовим три отдельных каталога. В нашей конфигурации только два storage-сервера, поэтому второй data brick и arbiter brick будут на `cn02` в разных каталогах. Это соответствует примеру из приложенного отчёта, но не обеспечивает независимость этих двух кирпичей при отказе `cn02`.

На `cn01` создали и запустили том:

```bash
sudo gluster volume create arbiter replica 3 arbiter 1 \
  cn01:/data/gluster/arbiter-data/brick \
  cn02:/data/gluster/arbiter-data/brick \
  cn02:/data/gluster/arbiter-metadata/brick force
sudo gluster volume start arbiter
sudo gluster volume info arbiter
```

Команды завершились `success`. Информация показала `Type: Replicate`, `Status: Started`, `Number of Bricks: 1 x (2 + 1) = 3`; третий кирпич `cn02:/data/gluster/arbiter-metadata/brick` помечен `(arbiter)`. Два первых кирпича содержат данные, третий хранит метаданные и голос для кворума, а не копию пользовательского файла.

Следующий шаг — записать файл через клиент и сравнить размеры файла на двух data bricks и arbiter brick.

Проверка прошла: на обоих data bricks файл `test.txt` содержит `arbiter test` и имеет размер 13 байт. На arbiter brick файл существует, но имеет размер 0 байт. Это показывает разницу между хранением данных и хранением метаданных для кворума.

## 9. Включили квоту на каталоге replica

На `manage` создали `/mnt/replica/quotaTest`; на `cn01` включили квоты для тома `replica` и задали каталогу `/quotaTest` лимит 10 МБ:

```bash
sudo gluster volume quota replica enable
sudo gluster volume quota replica limit-usage /quotaTest 10MB
```

Затем на `manage` команда `dd` записала файл размером 20 МиБ. При следующем запросе `quota list` показал `Used 20.0MB`, `Available 0Bytes`, `Soft-limit exceeded: Yes` и `Hard-limit exceeded: Yes`. То есть Gluster учёл превышение, но эта запись прошла целиком; не записываем в отчёт, что `dd` остановился на лимите.

## 10. Создали снапшот replica

На клиенте `manage` записали `/mnt/replica/before-snap.txt` с текстом `before snapshot`. На `cn01` выполнили:

```bash
sudo gluster snapshot create snap1 replica no-timestamp
sudo gluster snapshot list replica
sudo gluster snapshot info snap1
sudo gluster snapshot status snap1
```

Создание прошло успешно, `snap1` появился в списке. Статус показал `Deactivated Snapshot` и `Brick Running: No`. Это нормальное состояние созданного, но ещё не активированного снимка. Чтобы подключить его для просмотра, сначала активируем.

Снимок активировали командой `sudo gluster snapshot activate snap1`. После этого на `manage` создали `after-snap.txt` в живом томе и смонтировали снимок в `/mnt/snapview`. В каталоге снимка виден `before-snap.txt`, а `after-snap.txt` отсутствует — снимок содержит состояние на момент создания.

Следующий шаг — отмонтировать том и его просмотр, остановить `replica`, восстановить `snap1`, снова запустить том и проверить, что `after-snap.txt` исчез из живого тома.

Восстановление выполнено: `gluster volume stop replica`, `gluster snapshot restore snap1` и повторный `gluster volume start replica` завершились успешно. После повторного монтирования на `manage` в `/mnt/replica` остался `before-snap.txt`, а `after-snap.txt` отсутствует. Так проверили создание, просмотр и восстановление снапшота.

После восстановления `gluster snapshot list` показал `No snapshots present`; использованный снимок больше не числится. `gluster volume list` показал три оставшихся тома: `arbiter`, `dist`, `replica`. Заключительный этап — корректно отмонтировать их на клиенте, остановить и удалить их конфигурацию, затем отсоединить `cn02` из пула.

Тома `replica`, `dist` и `arbiter` остановлены и удалены на `cn01`. Команда `gluster volume list` подтвердила `No volumes present in cluster`. Затем `sudo gluster peer detach cn02` завершился `success`; `peer status` показал `Number of Peers: 0`. Основные операции лабораторной выполнены: установка, пул, replica/distributed/arbiter, изменение кирпичей и rebalance, квота, снапшот и удаление томов/пира.

## Что ещё сделать

- Завершить удаление кирпича из distributed-тома и проверить перенос файлов.
- Создать arbiter-том и посмотреть, что хранит arbiter-кирпич.
- Настроить квоту на каталог и проверить ограничение.
- Создать снапшот, посмотреть его и восстановить состояние тома.
- Сохранить выводы и скриншоты для итогового отчёта.
