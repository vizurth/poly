# GlusterFS: основные команды и примеры

Краткая шпаргалка к стенду из трёх Debian-ВМ. Полный ход выполнения и объяснения лежат в [журнале лабораторной](lab2-gluster-progress.md), а подробный план подготовки ВМ — в [гайде](lab2-gluster-debian-guide.md).

## Текущий статус стенда

- `manage` — клиент, адрес внутренней сети `10.10.10.10`.
- `cn01` — сервер хранения, `10.10.10.11`.
- `cn02` — сервер хранения, `10.10.10.12`.
- На `cn01` и `cn02` второй диск — `/dev/sdb`, подготовлен через LVM thin и смонтирован в `/snapbrick`.
- Сейчас `cn02` отсоединён от пула, а тома `replica`, `dist`, `arbiter` удалены из конфигурации.

Перед командами работы с томами снова собери пул **на cn01**:

```bash
sudo gluster peer probe cn02
sudo gluster peer status
```

Продолжай, когда `cn02` показывает `Peer in Cluster (Connected)`. После удаления томов их brick-каталоги могут содержать старые данные и служебную папку `.glusterfs`; перед повторным использованием проверь их и выбери чистые пустые каталоги.

## Установка пакетов

На `manage` нужен только клиент:

```bash
sudo apt update
sudo apt install -y glusterfs-client
```

На `cn01` и `cn02` нужны сервер и служба управления GlusterFS:

```bash
sudo apt update
sudo apt install -y glusterfs-server
sudo systemctl enable --now glusterd
sudo systemctl status glusterd --no-pager
```

## Проверка узлов и пула

Выполнять на `cn01`:

```bash
sudo gluster peer status
sudo gluster pool list
sudo gluster peer probe cn02
sudo gluster peer detach cn02
```

| Команда | Что делает |
|---|---|
| `peer status` | Показывает подключённых Gluster-пиров и их состояние. |
| `pool list` | Показывает узлы trusted storage pool. |
| `peer probe cn02` | Добавляет `cn02` в пул. |
| `peer detach cn02` | Отсоединяет `cn02`; перед этим останови и удали тома, использующие его кирпичи. |

## Создать и проверить том

Общий порядок для каждого типа: создать пустые каталоги-кирпичи на серверах, выполнить `volume create` на `cn01`, запустить том и посмотреть его конфигурацию.

```bash
sudo gluster volume create ИМЯ_ТОМА ...КИРПИЧИ...
sudo gluster volume start ИМЯ_ТОМА
sudo gluster volume info ИМЯ_ТОМА
sudo gluster volume list
```

### Replica — две копии

Пример команды, которую выполнили в стенде:

```bash
sudo gluster volume create replica replica 2 \
  cn01:/snapbrick/brick \
  cn02:/snapbrick/brick
sudo gluster volume start replica
sudo gluster volume info replica
```

Первое `replica` — имя тома; `replica 2` задаёт тип и число копий. Данные, записанные в том, хранятся в обоих кирпичах. Реплика из двух копий может столкнуться со split-brain.

### Distributed — распределение файлов

Пример одно-кирпичного тома перед добавлением второго кирпича:

```bash
sudo gluster volume create dist cn01:/data/gluster/dist/brick force
sudo gluster volume start dist
sudo gluster volume add-brick dist cn02:/data/gluster/dist/brick force
sudo gluster volume info dist
```

После добавления кирпича перенеси существующие файлы rebalance:

```bash
sudo gluster volume rebalance dist start
sudo gluster volume rebalance dist status
```

Удаление кирпича делается в два этапа: сначала миграция, затем подтверждение после `completed`:

```bash
sudo gluster volume remove-brick dist cn02:/data/gluster/dist/brick start
sudo gluster volume remove-brick dist cn02:/data/gluster/dist/brick status
sudo gluster volume remove-brick dist cn02:/data/gluster/dist/brick commit
sudo gluster volume info dist
```

`start` переносит файлы с удаляемого кирпича; `commit` удаляет его из конфигурации. Не записывай и не изменяй данные во время миграции. После `commit` проверь оставшийся и удалённый каталоги: Gluster может оставить данные в удалённом brick-пути.

### Arbiter — два data brick и один arbiter brick

Команда из стенда с двумя storage-серверами:

```bash
sudo gluster volume create arbiter replica 3 arbiter 1 \
  cn01:/data/gluster/arbiter-data/brick \
  cn02:/data/gluster/arbiter-data/brick \
  cn02:/data/gluster/arbiter-metadata/brick force
sudo gluster volume start arbiter
sudo gluster volume info arbiter
```

В выводе третий кирпич помечен `(arbiter)`. Data bricks хранят содержимое файлов; arbiter brick хранит метаданные и участвует в кворуме. В нашем учебном стенде два кирпича размещены на `cn02` в разных каталогах, поэтому это не защищает от потери самой ВМ `cn02`.

## Подключить том на клиенте

Команды выполняются на `manage`:

```bash
sudo mkdir -p /mnt/replica
sudo mount -t glusterfs cn01:/replica /mnt/replica
echo "hello gluster" | sudo tee /mnt/replica/test.txt
cat /mnt/replica/test.txt
```

Синтаксис подключения: `mount -t glusterfs СЕРВЕР:/ТОМ ТОЧКА_МОНТИРОВАНИЯ`. Писать данные нужно через клиентский mount, а не напрямую в brick-каталог.

Для проверки replica на обоих storage-серверах прочитай локальные кирпичи:

```bash
sudo cat /snapbrick/brick/test.txt
```

## Квота каталога

На `cn01` включить квоты, назначить лимит каталогу и посмотреть состояние:

```bash
sudo gluster volume quota replica enable
sudo gluster volume quota replica limit-usage /quotaTest 10MB
sudo gluster volume quota replica list /quotaTest
```

На `manage` создать тестовый файл больше лимита:

```bash
sudo dd if=/dev/zero of=/mnt/replica/quotaTest/bigfile.bin \
  bs=1M count=20 status=progress
```

В этом стенде `dd` записал 20 МиБ полностью. После записи `quota list` показал `Used 20.0MB`, `Available 0Bytes`, `Hard-limit exceeded: Yes`. Поэтому фактический вывод: квота учла превышение, но эта конкретная запись не была остановлена.

## Снапшоты

Для снапшотов brick должен находиться на LVM thin. На каждом storage-сервере в стенде:

> Эти команды описывают первоначальную подготовку нового пустого диска `/dev/sdb`. LVM уже настроен на текущих ВМ; не запускай `pvcreate` или `mkfs` повторно на диске с данными.

```bash
sudo pvcreate /dev/sdb
sudo vgcreate vg /dev/sdb
sudo lvcreate --type thin-pool -L 8G -n pool vg
sudo lvcreate -V 7G -T vg/pool -n brick
sudo mkfs.xfs /dev/vg/brick
sudo mkdir -p /snapbrick
sudo mount /dev/vg/brick /snapbrick
sudo mkdir -p /snapbrick/brick
```

Перед `pvcreate` обязательно проверь `lsblk`: `/dev/sdb` должен быть дополнительным пустым диском. Это стирает метаданные на выбранном диске.

Цикл снапшота тома `replica`:

```bash
# На manage: файл до снимка
echo "before snapshot" | sudo tee /mnt/replica/before-snap.txt

# На cn01: создать и изучить снимок
sudo gluster snapshot create snap1 replica no-timestamp
sudo gluster snapshot list replica
sudo gluster snapshot info snap1
sudo gluster snapshot status snap1
sudo gluster snapshot activate snap1

# На manage: открыть снимок отдельно, только для просмотра
sudo mkdir -p /mnt/snapview
sudo mount -t glusterfs cn01:/snaps/snap1/replica /mnt/snapview
ls -l /mnt/snapview
```

Файл, записанный после снимка, в `/mnt/snapview` отсутствует. Для восстановления отмонтируй клиентские точки, останови том, восстанови снимок и снова запусти том:

```bash
# На manage
sudo umount /mnt/snapview
sudo umount /mnt/replica

# На cn01; команды откатывают весь том к состоянию снимка
sudo gluster volume stop replica
sudo gluster snapshot restore snap1
sudo gluster volume start replica

# На manage: подключить живой том заново
sudo mount -t glusterfs cn01:/replica /mnt/replica
ls -l /mnt/replica
```

Восстановление удаляет изменения, сделанные после снимка. В нашем прогоне `before-snap.txt` остался, а `after-snap.txt` исчез; после restore `snapshot list` показал, что снимка больше нет.

## Остановить и удалить том

Сначала на `manage` отмонтировать его клиентский путь, затем на `cn01`:

```bash
sudo gluster volume stop ИМЯ_ТОМА
sudo gluster volume delete ИМЯ_ТОМА
sudo gluster volume list
```

При остановке и удалении CLI запрашивает подтверждение. Удаление убирает конфигурацию тома; до очистки данных на brick-путях проверь, что нужные файлы сохранены.
