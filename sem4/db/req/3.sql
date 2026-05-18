-- Запрос 3
-- Для каждого сотрудника посчитать число тревожных событий и количество автомобилей
SELECT e.employee_id, e.full_name,
       COUNT(DISTINCT ae.alert_event_id) AS alert_count,
       COUNT(DISTINCT ae.car_id) AS car_count
FROM employee e
LEFT JOIN alert_event ae ON ae.employee_id = e.employee_id
GROUP BY e.employee_id, e.full_name
ORDER BY e.employee_id;
