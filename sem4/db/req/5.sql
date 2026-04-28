WITH CarCount AS (
	SELECT pz.parking_zone_id, pz.name, COUNT(DISTINCT ps.car_id) AS total_cars
FROM parking_zone pz
JOIN parking_session ps ON pz.parking_zone_id = ps.parking_zone_id
GROUP BY pz.parking_zone_id, pz.name
)

SELECT COUNT(DISTINCT parking_zone_id) AS total_parking_zones, total_cars
FROM CarCount
GROUP BY total_cars
ORDER BY total_cars DESC;