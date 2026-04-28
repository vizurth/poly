WITH CarCount AS (
	SELECT pz.parking_zone_id, pz.name, COUNT(DISTINCT ps.car_id) AS total_cars
FROM parking_zone pz
JOIN parking_session ps ON pz.parking_zone_id = ps.parking_zone_id
GROUP BY pz.parking_zone_id, pz.name
)
SELECT parking_zone_id, name, total_cars
FROM CarCount
WHERE total_cars = (SELECT MAX(total_cars) FROM CarCount);

WITH CarCount AS (
	SELECT pz.parking_zone_id, pz.name, COUNT(DISTINCT ps.car_id) AS total_cars
FROM parking_zone pz
JOIN parking_session ps ON pz.parking_zone_id = ps.parking_zone_id
GROUP BY pz.parking_zone_id, pz.name
)
SELECT parking_zone_id, name, total_cars
FROM CarCount
WHERE total_cars = (SELECT MIN(total_cars) FROM CarCount);