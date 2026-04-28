SELECT dst.name AS data_sourse_name, c.reg_number AS car_reg_number, COUNT(tp.track_point_id) AS total_track_points
FROM track_point tp
JOIN data_source_type dst ON tp.data_source_id = dst.data_source_id
JOIN car c ON tp.car_id = c.car_id
GROUP BY dst.name, c.reg_number
ORDER BY data_source_name, points_count DESC;
