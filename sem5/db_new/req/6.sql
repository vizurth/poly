-- Запрос 6
-- Найти сотрудников, которые обработали больше тревожных событий, чем "Иванов Игорь Николаевич"
SELECT e.employee_id, e.full_name, COUNT(ae.alert_event_id) AS cnt
FROM employee e
JOIN alert_event ae ON ae.employee_id = e.employee_id
GROUP BY e.employee_id, e.full_name
HAVING COUNT(ae.alert_event_id) > (
    SELECT COUNT(ae2.alert_event_id)
    FROM alert_event ae2
    JOIN employee e2 ON e2.employee_id = ae2.employee_id
    WHERE e2.full_name = 'Иванов Игорь Николаевич'
)
ORDER BY cnt DESC;
