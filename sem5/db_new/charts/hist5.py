import psycopg2
import matplotlib.pyplot as plt
import numpy as np

conn = psycopg2.connect(
    host="localhost", port=5432,
    dbname="db", user="db-user", password="db-password"
)
cur = conn.cursor()

cur.execute("""
    SELECT e.full_name, sub.car_count, COUNT(*) AS zone_count
    FROM employee e
    JOIN (
        SELECT ae.employee_id, ps.parking_zone_id,
               COUNT(DISTINCT ae.car_id) AS car_count
        FROM alert_event ae
        JOIN parking_session ps ON ps.car_id = ae.car_id
        GROUP BY ae.employee_id, ps.parking_zone_id
    ) sub ON sub.employee_id = e.employee_id
    GROUP BY e.full_name, sub.car_count
    ORDER BY e.full_name, sub.car_count
""")
rows = cur.fetchall()
cur.close()
conn.close()

names_all  = sorted(set(r[0] for r in rows))
surnames   = [n.split()[0] for n in names_all]
car_counts = sorted(set(r[1] for r in rows))

data = {name: {cc: 0 for cc in car_counts} for name in names_all}
for name, cc, zc in rows:
    data[name][cc] = zc

x     = np.arange(len(names_all))
width = 0.8 / len(car_counts)
colors = plt.cm.tab10(np.linspace(0, 0.6, len(car_counts)))

fig, ax = plt.subplots(figsize=(18, 6))
for i, cc in enumerate(car_counts):
    vals = [data[name][cc] for name in names_all]
    ax.bar(x + i * width - 0.4 + width/2, vals, width,
           label=f'{cc} авт.', color=colors[i])

ax.set_xticks(x)
ax.set_xticklabels(surnames, rotation=45, ha='right', fontsize=8)
ax.set_xlabel('Сотрудник')
ax.set_ylabel('Число зон парковки')
ax.set_title('Запрос 5 — Число зон парковки с одинаковым кол-вом автомобилей по каждому сотруднику')
ax.legend(title='Кол-во авт. в зоне', fontsize=8)
ax.grid(axis='y', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.savefig('charts/hist5.png', dpi=150)
print("Сохранено: charts/hist5.png")
