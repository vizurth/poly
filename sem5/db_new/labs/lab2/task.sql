CREATE TABLE
	c.car_id,
	c.reg_number,
	COUNT(DISTINCT a.alert_event_id) AS alert_count,
	COUNT(DISTINCT pz.parking_zone_id) AS parking_count
FROM car c
LEFT JOIN alert_event a ON c.car_id = a.car_id
LEFT JOIN parking_session ps ON c.car_id = ps.car_id
LEFT JOIN parking_zone pz ON ps.parking_zone_id = pz.parking_zone_id
GROUP BY c.car_id, c.reg_number;