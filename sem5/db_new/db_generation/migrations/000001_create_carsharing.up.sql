CREATE TABLE employee_type (
    employee_type_id   SERIAL PRIMARY KEY,
    name               VARCHAR(30) NOT NULL UNIQUE,
    description        TEXT
);

CREATE TABLE employee_status (
    employee_status_id SERIAL PRIMARY KEY,
    name               VARCHAR(20) NOT NULL UNIQUE
);

CREATE TABLE employee (
    employee_id        SERIAL PRIMARY KEY,
    employee_type_id   INT NOT NULL REFERENCES employee_type(employee_type_id),
    full_name          VARCHAR(100) NOT NULL,
    login              VARCHAR(30) NOT NULL UNIQUE,
    employee_status_id INT NOT NULL REFERENCES employee_status(employee_status_id)
);

CREATE TABLE parking_zone_type (
    zone_type_id        SERIAL PRIMARY KEY,
    name               VARCHAR(20) NOT NULL UNIQUE
);

CREATE TABLE parking_zone_status (
    zone_status_id     SERIAL PRIMARY KEY,
    name               VARCHAR(25) NOT NULL UNIQUE
);

CREATE TABLE parking_zone (
    parking_zone_id    SERIAL PRIMARY KEY,
    name               VARCHAR(60) NOT NULL,
    zone_type_id       INT NOT NULL REFERENCES parking_zone_type(zone_type_id),
    city_district      VARCHAR(50) NOT NULL,
    max_cars           INT      NOT NULL,
    zone_status_id     INT NOT NULL REFERENCES parking_zone_status(zone_status_id)
);

CREATE TABLE parking_zone_point (
    parking_zone_point_id SERIAL PRIMARY KEY,
    parking_zone_id       INT NOT NULL REFERENCES parking_zone(parking_zone_id) ON DELETE NO ACTION,
    vertex_number         INT NOT NULL,
    latitude              NUMERIC(8, 5) NOT NULL,
    longitude             NUMERIC(8, 5) NOT NULL,
    UNIQUE (parking_zone_id, vertex_number)
);

CREATE TABLE car_status (
    car_status_id      SERIAL PRIMARY KEY,
    name               VARCHAR(25) NOT NULL UNIQUE
);

CREATE TABLE car (
    car_id             SERIAL PRIMARY KEY,
    reg_number         VARCHAR(9)  NOT NULL UNIQUE,
    brand              VARCHAR(50)  NOT NULL,
    model              VARCHAR(70)  NOT NULL,
    manufacture_year   INT      NOT NULL,
    vin                VARCHAR(17)  NOT NULL UNIQUE,
    car_status_id      INT NOT NULL REFERENCES car_status(car_status_id)
);

CREATE TABLE parking_session (
    parking_session_id SERIAL PRIMARY KEY,
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE NO ACTION,
    parking_zone_id    INT NOT NULL REFERENCES parking_zone(parking_zone_id) ON DELETE NO ACTION,
    entry_time         TIMESTAMP NOT NULL,
    exit_time          TIMESTAMP
);

CREATE TABLE track_status_type (
    track_status_id    SERIAL PRIMARY KEY,
    name               VARCHAR(20) NOT NULL UNIQUE
);

CREATE TABLE track_kind_type (
    track_kind_id      SERIAL PRIMARY KEY,
    name               VARCHAR(25) NOT NULL UNIQUE
);

CREATE TABLE data_source_type (
    data_source_id     SERIAL PRIMARY KEY,
    name               VARCHAR(25) NOT NULL UNIQUE
);

CREATE TABLE track (
    track_id           SERIAL PRIMARY KEY,
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE NO ACTION,
    start_time         TIMESTAMP NOT NULL,
    end_time           TIMESTAMP,
    track_status_id    INT NOT NULL REFERENCES track_status_type(track_status_id),
    track_kind_id      INT NOT NULL REFERENCES track_kind_type(track_kind_id)
);

CREATE TABLE track_point (
    track_point_id     SERIAL PRIMARY KEY,
    track_id           INT REFERENCES track(track_id) ON DELETE NO ACTION,
    car_id             INT REFERENCES car(car_id) ON DELETE NO ACTION,
    latitude           NUMERIC(8, 5) NOT NULL,
    longitude          NUMERIC(8, 5) NOT NULL,
    speed_kmh          NUMERIC(5, 2),
    data_source_id     INT NOT NULL REFERENCES data_source_type(data_source_id)
);

CREATE TABLE alert_event_type (
    alert_event_type_id SERIAL PRIMARY KEY,
    name               VARCHAR(25) NOT NULL UNIQUE,
    description        TEXT
);

CREATE TABLE alert_event_process_status (
    status_id          SERIAL PRIMARY KEY,
    name               VARCHAR(20) NOT NULL UNIQUE
);

CREATE TABLE alert_event (
    alert_event_id     SERIAL PRIMARY KEY,
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE NO ACTION,
    employee_id        INT REFERENCES employee(employee_id),
    alert_event_type_id INT NOT NULL REFERENCES alert_event_type(alert_event_type_id),
    latitude           NUMERIC(8, 5),
    longitude          NUMERIC(8, 5),
    description        TEXT,
    status_id          INT NOT NULL REFERENCES alert_event_process_status(status_id)
);

CREATE TABLE geo_request_type (
    geo_request_type_id SERIAL PRIMARY KEY,
    name               VARCHAR(25) NOT NULL UNIQUE,
    description        TEXT
);

CREATE TABLE geo_request (
    geo_request_id     SERIAL PRIMARY KEY,
    employee_id        INT NOT NULL REFERENCES employee(employee_id),
    car_id             INT NOT NULL REFERENCES car(car_id),
    geo_request_type_id INT NOT NULL REFERENCES geo_request_type(geo_request_type_id),
    request_goal       TEXT NOT NULL
);