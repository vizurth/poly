import psycopg2
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

conn = psycopg2.connect(
    host="localhost", port=5432,
    dbname="db", user="db-user", password="db-password"
)
cur = conn.cursor()

cur.execute("""
    SELECT c.car_id, c.reg_number, dst.name AS data_source,
       COUNT(tp.track_point_id) AS coord_count
FROM (
    SELECT car_id, reg_number
    FROM car
) c
CROSS JOIN data_source_type dst
LEFT JOIN track_point tp
    ON tp.car_id = c.car_id
    AND tp.data_source_id = dst.data_source_id
GROUP BY c.car_id, c.reg_number, dst.data_source_id, dst.name
ORDER BY c.car_id, dst.name;
""")
rows = cur.fetchall()
cur.close()
conn.close()

car_ids  = sorted(set(r[0] for r in rows))[:10]
sources  = sorted(set(r[2] for r in rows))

data = {(r[0], r[2]): r[3] for r in rows if r[0] in car_ids}

xpos = np.arange(len(car_ids))
ypos = np.arange(len(sources))
xposM, yposM = np.meshgrid(xpos, ypos, indexing='ij')

dz = np.array([[data.get((cid, src), 0) for src in sources] for cid in car_ids])

fig = plt.figure(figsize=(18, 10))
ax  = fig.add_subplot(111, projection='3d')

colors = plt.cm.tab10(np.linspace(0, 0.8, len(sources)))
bar_w  = 0.6
bar_d  = 0.6

for yi, src in enumerate(sources):
    ax.bar3d(
        xposM[:, yi].ravel(), yposM[:, yi].ravel(), np.zeros(len(car_ids)),
        bar_w, bar_d, dz[:, yi].ravel(),
        color=colors[yi], alpha=0.85, label=src
    )

ax.set_xticks(xpos[::10])
ax.set_xticklabels([str(cid) for cid in car_ids[::10]], fontsize=7, rotation=30)
ax.set_yticks(ypos)
ax.set_yticklabels(sources, fontsize=8)
ax.set_xlabel('ID автомобиля', labelpad=10)
ax.set_ylabel('Источник данных', labelpad=10)
ax.set_zlabel('Кол-во координат', labelpad=8)
ax.set_title('Запрос 8 — Количество координат по автомобилю и источнику данных')
ax.legend(loc='upper left', fontsize=8)

plt.tight_layout()
plt.savefig('charts/hist8.png', dpi=150)
print("Сохранено: charts/hist8.png")
