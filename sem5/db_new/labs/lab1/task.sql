-- Создание View
CREATE VIEW car_stats AS
SELECT
	c.car_id,
	c.reg_number,
	COUNT(DISTINCT a.alert_event_id) AS alert_count,
	COUNT(DISTINCT pz.parking_zone_id) AS parking_count
FROM car c
LEFT JOIN alert_event a ON c.car_id = a.car_id
LEFT JOIN parking_session ps ON c.car_id = ps.car_id
LEFT JOIN parking_zone pz ON ps.parking_zone_id = pz.parking_zone_id
GROUP BY c.car_id, c.reg_number;

-- Запрос 1
SELECT
	c.reg_number,
	c.alert_count,
	c.parking_count
FROM
	car_stats c
WHERE
	c.parking_count > 17
	AND
	c.alert_count > 1;

-- Запрос 2
SELECT
	c.reg_number,
	c.alert_count
FROM
	car_stats c
LEFT JOIN
	alert_event a ON c.car_id = a.car_id
LEFT JOIN
	alert_event_process_status s ON a.status_id = s.status_id
LEFT JOIN
	alert_event_type t ON a.alert_event_type_id = t.alert_event_type_id
WHERE
	c.parking_count < 18
	AND 
	s.name = 'Новое'
	AND
	t.name = 'Потеря связи';