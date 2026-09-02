-- Запрос 4 MAX
-- Найти зоны парковки, в которых парковалось максимальное количество автомобилей
SELECT pz.parking_zone_id, pz.name, COUNT(DISTINCT ps.car_id) AS cnt
FROM parking_zone pz
JOIN parking_session ps ON ps.parking_zone_id = pz.parking_zone_id
GROUP BY pz.parking_zone_id, pz.name
HAVING COUNT(DISTINCT ps.car_id) = (
    SELECT MAX(sub.car_count)
    FROM (
        SELECT COUNT(DISTINCT ps2.car_id) AS car_count
        FROM parking_session ps2
        GROUP BY ps2.parking_zone_id
    ) sub
);

-- Запрос 4 MIN
-- Найти зоны парковки, в которых парковалось минимальное количество автомобилей
SELECT pz.parking_zone_id, pz.name, COUNT(DISTINCT ps.car_id) AS cnt
FROM parking_zone pz
JOIN parking_session ps ON ps.parking_zone_id = pz.parking_zone_id
GROUP BY pz.parking_zone_id, pz.name
HAVING COUNT(DISTINCT ps.car_id) = (
    SELECT MIN(sub.car_count)
    FROM (
        SELECT COUNT(DISTINCT ps2.car_id) AS car_count
        FROM parking_session ps2
        GROUP BY ps2.parking_zone_id
    ) sub
);
