SELECT e.full_name, COUNT(ae.alert_event_id) AS total_alert_events,
COUNT(DISTINCT ae.car_id) AS total_unique_cars
FROM employee e
LEFT JOIN alert_event ae ON e.employee_id = ae.employee_id
GROUP BY e.employee_id, e.full_name, 
ORDER BY total_alert_events DESC;