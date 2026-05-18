-- Запрос 8
-- Для каждого источника данных и автомобиля посчитать количество координат
SELECT c.car_id, c.reg_number, dst.name AS data_source, COUNT(tp.track_point_id) AS coord_count
FROM data_source_type dst
JOIN track_point tp ON tp.data_source_id = dst.data_source_id
JOIN car c ON c.car_id = tp.car_id
GROUP BY c.car_id, c.reg_number, dst.data_source_id, dst.name
ORDER BY c.car_id, dst.name;
