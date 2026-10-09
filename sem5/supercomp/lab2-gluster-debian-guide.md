# Лабораторная работа 2: GlusterFS на Debian 12

Пошаговый гайд для учебного стенда в VirtualBox. Виртуальными машинами можно управлять из Terminal на macOS через SSH; GlusterFS-серверы общаются по отдельной внутренней сети.

## Что в итоге будет

| Виртуальная машина | Назначение | Внутренний IP |
|---|---|---|
| `manage` | клиент GlusterFS и точка монтирования томов | `10.10.10.10` |
| `cn01` | сервер хранения и узел управления пулом | `10.10.10.11` |
| `cn02` | сервер хранения | `10.10.10.12` |

Стенд соответствует минимальному варианту из задания: два сервера хранения и клиент. Arbiter-том будет использовать два кирпича данных и третий кирпич-арбитр. Поскольку хранилищ только два, кирпич данных и arbiter окажутся на `cn02` в разных каталогах. Это годится для демонстрации механизма, но не даёт полноценной отказоустойчивости при потере `cn02`. Если преподаватель требует отдельную машину для каждого из трёх кирпичей arbiter-тома, добавь четвёртую ВМ.

## Перед началом

- Установи Debian 12 на все три ВМ.
- Для каждой ВМ выдели не меньше 1 vCPU и 1 ГБ RAM; если ресурсы Mac позволяют, лучше дать по 2 ГБ.
- Для `cn01` и `cn02` добавь второй виртуальный диск по 8–10 ГБ. Он понадобится только для раздела со снапшотами.
- Создай обычного пользователя Debian с правом `sudo`. Не включай удалённый вход по SSH под root.
- Во время установки Debian подключи виртуальный CD/DVD с ISO и выбери установку без графического окружения, если графика не нужна.

> Команды с пометкой `на Mac` вводятся в Terminal macOS. Команды с пометкой `на manage`, `на cn01` или `на cn02` выполняются на соответствующей Debian-ВМ. В командах `ТВОЙ_ЛОГИН` замени текст на имя своего пользователя Debian. Команды `sudo` выполняй из-под этого пользователя.

## 1. Создай ВМ и настрой виртуальную сеть

Создай три ВМ с точными именами `manage`, `cn01`, `cn02` и установи на них Debian 12.

На каждой ВМ в VirtualBox открой **Settings → Network** и настрой:

1. **Adapter 1**: включён, `Attached to: NAT`. Он даст Debian доступ в интернет и позволит подключаться с Mac через проброс портов.
2. **Adapter 2**: включён, `Attached to: Internal Network`, имя сети у всех машин одинаковое — `labnet`. Эта сеть будет использоваться для трафика GlusterFS.

Не назначай внутренней сети шлюз по умолчанию: интернет для ВМ уже предоставляет NAT-адаптер.

### Настрой SSH-проброс портов

На каждой ВМ открой **Settings → Network → Adapter 1 → Advanced → Port Forwarding**. Добавь правило TCP:

| ВМ | Имя правила | Host IP | Host Port | Guest IP | Guest Port |
|---|---|---:|---:|---:|---:|
| `manage` | `ssh-manage` | `127.0.0.1` | `2221` | `10.0.2.15` | `22` |
| `cn01` | `ssh-cn01` | `127.0.0.1` | `2222` | `10.0.2.15` | `22` |
| `cn02` | `ssh-cn02` | `127.0.0.1` | `2223` | `10.0.2.15` | `22` |

У стандартного NAT VirtualBox адрес гостя обычно `10.0.2.15`. Если после установки Debian у NAT-интерфейса другой адрес, посмотри его внутри ВМ командой `ip -4 addr` и укажи фактический адрес в правиле. Порт `22` — SSH внутри гостевой машины; порты `2221`–`2223` — входные порты на Mac.

## 2. Установи SSH и подключайся с Mac

В установщике Debian, если будет выбор программ, отметь **SSH server**. Если пропустил этот пункт, пока используй окно консоли VirtualBox и на каждой ВМ выполни:

```bash
sudo apt update
sudo apt install -y openssh-server
sudo systemctl enable --now ssh
sudo systemctl status ssh --no-pager
```

В статусе ожидается `Active: active (running)`.

На Mac открой Terminal. Сначала проверь вход по паролю:

```bash
ssh -p 2221 ТВОЙ_ЛОГИН@127.0.0.1
ssh -p 2222 ТВОЙ_ЛОГИН@127.0.0.1
ssh -p 2223 ТВОЙ_ЛОГИН@127.0.0.1
```

При первом входе подтверди ключ хоста, затем введи пароль Debian. Выйти из удалённой оболочки можно командой `exit`.

### Настрой вход по SSH-ключу

На Mac создай отдельный ключ для лабы. Если такой ключ уже создан, не запускай команду повторно с тем же путём:

```bash
ssh-keygen -t ed25519 -f ~/.ssh/gluster_lab -C "gluster-lab"
```

На вопросы о passphrase нажми Enter, если хочешь подключаться без дополнительного пароля. Передай публичный ключ на все ВМ:

```bash
cat ~/.ssh/gluster_lab.pub | ssh -p 2221 ТВОЙ_ЛОГИН@127.0.0.1 'umask 077; mkdir -p ~/.ssh; cat >> ~/.ssh/authorized_keys; chmod 700 ~/.ssh; chmod 600 ~/.ssh/authorized_keys'
cat ~/.ssh/gluster_lab.pub | ssh -p 2222 ТВОЙ_ЛОГИН@127.0.0.1 'umask 077; mkdir -p ~/.ssh; cat >> ~/.ssh/authorized_keys; chmod 700 ~/.ssh; chmod 600 ~/.ssh/authorized_keys'
cat ~/.ssh/gluster_lab.pub | ssh -p 2223 ТВОЙ_ЛОГИН@127.0.0.1 'umask 077; mkdir -p ~/.ssh; cat >> ~/.ssh/authorized_keys; chmod 700 ~/.ssh; chmod 600 ~/.ssh/authorized_keys'
```

Создай SSH-конфиг на Mac:

```bash
mkdir -p ~/.ssh
nano ~/.ssh/config
```

Вставь, заменив `ТВОЙ_ЛОГИН`:

```sshconfig
Host manage
    HostName 127.0.0.1
    Port 2221
    User ТВОЙ_ЛОГИН
    IdentityFile ~/.ssh/gluster_lab

Host cn01
    HostName 127.0.0.1
    Port 2222
    User ТВОЙ_ЛОГИН
    IdentityFile ~/.ssh/gluster_lab

Host cn02
    HostName 127.0.0.1
    Port 2223
    User ТВОЙ_ЛОГИН
    IdentityFile ~/.ssh/gluster_lab
```

В `nano` сохрани файл сочетанием `Ctrl+O`, Enter и выйди `Ctrl+X`. Ограничь доступ к конфигу:

```bash
chmod 600 ~/.ssh/config
```

Теперь можно подключаться коротко:

```bash
ssh manage
ssh cn01
ssh cn02
```

Команда на удалённой машине без входа в интерактивную оболочку:

```bash
ssh cn01 'hostname'
ssh -t cn01 'sudo gluster peer status'
```

Флаг `-t` выделяет терминал, чтобы `sudo` мог запросить пароль.

### Запускай и выключай ВМ из Terminal на Mac

Установи VirtualBox, затем в Terminal на Mac проверь имена ВМ:

```bash
VBoxManage list vms
```

Запусти ВМ без отдельных окон VirtualBox. Имена в кавычках должны совпадать с выводом `list vms`:

```bash
VBoxManage startvm "manage" --type headless
VBoxManage startvm "cn01" --type headless
VBoxManage startvm "cn02" --type headless
```

Если `VBoxManage` не найден, используй полный путь:

```bash
/Applications/VirtualBox.app/Contents/MacOS/VBoxManage list vms
```

Для корректного выключения отправь гостевой ОС запрос на завершение:

```bash
VBoxManage controlvm "manage" acpipowerbutton
VBoxManage controlvm "cn01" acpipowerbutton
VBoxManage controlvm "cn02" acpipowerbutton
```

SSH подключается только к включённой машине. Сначала запусти ВМ, подожди загрузки Debian и затем выполни `ssh cn01`.

## 3. Настрой адреса и имена внутренней сети

Зайди по SSH на каждую машину и посмотри имена интерфейсов:

```bash
ip -br link
ip -br addr
```

Обычно первый интерфейс NAT называется `enp0s3`, а второй интерфейс `labnet` — `enp0s8`. Используй реальные имена из своего вывода.

### Сделай IP-адреса постоянными

Сначала проверь, какой сетевой менеджер установлен:

```bash
command -v ifup
command -v nmcli
cat /etc/network/interfaces
```

Если используется `ifupdown` (команда `ifup` существует), открой `/etc/network/interfaces`:

```bash
sudo nano /etc/network/interfaces
```

Не удаляй существующую настройку NAT-интерфейса. Добавь блок для второго интерфейса, заменив адрес на адрес этой ВМ и при необходимости изменив имя `enp0s8`:

```text
auto enp0s8
iface enp0s8 inet static
    address 10.10.10.10
    netmask 255.255.255.0
```

На `cn01` адрес должен быть `10.10.10.11`, на `cn02` — `10.10.10.12`. Внутреннему интерфейсу не задавай `gateway`.

Применяй настройку с консоли VirtualBox, чтобы не потерять SSH-соединение при ошибке:

```bash
sudo ifdown enp0s8 2>/dev/null || true
sudo ifup enp0s8
```

Если установлен NetworkManager (`nmcli` существует), но `ifup` отсутствует, вместо правки файла выполни на соответствующей ВМ, подставив её IP:

```bash
sudo nmcli connection add type ethernet ifname enp0s8 con-name labnet \
  ipv4.method manual ipv4.addresses 10.10.10.10/24 ipv6.method disabled
sudo nmcli connection up labnet
```

Проверь адреса после настройки:

```bash
ip -br addr
```

На `manage` проверь связь с обоими серверами:

```bash
ping -c 3 cn01
ping -c 3 cn02
```

Переходи к GlusterFS, только когда оба ping проходят.

### Пропиши имена на всех ВМ

На `manage`, `cn01` и `cn02` добавь одинаковые строки в конец `/etc/hosts`:

```bash
sudo nano /etc/hosts
```

```text
10.10.10.10 manage
10.10.10.11 cn01
10.10.10.12 cn02
```

Проверь:

```bash
getent hosts manage cn01 cn02
```

## 4. Установи GlusterFS

Для Debian 12 пакеты `glusterfs-server` и `glusterfs-client` доступны из репозитория Debian. Сначала на всех ВМ обнови индексы пакетов:

```bash
sudo apt update
```

> **Важно для сдачи:** на `manage` ставится только клиент GlusterFS (`glusterfs-client`). Пакет `glusterfs-server` ставится только на `cn01` и `cn02`. Не устанавливай серверный пакет на клиентскую ВМ.

На `cn01` и `cn02`:

```bash
sudo apt install -y glusterfs-server
sudo systemctl enable --now glusterd
sudo systemctl status glusterd --no-pager
```

На `manage`:

```bash
sudo apt install -y glusterfs-client
```

Проверь установленную версию на серверах:

```bash
sudo gluster --version
```

## 5. Собери trusted storage pool

На `cn01` добавь `cn02` в пул и проверь состояние:

```bash
sudo gluster peer probe cn02
sudo gluster peer status
sudo gluster pool list
```

В `peer status` должно быть `Peer in Cluster (Connected)`. Команду `peer detach` пока не выполняй: сначала нужно остановить и удалить тома, использующие кирпичи `cn02`.

## 6. Подготовь отдельные каталоги-кирпичи

Кирпич GlusterFS — это отдельный каталог на сервере хранения. Не используй один и тот же каталог кирпича сразу для нескольких томов. Создай каталоги.

На `cn01`:

```bash
sudo mkdir -p /data/gluster/replica/brick
sudo mkdir -p /data/gluster/distributed/brick
sudo mkdir -p /data/gluster/arbiter-data/brick
```

На `cn02`:

```bash
sudo mkdir -p /data/gluster/replica/brick
sudo mkdir -p /data/gluster/distributed/brick
sudo mkdir -p /data/gluster/arbiter-data/brick
sudo mkdir -p /data/gluster/arbiter-metadata/brick
```

Для базовых экспериментов эти каталоги лежат на системной файловой системе. Для снапшотного тома ниже будут отдельные каталоги на отдельном LVM-диске.

## 7. Создай и проверь replica-том

На `cn01` создай том `vol_replica` с одной копией на каждом storage-сервере:

```bash
sudo gluster volume create vol_replica replica 2 \
  cn01:/data/gluster/replica/brick \
  cn02:/data/gluster/replica/brick force

sudo gluster volume start vol_replica
sudo gluster volume info vol_replica
```

В информации ожидается тип `Replicate`, состояние `Started` и два кирпича.

На `manage` смонтируй том и запиши файл:

```bash
sudo mkdir -p /mnt/replica
sudo mount -t glusterfs cn01:/vol_replica /mnt/replica
findmnt /mnt/replica
echo "hello from replica" | sudo tee /mnt/replica/test.txt
cat /mnt/replica/test.txt
```

На `cn01` и `cn02` проверь копии:

```bash
sudo cat /data/gluster/replica/brick/test.txt
```

Файл должен читаться на обоих серверах. Данные всегда записывай через `/mnt/replica`, не напрямую в каталог кирпича.

## 8. Создай distributed-том, добавь кирпич и перераспредели данные

Сначала на `cn01` создай distributed-том с одним кирпичом:

```bash
sudo gluster volume create vol_dist \
  cn01:/data/gluster/distributed/brick force
sudo gluster volume start vol_dist
sudo gluster volume info vol_dist
```

На `manage` смонтируй том и создай много небольших файлов, чтобы было что распределять:

```bash
sudo mkdir -p /mnt/dist
sudo mount -t glusterfs cn01:/vol_dist /mnt/dist
for i in $(seq 1 50); do
  echo "file $i" | sudo tee "/mnt/dist/file_$i.txt" >/dev/null
done
```

На `cn02` подготовь второй кирпич:

```bash
sudo mkdir -p /data/gluster/distributed/brick
```

На `cn01` добавь его и запусти rebalance:

```bash
sudo gluster volume add-brick vol_dist \
  cn02:/data/gluster/distributed/brick force
sudo gluster volume info vol_dist
sudo gluster volume rebalance vol_dist start
sudo gluster volume rebalance vol_dist status
```

Если статус `in progress`, подожди и повтори команду `status`. Проверь расположение файлов на серверах:

```bash
sudo find /data/gluster/distributed/brick -maxdepth 1 -type f | head
```

Часть файлов должна оказаться на обоих кирпичах. В distributed-томе копий нет: если потерять сервер или кирпич, файлы на нём могут стать недоступны.

### Удали кирпич из distributed-тома

Эта процедура переносит данные с удаляемого кирпича на оставшийся. Проводи её на лабораторных файлах. На `cn01`:

```bash
sudo gluster volume remove-brick vol_dist \
  cn02:/data/gluster/distributed/brick start
sudo gluster volume remove-brick vol_dist \
  cn02:/data/gluster/distributed/brick status
```

Когда статус завершён, подтверди изменение конфигурации:

```bash
sudo gluster volume remove-brick vol_dist \
  cn02:/data/gluster/distributed/brick commit
sudo gluster volume info vol_dist
```

Не удаляй каталог кирпича вручную до завершения операции. `remove-brick` запускает миграцию, а `commit` окончательно убирает кирпич из конфигурации.

## 9. Создай arbiter-том

Команда `replica 2 arbiter 1` означает два кирпича данных и один кирпич-арбитр. На `cn01` кирпичи данных будут обычными; на `cn02` второй каталог станет arbiter:

```bash
sudo gluster volume create vol_arbiter replica 2 arbiter 1 \
  cn01:/data/gluster/arbiter-data/brick \
  cn02:/data/gluster/arbiter-data/brick \
  cn02:/data/gluster/arbiter-metadata/brick force

sudo gluster volume start vol_arbiter
sudo gluster volume info vol_arbiter
```

Проверь, что третий кирпич помечен `(arbiter)`. На `manage`:

```bash
sudo mkdir -p /mnt/arbiter
sudo mount -t glusterfs cn01:/vol_arbiter /mnt/arbiter
echo "arbiter test" | sudo tee /mnt/arbiter/test.txt
```

На обоих кирпичах данных проверь содержимое `test.txt`. В `/data/gluster/arbiter-metadata/brick` одноимённый файл будет иметь размер 0 байт: arbiter хранит структуру и метаданные, а не пользовательское содержимое.

## 10. Включи и проверь квоты

Каталог, на который ставится лимит, сначала создай через смонтированный том. На `manage`:

```bash
sudo mkdir -p /mnt/replica/quotaTest
```

На `cn01`:

```bash
sudo gluster volume quota vol_replica enable
sudo gluster volume quota vol_replica limit-usage /quotaTest 10MB
sudo gluster volume quota vol_replica list
```

На `manage` попробуй записать файл размером больше лимита:

```bash
sudo dd if=/dev/zero of=/mnt/replica/quotaTest/bigfile.bin \
  bs=1M count=20 status=progress
```

Команда должна остановиться с ошибкой превышения квоты или не дать записать весь файл. Затем на `cn01` проверь квоту снова:

```bash
sudo gluster volume quota vol_replica list /quotaTest
```

Учти, что квота может обновлять статистику с задержкой и учитывает служебные расходы. Поэтому используем небольшой лимит и проверяем сам факт ограничения, а не точное совпадение числа байтов.

## 11. Подготовь LVM thin для снапшотов

Снапшоты GlusterFS требуют, чтобы кирпичи снапшотного тома находились на отдельных thin-provisioned LVM. На `cn01` и `cn02` используй дополнительный диск, добавленный в VirtualBox.

На обеих ВМ сначала найди диск:

```bash
lsblk
```

В отчёте дополнительный диск назывался `/dev/sdb`, но проверь имя у себя. **Не выполняй `pvcreate` на системном диске**. Если не уверен, какой диск пустой дополнительный, остановись и сравни вывод `lsblk`.

На обеих ВМ установи утилиты:

```bash
sudo apt install -y lvm2 thin-provisioning-tools xfsprogs
```

Следующие команды предполагают, что дополнительный диск — `/dev/sdb` объёмом около 10 ГБ.

На `cn01`:

```bash
sudo pvcreate /dev/sdb
sudo vgcreate vg_snap /dev/sdb
sudo lvcreate --type thin-pool -L 7G -n pool vg_snap
sudo lvcreate -V 5G -T vg_snap/pool -n brick
sudo mkfs.xfs -f /dev/vg_snap/brick
sudo mkdir -p /snapbrick
sudo mount /dev/vg_snap/brick /snapbrick
sudo mkdir -p /snapbrick/brick
findmnt /snapbrick
sudo lvs
```

На `cn02` используй отдельную группу `vg_snap2`:

```bash
sudo pvcreate /dev/sdb
sudo vgcreate vg_snap2 /dev/sdb
sudo lvcreate --type thin-pool -L 7G -n pool vg_snap2
sudo lvcreate -V 5G -T vg_snap2/pool -n brick
sudo mkfs.xfs -f /dev/vg_snap2/brick
sudo mkdir -p /snapbrick
sudo mount /dev/vg_snap2/brick /snapbrick
sudo mkdir -p /snapbrick/brick
findmnt /snapbrick
sudo lvs
```

В `findmnt` источником должен быть LV `brick`, а `lvs` должен показывать thin pool и thin LV. Не размещай в `/snapbrick` никакие данные, кроме каталога кирпича.

> После перезагрузки файловые системы могут потребовать повторного монтирования. До конца эксперимента со снапшотом не перезагружай storage-ВМ. Если нужен перезапуск, сначала проверь `sudo lvs`, затем смонтируй `/dev/vg_snap/brick` или `/dev/vg_snap2/brick` в `/snapbrick` на соответствующем сервере и убедись, что путь `/snapbrick/brick` существует.

## 12. Создай, просмотри и восстанови снапшот

На `cn01` создай отдельный replica-том на подготовленных LVM-кирпичах:

```bash
sudo gluster volume create vol_snap replica 2 \
  cn01:/snapbrick/brick \
  cn02:/snapbrick/brick force
sudo gluster volume start vol_snap
sudo gluster volume info vol_snap
```

На `manage` создай тестовый файл до снимка:

```bash
sudo mkdir -p /mnt/snap
sudo mount -t glusterfs cn01:/vol_snap /mnt/snap
echo "created before snapshot" | sudo tee /mnt/snap/before.txt
```

На `cn01` создай снимок и просмотри информацию:

```bash
sudo gluster snapshot create snap1 vol_snap no-timestamp
sudo gluster snapshot list vol_snap
sudo gluster snapshot info snap1
sudo gluster snapshot status snap1
```

Создай файл уже после снимка на `manage`:

```bash
echo "created after snapshot" | sudo tee /mnt/snap/after.txt
```

Чтобы отдельно просмотреть снимок, на `manage` смонтируй его read-only:

```bash
sudo mkdir -p /mnt/snapview
sudo mount -t glusterfs cn01:/snaps/snap1/vol_snap /mnt/snapview
ls -l /mnt/snapview
```

В `/mnt/snapview` ожидается `before.txt`, а `after.txt` отсутствует. После проверки:

```bash
sudo umount /mnt/snapview
```

### Восстанови том к состоянию снимка

Restore откатывает содержимое тома на момент снимка, поэтому выполняй его только на тестовых данных. На `manage` сначала отмонтируй живой том:

```bash
sudo umount /mnt/snap
```

На `cn01` останови том и восстанови снимок:

```bash
sudo gluster volume stop vol_snap
sudo gluster snapshot restore snap1
```

Подтверди действие, если Gluster запросит подтверждение и ты действительно хочешь откатить учебный том. После восстановления проверь состояние и запусти том, если он остановлен:

```bash
sudo gluster volume info vol_snap
sudo gluster volume start vol_snap
```

На `manage` смонтируй том заново:

```bash
sudo mount -t glusterfs cn01:/vol_snap /mnt/snap
ls -l /mnt/snap
```

После восстановления `before.txt` должен остаться, а `after.txt` — исчезнуть. Снимок, использованный для restore, может быть удалён из списка снимков.

## 13. Удали peer после томов

Выполняй этот раздел, только если эксперимент с пулом надо показать именно с удалением узла. На `manage` отмонтируй все тома, которые ещё смонтированы:

```bash
sudo umount /mnt/replica 2>/dev/null || true
sudo umount /mnt/dist 2>/dev/null || true
sudo umount /mnt/arbiter 2>/dev/null || true
sudo umount /mnt/snap 2>/dev/null || true
```

Если после просмотра или проверки остался снимок `vol_snap`, сначала удали снимки этого тома. Если restore уже был выполнен, снимок обычно уже удалён:

```bash
sudo gluster snapshot list vol_snap
sudo gluster snapshot delete volume vol_snap
```

На `cn01` останови и удали каждый созданный том. Если Gluster задаёт вопрос при остановке или удалении — внимательно прочитай его и подтверди только для учебных томов:

```bash
sudo gluster volume stop vol_replica
sudo gluster volume delete vol_replica

sudo gluster volume stop vol_dist
sudo gluster volume delete vol_dist

sudo gluster volume stop vol_arbiter
sudo gluster volume delete vol_arbiter

sudo gluster volume stop vol_snap
sudo gluster volume delete vol_snap
```

Если какой-то том не создавался, пропусти его команды. Теперь можно отсоединить peer и добавить его обратно:

```bash
sudo gluster peer detach cn02
sudo gluster peer status
sudo gluster peer probe cn02
sudo gluster peer status
```

Удаление тома стирает его конфигурацию GlusterFS. Не используй каталоги с важными данными для этой лабораторной.

## 14. Что показать в отчёте

Сохрани текстовые выводы или скриншоты следующих проверок:

1. `ip -br addr`, `ping cn01`, `ping cn02`, `getent hosts ...` — сеть и имена.
2. `systemctl status glusterd`, `gluster peer status` — службы и пул.
3. `gluster volume info vol_replica`, `vol_dist`, `vol_arbiter`, `vol_snap` — конфигурация типов томов.
4. Файл, записанный в replica через mount, и его наличие в обоих кирпичах.
5. Наличие файлов на обоих distributed-кирпичах и статус rebalance.
6. Вывод `volume info` с arbiter-кирпичом; размер файла на нём равен нулю.
7. Список квот до и после попытки превысить лимит.
8. `lsblk`, `findmnt /snapbrick`, `lvs`; `snapshot list/info/status`; файлы до и после restore.
9. `peer detach` и повторный `peer probe`, если демонстрируешь удаление пира.

В заключении коротко сравни поведение: replica хранит копии данных; distributed распределяет файлы без копий; arbiter хранит метаданные для кворума; quota ограничивает объём каталога; snapshot позволяет посмотреть и восстановить прежнее состояние тома.

## Если что-то не работает

- **`ssh` не подключается:** проверь, что ВМ включена, `openssh-server` установлен, `systemctl status ssh` показывает active, порт-форвардинг настроен для правильного NAT-адреса и порт `2221`/`2222`/`2223` не занят на Mac.
- **`ping cn01` не проходит:** проверь одинаковое имя внутренней сети `labnet`, подключён ли Adapter 2, совпадают ли IP и маска `/24`, корректно ли заполнен `/etc/hosts`.
- **`peer probe` не проходит:** проверь `getent hosts cn02`, `ping cn02`, затем на `cn02` — `systemctl status glusterd`. Если включён firewall, разреши Gluster-трафик между узлами по внутренней сети.
- **Том не монтируется на клиенте:** проверь `gluster volume info` и что статус Started; проверь доступность `cn01` по внутренней сети и установку `glusterfs-client` на `manage`.
- **Ошибка при создании снимка:** проверь, что оба brick-пути `vol_snap` смонтированы с thin LVM на правильных серверах, доступны Gluster-пулу, не содержат посторонних данных и все кирпичи online.
- **`pvcreate` показывает не тот диск или размер:** не подтверждай операцию; вернись к `lsblk` и определи дополнительный диск перед продолжением.

## Документация

- [GlusterFS: установка на Debian](https://docs.gluster.org/en/main/Install-Guide/Install/)
- [Debian 12: пакет glusterfs-server](https://packages.debian.org/bookworm/glusterfs-server)
- [GlusterFS: создание томов](https://docs.gluster.org/en/main/Administrator-Guide/Setting-Up-Volumes/)
- [GlusterFS: arbiter и кворум](https://docs.gluster.org/en/latest/Administrator-Guide/arbiter-volumes-and-quorum/)
- [GlusterFS: расширение, удаление кирпичей и rebalance](https://docs.gluster.org/en/main/Administrator-Guide/Managing-Volumes/)
- [GlusterFS: квоты каталогов](https://docs.gluster.org/en/main/Administrator-Guide/Directory-Quota/)
- [GlusterFS: снапшоты](https://docs.gluster.org/en/main/Administrator-Guide/Managing-Snapshots/)
- [VirtualBox: руководство пользователя, NAT port forwarding](https://download.virtualbox.org/virtualbox/7.0.16/UserManual.pdf)
