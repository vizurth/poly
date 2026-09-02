-- Создание View
CREATE VIEW car_stats AS
SELECT
	c.car_id,
	c.reg_number,
	COUNT(DISTINCT a.alert_event_id) AS alert_count,
	COUNT(DISTINCT p.parking_zone_id) AS parking_count
FROM
	cars c
LEFT JOIN
	alerts a ON c.car_id = a.car_id
LEFT JOIN
	parking p ON c.car_id = p.car_id
GROUP BY
	c.car_id, c.reg_number;

-- Запрос 1
SELECT
	c.reg_number,
	c.alert_count,
	c.parking_count,
FROM
	car_stats c
WHERE
	c.parking_count > 10
	AND
	c.alert_count > 1;

-- Запрос 2
SELECT
	c.reg_number,
	c.alert_count,
FROM
	car_stats c
LEFT JOIN
	alert_event a ON c.car_id = a.car_id
LEFT JOIN
	alert_event_process_status s ON a.alert_event_id = s.alert_event_id
LEFT JOIN
	alert_event_type t ON a.alert_event_id = t.alert_event_id
WHERE
	c.alert_count > N
	AND 
	s.status = 'pending'
	AND
	t.type_name = 'A'