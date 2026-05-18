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
    SELECT e.full_name,
           COUNT(DISTINCT ae.alert_event_id) AS alert_count,
           COUNT(DISTINCT ae.car_id)         AS car_count
    FROM employee e
    LEFT JOIN alert_event ae ON ae.employee_id = e.employee_id
    GROUP BY e.employee_id, e.full_name
    ORDER BY e.employee_id
""")
rows = cur.fetchall()
cur.close()
conn.close()

names      = [r[0].split()[0] for r in rows]   # фамилия
alerts     = [r[1] for r in rows]
cars       = [r[2] for r in rows]

x      = np.arange(len(names))
width  = 0.4

fig, ax = plt.subplots(figsize=(16, 6))
ax.bar(x - width/2, alerts, width, label='Тревожные события', color='#4C72B0')
ax.bar(x + width/2, cars,   width, label='Автомобили',        color='#DD8452')

ax.set_xticks(x)
ax.set_xticklabels(names, rotation=45, ha='right', fontsize=9)
ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))
ax.set_xlabel('Сотрудник')
ax.set_ylabel('Количество')
ax.set_title('Запрос 3 — Число тревожных событий и автомобилей по каждому сотруднику')
ax.legend()
ax.grid(axis='y', linestyle='--', alpha=0.5)

plt.tight_layout()
plt.savefig('charts/hist3.png', dpi=150)
print("Сохранено: charts/hist3.png")
