-- Создание таблицы
CREATE TABLE car_stats (
    car_id        INT PRIMARY KEY REFERENCES car(car_id) ON DELETE CASCADE,
    reg_number    VARCHAR(9) NOT NULL,
    alert_count   BIGINT NOT NULL DEFAULT 0,
    parking_count BIGINT NOT NULL DEFAULT 0
);

-- Переименование view
ALTER VIEW car_stats RENAME TO car_stats_view;

-- Заполнение таблицы
INSERT INTO car_stats (
    car_id,
    reg_number,
    alert_count,
    parking_count
)
SELECT
    car_id,
    reg_number,
    alert_count,
    parking_count
FROM car_stats_view;

-- Установка триггеров

-- Триггеры на таблицу car
CREATE OR REPLACE FUNCTION car_stats_change()
RETURNS TRIGGER AS $$
BEGIN
	-- insert
	IF TG_OP = 'INSERT' THEN
		INSERT INTO car_stats (
			car_id,
			reg_number,
			alert_count,
			parking_count
		) VALUES (
			NEW.car_id
			NEW.reg_number
			0,
			0
		);

		RETURN NEW;
	END IF;

	-- update
	IF TG_OP = 'UPDATE' THEN
		UPDATE car_stats
		SET reg_number = NEW.reg_number
		WHERE car_id = NEW.car_id

		RETURN NEW;
	END IF;

	-- delete
	IF TG_OP = 'DELETE' THEN
		DELETE FROM car_stats
		WHERE car_id = OLD.car_id

		RETURN OLD;
	END IF;

END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER car_stats_after_insert
AFTER INSERT ON car
FOR EACH ROW
EXECUTE FUNCTION car_stats_change();

CREATE TRIGGER car_stats_after_reg_number_update
AFTER UPDATE ON car
FOR EACH ROW
WHEN (OLD.reg_number IS DISTINCT FROM NEW.reg_number)
EXECUTE FUNCTION car_stats_change();

CREATE TRIGGER car_stats_before_delete
BEFORE DELETE ON car
FOR EACH ROW
EXECUTE FUNCTION car_stats_change();

-- Триггеры на parking_session

CREATE OR REPLACE FUNCTION parking_session_stats_delete()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM parking_session
        WHERE car_id = OLD.car_id
          AND parking_zone_id = OLD.parking_zone_id
          AND parking_session_id <> OLD.parking_session_id
    ) THEN
        UPDATE car_stats
        SET parking_count = GREATEST(parking_count - 1, 0)
        WHERE car_id = OLD.car_id;
    END IF;

    RETURN OLD;
END;
$$ LANGUAGE plpgsql;

-- delete
CREATE TRIGGER parking_session_before_delete
BEFORE DELETE ON parking_session
FOR EACH ROW
EXECUTE FUNCTION parking_session_stats_delete();

-- Триггеры на alert_event
CREATE OR REPLACE FUNCTION alert_event_stats_change()
RETURNS TRIGGER AS $$
BEGIN
    -- Добавили тревожное событие
    IF TG_OP = 'INSERT' THEN
        UPDATE car_stats
        SET alert_count = alert_count + 1
        WHERE car_id = NEW.car_id;

        RETURN NEW;
    END IF;

    -- Удалили тревожное событие
    IF TG_OP = 'DELETE' THEN
        UPDATE car_stats
        SET alert_count = GREATEST(alert_count - 1, 0)
        WHERE car_id = OLD.car_id;

        RETURN OLD;
    END IF;
END;
$$ LANGUAGE plpgsql;

-- insert
CREATE TRIGGER alert_event_after_insert
AFTER INSERT ON alert_event
FOR EACH ROW
EXECUTE FUNCTION alert_event_stats_change();

-- delete
CREATE TRIGGER alert_event_after_delete
AFTER DELETE ON alert_event
FOR EACH ROW
EXECUTE FUNCTION alert_event_stats_change();
