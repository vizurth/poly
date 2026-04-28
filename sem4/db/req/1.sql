SELECT DISTINCT c.reg_number, aet.name, COUNT(c.reg_number)
FROM car c

JOIN parking_session ps ON c.car_id = ps.car_id
JOIN parking_zone pz ON ps.parking_zone_id = pz.parking_zone_id

JOIN alert_event ae ON c.car_id = ae.car_id
JOIN alert_event_type aet ON ae.alert_event_type_id = aet.alert_event_type_id

JOIN track_point tp ON c.car_id = tp.car_id
JOIN data_source_type dst ON tp.data_source_id = dst.data_source_id

WHERE pz.name = 'ТЦ Мега — парковка В'
AND aet.name = 'Выезд за границу'
AND dst.name = 'GPS'

GROUP BY c.reg_number;