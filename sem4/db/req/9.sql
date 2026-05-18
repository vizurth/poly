-- Запрос 9
-- Для всех треков автомобиля "А001АА77" поменять статус с "Активный" на "Завершён"
UPDATE track
SET track_status_id = (SELECT track_status_id FROM track_status_type WHERE name = 'Завершён')
WHERE car_id = (SELECT car_id FROM car WHERE reg_number = 'А001АА77')
  AND track_status_id = (SELECT track_status_id FROM track_status_type WHERE name = 'Активный');
