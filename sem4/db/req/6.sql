WITH ClosedAlert AS (
	SELECT e.employee_id, e.full_name, COUNT(arps.status_id) AS closed_alert
	FROM employee e
	JOIN alert_event ae ON e.employee_id = ae.employee_id
	JOIN alert_event_process_status arps ON ae.status_id = arps.status_id
	WHERE arps.name = 'Закрыто'
	GROUP BY e.employee_id, e.full_name
)

SELECT ca.employee_id, ca.full_name
FROM ClosedAlert ca
WHERE ca.closed_alert < (
	SELECT closed_alert 
	FROM ClosedAlert
	WHERE employee_id = 1);
