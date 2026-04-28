SELECT pz.parking_zone_id, pz.name 
FROM parking_zone pz
LEFT JOIN parking_session ps ON pz.parking_zone_id = ps.parking_zone_id AND ps.car_id = 1
WHERE ps.car_id IS NULL
ORDER BY pz.parking_zone_id;