CREATE TABLE employee_type (
    employee_type_id   SERIAL PRIMARY KEY,
    name               VARCHAR(50) NOT NULL UNIQUE,
    description        TEXT
);

CREATE TABLE employee (
    employee_id        SERIAL PRIMARY KEY,
    employee_type_id   INT NOT NULL REFERENCES employee_type(employee_type_id),
    full_name          VARCHAR(255) NOT NULL,
    login              VARCHAR(50) NOT NULL UNIQUE,
    status             VARCHAR(30)  NOT NULL,
    position           VARCHAR(50) NOT NULL
);

CREATE TABLE parking_zone (
    parking_zone_id    SERIAL PRIMARY KEY,
    name               VARCHAR(100) NOT NULL,
    zone_type          VARCHAR(30)  NOT NULL,
    city_district      VARCHAR(100) NOT NULL,
    max_cars           INT      NOT NULL,
    active_status      VARCHAR(30)  NOT NULL
);

CREATE TABLE parking_zone_point (
    parking_zone_point_id SERIAL PRIMARY KEY,
    parking_zone_id       INT NOT NULL REFERENCES parking_zone(parking_zone_id) ON DELETE CASCADE,
    vertex_number         INT NOT NULL,
    latitude              NUMERIC(9,6)  NOT NULL,
    longitude             NUMERIC(9,6)  NOT NULL,
    UNIQUE (parking_zone_id, vertex_number)
);

CREATE TABLE car (
    car_id             SERIAL PRIMARY KEY,
    parking_zone_id    INT REFERENCES parking_zone(parking_zone_id),
    reg_number         VARCHAR(20)  NOT NULL UNIQUE,
    brand              VARCHAR(100) NOT NULL,
    model              VARCHAR(100) NOT NULL,
    manufacture_year   INT      NOT NULL,
    vin                VARCHAR(17)  NOT NULL UNIQUE,
    current_status     VARCHAR(30)  NOT NULL
);

CREATE TABLE track (
    track_id           SERIAL PRIMARY KEY,
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE CASCADE,
    start_time         TIMESTAMP NOT NULL,
    end_time           TIMESTAMP,
    track_status       VARCHAR(30) NOT NULL,
    track_kind         VARCHAR(30) NOT NULL
);

CREATE TABLE track_point (
    track_point_id     SERIAL PRIMARY KEY,
    track_id           INT REFERENCES track(track_id) ON DELETE CASCADE, -- NULL разрешён
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE CASCADE,
    latitude           NUMERIC(9,6)  NOT NULL,
    longitude          NUMERIC(9,6)  NOT NULL,
    speed_kmh          NUMERIC(6,2),
    data_source        VARCHAR(30) NOT NULL
);

CREATE TABLE alert_event (
    alert_event_id     SERIAL PRIMARY KEY,
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE CASCADE,
    employee_id        INT REFERENCES employee(employee_id),
    event_type         VARCHAR(50) NOT NULL,
    latitude           NUMERIC(9,6),
    longitude          NUMERIC(9,6),
    description        TEXT,
    process_status     VARCHAR(30) NOT NULL
);

CREATE TABLE geo_request (
    geo_request_id     SERIAL PRIMARY KEY,
    employee_id        INT NOT NULL REFERENCES employee(employee_id),
    car_id             INT NOT NULL REFERENCES car(car_id),
    request_type       VARCHAR(50) NOT NULL,
    request_goal       TEXT NOT NULL
);

