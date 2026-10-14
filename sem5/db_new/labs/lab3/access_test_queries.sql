-- [1] Выполнить от имени user_a И user_b — у обоих должно быть OK.
SELECT * FROM car_stats LIMIT 5;

-- [2] Выполнить отдельно от имени user_a И user_b — у обоих DENIED.
INSERT INTO car_stats (car_id, reg_number, alert_count, parking_count)
VALUES (999999, 'Т999ТТ78', 0, 0);

-- [3] Выполнить отдельно от имени user_a И user_b — у обоих DENIED.
UPDATE car_stats
SET alert_count = 100
WHERE car_id = 1;

-- [4] Выполнить отдельно от имени user_a И user_b — у обоих DENIED.
DELETE FROM car_stats
WHERE car_id = 1;

-- [5] Проверка доступа к исходной таблице.
-- user_a: DENIED. user_b: OK.
SELECT * FROM car LIMIT 3;

-- [6] Проверка добавления тревожного события.
-- Сначала выполнить от имени user_a: DENIED.
INSERT INTO alert_event (
    car_id, alert_event_type_id, status_id, description
)
VALUES (
    1, 1, 1, 'Проверка прав пользователей'
)
RETURNING alert_event_id;

-- [7] Проверка результата работы триггера после INSERT.
-- Запрос выполняется от имени ОБОИХ пользователей; оба должны увидеть
-- alert_count, увеличившийся на 1 для car_id = 1.
SELECT car_id, alert_count
FROM car_stats
WHERE car_id = 1;

-- [8] Проверка UPDATE исходной таблицы.
-- user_a: DENIED. user_b: UPDATE 1; счётчик не меняется, так как car_id прежний.
UPDATE alert_event
SET description = 'Проверка UPDATE прав пользователя Б'
WHERE alert_event_id = :alert_event_id;

-- [9] Проверка DELETE исходной таблицы.
-- Сначала выполнить от имени user_a: DENIED.
DELETE FROM alert_event
WHERE alert_event_id = :alert_event_id;

-- Затем выполнить от имени user_b: ожидается DELETE 1.
DELETE FROM alert_event
WHERE alert_event_id = :alert_event_id;

-- Повторить запрос от имени ОБОИХ пользователей.
-- alert_count должен вернуться к значению до тестовой вставки.
SELECT car_id, alert_count
FROM car_stats
WHERE car_id = 1;

-- [10] Проверка таблицы, на которую права не выдавались.
-- Выполнить от имени user_a И user_b; оба должны получить DENIED.
SELECT * FROM alert_event_type LIMIT 3;