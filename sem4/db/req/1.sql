-- Запрос 1.1
-- Найти все автомобили, которые парковались в типе зоны "Бесплатная",
-- у которого тревожное событие "Потеря связи" и источник координат "GPS"
SELECT DISTINCT c.car_id, c.reg_number, c.brand, c.model
FROM car c
JOIN parking_session ps ON ps.car_id = c.car_id
JOIN parking_zone pz ON pz.parking_zone_id = ps.parking_zone_id
JOIN parking_zone_type pzt ON pzt.zone_type_id = pz.zone_type_id
JOIN alert_event ae ON ae.car_id = c.car_id
JOIN alert_event_type aet ON aet.alert_event_type_id = ae.alert_event_type_id
JOIN track t ON t.car_id = c.car_id
JOIN track_point tp ON tp.track_id = t.track_id
JOIN data_source_type dst ON dst.data_source_id = tp.data_source_id
WHERE pzt.name = 'Бесплатная'
  AND aet.name = 'Потеря связи'
  AND dst.name = 'GPS';

-- Запрос 1.2
-- То же самое, но автомобиль парковался не менее 3 раз
SELECT c.car_id, c.reg_number, c.brand, c.model, COUNT(DISTINCT ps.parking_session_id) AS cnt
FROM car c
JOIN parking_session ps ON ps.car_id = c.car_id
JOIN parking_zone pz ON pz.parking_zone_id = ps.parking_zone_id
JOIN parking_zone_type pzt ON pzt.zone_type_id = pz.zone_type_id
JOIN alert_event ae ON ae.car_id = c.car_id
JOIN alert_event_type aet ON aet.alert_event_type_id = ae.alert_event_type_id
JOIN track t ON t.car_id = c.car_id
JOIN track_point tp ON tp.track_id = t.track_id
JOIN data_source_type dst ON dst.data_source_id = tp.data_source_id
WHERE pzt.name = 'Бесплатная'
  AND aet.name = 'Потеря связи'
  AND dst.name = 'GPS'
GROUP BY c.car_id, c.reg_number, c.brand, c.model
HAVING COUNT(DISTINCT ps.parking_session_id) >= 3;
