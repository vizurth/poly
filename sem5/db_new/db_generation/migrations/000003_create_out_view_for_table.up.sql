CREATE TABLE car_stats (
    car_id        INT PRIMARY KEY REFERENCES car(car_id) ON DELETE CASCADE,
    reg_number    VARCHAR(9) NOT NULL,
    alert_count   BIGINT NOT NULL DEFAULT 0,
    parking_count BIGINT NOT NULL DEFAULT 0
);

