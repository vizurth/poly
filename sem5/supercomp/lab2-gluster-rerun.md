# GlusterFS lab: повторный прогон командами

```text
Предполагается: Debian-ВМ, SSH, статические IP и /etc/hosts уже настроены.
Для нового чистого прогона замени run2 в путях на новый номер.
Не используй уже заполненные brick-каталоги повторно.
```

```text
manage  10.10.10.10  клиент
cn01    10.10.10.11  storage / управление
cn02    10.10.10.12  storage
```

## Подключение с Mac

```bash
ssh manage
ssh cn01
ssh cn02
```

## Пакеты и службы

На `manage`:

```bash
sudo apt update
sudo apt install -y glusterfs-client
```

На `cn01` и `cn02`:

```bash
sudo apt update
sudo apt install -y glusterfs-server
sudo systemctl enable --now glusterd
sudo systemctl is-active glusterd
```

## LVM thin для brick и снапшотов

```bash
# На cn01 и cn02; только на новом пустом дополнительном диске
lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS
sudo apt install -y lvm2 thin-provisioning-tools xfsprogs
sudo pvcreate /dev/sdb
sudo vgcreate vg /dev/sdb
sudo lvcreate --type thin-pool -L 8G -n pool vg
sudo lvcreate -V 7G -T vg/pool -n brick
sudo mkfs.xfs /dev/vg/brick
sudo mkdir -p /snapbrick
sudo mount /dev/vg/brick /snapbrick
sudo mkdir -p /snapbrick/run2/replica/brick
findmnt /snapbrick
sudo lvs
```

```bash
# На cn01 и cn02; добавить автомонтирование в /etc/fstab
UUID=$(sudo blkid -s UUID -o value /dev/vg/brick)
grep -q ' /snapbrick ' /etc/fstab || echo "UUID=$UUID /snapbrick xfs defaults 0 0" | sudo tee -a /etc/fstab
```

## Trusted storage pool

На `cn01`:

```bash
sudo gluster peer probe cn02
sudo gluster peer status
sudo gluster pool list
```

## Replica volume

На `cn01` и `cn02`:

```bash
sudo mkdir -p /snapbrick/run2/replica/brick
```

На `cn01`:

```bash
sudo gluster volume create replica replica 2 \
  cn01:/snapbrick/run2/replica/brick \
  cn02:/snapbrick/run2/replica/brick
sudo gluster volume start replica
sudo gluster volume info replica
```

На `manage`:

```bash
sudo mkdir -p /mnt/replica
sudo mount -t glusterfs cn01:/replica /mnt/replica
echo "hello gluster" | sudo tee /mnt/replica/test.txt
cat /mnt/replica/test.txt
```

На `cn01` и `cn02`:

```bash
sudo cat /snapbrick/run2/replica/brick/test.txt
```

## Distributed volume: add brick и rebalance

На `cn01` и `cn02`:

```bash
sudo mkdir -p /data/gluster/run2/dist/brick
```

На `cn01`:

```bash
sudo gluster volume create dist cn01:/data/gluster/run2/dist/brick force
sudo gluster volume start dist
sudo gluster volume info dist
```

На `manage`:

```bash
sudo mkdir -p /mnt/dist
sudo mount -t glusterfs cn01:/dist /mnt/dist
for i in $(seq 1 10); do
  echo "file $i" | sudo tee "/mnt/dist/file_$i.txt" >/dev/null
done
find /mnt/dist -maxdepth 1 -type f | wc -l
```

На `cn01`:

```bash
sudo gluster volume add-brick dist cn02:/data/gluster/run2/dist/brick force
sudo gluster volume info dist
sudo gluster volume rebalance dist start
sudo gluster volume rebalance dist status
```

Повторять `rebalance status`, пока состояние не станет `completed`.

На `manage` проверить общее количество:

```bash
find /mnt/dist -maxdepth 1 -type f | wc -l
```

На `cn01` и `cn02` посмотреть число файлов на каждом brick:

```bash
sudo find /data/gluster/run2/dist/brick -maxdepth 1 -type f | wc -l
```

На `cn01` удалить brick с переносом данных:

```bash
sudo gluster volume remove-brick dist cn02:/data/gluster/run2/dist/brick start
sudo gluster volume remove-brick dist cn02:/data/gluster/run2/dist/brick status
```

После статуса `completed` на `cn01`:

```bash
sudo gluster volume remove-brick dist cn02:/data/gluster/run2/dist/brick commit
sudo gluster volume info dist
```

## Arbiter volume

На `cn01`:

```bash
sudo mkdir -p /data/gluster/run2/arbiter-data/brick
```

На `cn02`:

```bash
sudo mkdir -p /data/gluster/run2/arbiter-data/brick
sudo mkdir -p /data/gluster/run2/arbiter-metadata/brick
```

На `cn01`:

```bash
sudo gluster volume create arbiter replica 3 arbiter 1 \
  cn01:/data/gluster/run2/arbiter-data/brick \
  cn02:/data/gluster/run2/arbiter-data/brick \
  cn02:/data/gluster/run2/arbiter-metadata/brick force
sudo gluster volume start arbiter
sudo gluster volume info arbiter
```

На `manage`:

```bash
sudo mkdir -p /mnt/arbiter
sudo mount -t glusterfs cn01:/arbiter /mnt/arbiter
echo "arbiter test" | sudo tee /mnt/arbiter/test.txt
```

На `cn01` и `cn02` проверить data brick:

```bash
sudo stat -c '%n: %s bytes' /data/gluster/run2/arbiter-data/brick/test.txt
sudo cat /data/gluster/run2/arbiter-data/brick/test.txt
```

На `cn02` проверить arbiter brick:

```bash
sudo stat -c '%n: %s bytes' /data/gluster/run2/arbiter-metadata/brick/test.txt
```

## Directory quota

На `manage`:

```bash
sudo mkdir -p /mnt/replica/quotaTest
```

На `cn01`:

```bash
sudo gluster volume quota replica enable
sudo gluster volume quota replica limit-usage /quotaTest 10MB
sudo gluster volume quota replica list /quotaTest
```

На `manage` записать тестовый файл, затем на `cn01` снова посмотреть квоту:

```bash
sudo dd if=/dev/zero of=/mnt/replica/quotaTest/bigfile.bin bs=1M count=20 status=progress
```

```bash
sudo gluster volume quota replica list /quotaTest
```

## Snapshot: create, view, restore

На `manage` записать файл до снимка:

```bash
echo "before snapshot" | sudo tee /mnt/replica/before-snap.txt
```

На `cn01`:

```bash
sudo gluster snapshot create snap1 replica no-timestamp
sudo gluster snapshot list replica
sudo gluster snapshot info snap1
sudo gluster snapshot status snap1
sudo gluster snapshot activate snap1
```

На `manage` создать файл после снимка и подключить снимок для просмотра:

```bash
echo "after snapshot" | sudo tee /mnt/replica/after-snap.txt
sudo mkdir -p /mnt/snapview
sudo mount -t glusterfs cn01:/snaps/snap1/replica /mnt/snapview
ls -l /mnt/snapview
```

В `/mnt/snapview` должен быть `before-snap.txt`, но не `after-snap.txt`.

На `manage` отмонтировать снимок и рабочий том:

```bash
sudo umount /mnt/snapview
sudo umount /mnt/replica
```

На `cn01` восстановить снимок; подтвердить запросы `y`:

```bash
sudo gluster volume stop replica
sudo gluster snapshot restore snap1
sudo gluster volume start replica
```

На `manage` снова смонтировать том и посмотреть файлы:

```bash
sudo mount -t glusterfs cn01:/replica /mnt/replica
ls -l /mnt/replica
```

## Удалить тома и отсоединить peer

На `manage`:

```bash
sudo umount /mnt/replica
sudo umount /mnt/dist
sudo umount /mnt/arbiter
```

На `cn01`; подтверждать остановку и удаление только своих учебных томов:

```bash
sudo gluster volume stop replica
sudo gluster volume delete replica
sudo gluster volume stop dist
sudo gluster volume delete dist
sudo gluster volume stop arbiter
sudo gluster volume delete arbiter
sudo gluster volume list
```

Затем на `cn01`:

```bash
sudo gluster peer detach cn02
sudo gluster peer status
sudo gluster pool list
```
