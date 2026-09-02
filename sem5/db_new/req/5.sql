-- Запрос 5
-- Найти число зон парковок с одинаковым числом паркавшихся автомобилей для каждого сотрудника
SELECT e.full_name, sub.car_count, COUNT(*) AS zone_count
FROM employee e
JOIN (
    SELECT ae.employee_id, ps.parking_zone_id, COUNT(DISTINCT ae.car_id) AS car_count
    FROM alert_event ae
    JOIN parking_session ps ON ps.car_id = ae.car_id
    GROUP BY ae.employee_id, ps.parking_zone_id
) sub ON sub.employee_id = e.employee_id
GROUP BY e.full_name, sub.car_count
ORDER BY e.full_name, sub.car_count;
