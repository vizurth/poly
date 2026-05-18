-- Запрос 7
-- Найти зоны парковки, в которых никогда не парковался автомобиль "А001АА77"
SELECT pz.parking_zone_id, pz.name
FROM parking_zone pz
WHERE pz.parking_zone_id NOT IN (
    SELECT ps.parking_zone_id
    FROM parking_session ps
    JOIN car c ON c.car_id = ps.car_id
    WHERE c.reg_number = 'А001АА77'
);
