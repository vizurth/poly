# Промпт для генерации данных в БД геомониторинга каршеринга

## Задача

Напиши Go программу для заполнения базы данных PostgreSQL тестовыми данными.
Общее количество записей: **300 000 – 350 000**.

## Схема базы данных

```sql
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
    zone_type_id       SERIAL PRIMARY KEY,
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
    max_cars           INT NOT NULL,
    zone_status_id     INT NOT NULL REFERENCES parking_zone_status(zone_status_id)
);

CREATE TABLE parking_zone_point (
    parking_zone_point_id SERIAL PRIMARY KEY,
    parking_zone_id       INT NOT NULL REFERENCES parking_zone(parking_zone_id) ON DELETE CASCADE,
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
    brand              VARCHAR(50) NOT NULL,
    model              VARCHAR(70) NOT NULL,
    manufacture_year   INT NOT NULL,
    vin                VARCHAR(17) NOT NULL UNIQUE,
    car_status_id      INT NOT NULL REFERENCES car_status(car_status_id)
);

CREATE TABLE parking_session (
    parking_session_id SERIAL PRIMARY KEY,
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE CASCADE,
    parking_zone_id    INT NOT NULL REFERENCES parking_zone(parking_zone_id) ON DELETE CASCADE,
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
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE CASCADE,
    start_time         TIMESTAMP NOT NULL,
    end_time           TIMESTAMP,
    track_status_id    INT NOT NULL REFERENCES track_status_type(track_status_id),
    track_kind_id      INT NOT NULL REFERENCES track_kind_type(track_kind_id)
);

CREATE TABLE track_point (
    track_point_id     SERIAL PRIMARY KEY,
    track_id           INT REFERENCES track(track_id) ON DELETE CASCADE,
    car_id             INT REFERENCES car(car_id) ON DELETE CASCADE,
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
    car_id             INT NOT NULL REFERENCES car(car_id) ON DELETE CASCADE,
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
```

## Требования к программе

### Подключение

```
host=localhost port=5432 dbname=db user=db-user password=db-password sslmode=disable
```

Использовать `github.com/jackc/pgx/v5`.

### Структура кода

Каждый уровень — отдельная функция. Запросы писать в виде:

```go
query := "INSERT INTO ... VALUES ..."
conn.Exec(ctx, query, param1, param2, ...)
```

Никаких ORM. Только `github.com/jackc/pgx/v5`.

### Порядок заполнения и количество записей

#### Уровень 1 — Справочники (фиксированные значения, вставлять именно эти)

| Таблица | Значения |
|---|---|
| employee_type | `Оператор колл-центра`, `Администратор системы`, `Сервисный специалист`, `Аналитик` |
| employee_status | `Активен`, `В отпуске`, `Уволен`, `Заблокирован` |
| car_status | `Свободен`, `В аренде`, `На обслуживании`, `Неисправен` |
| parking_zone_type | `Бесплатная`, `Платная`, `Запрещённая` |
| parking_zone_status | `Активна`, `Временно закрыта`, `Недоступна` |
| track_status_type | `Активный`, `Завершён`, `Архивный` |
| track_kind_type | `Пользовательский`, `Сервисный` |
| data_source_type | `GPS`, `ГЛОНАСС`, `GPS/ГЛОНАСС`, `Инерциальная система` |
| alert_event_type | `Потеря связи`, `Превышение скорости`, `Резкое торможение`, `Удар`, `Попытка угона`, `Выезд за границу` |
| alert_event_process_status | `Новое`, `В работе`, `Закрыто` |
| geo_request_type | `Текущие координаты`, `История событий` |

#### Уровень 2 — Основные сущности

| Таблица | Количество | Примечания |
|---|---|---|
| employee | 30 | Реалистичные русские ФИО, логин = транслит фамилии + цифры |
| car | 100 | Реальные марки и модели российского рынка, гос. номер формата `А000АА77` |
| parking_zone | 200 | Названия вида «ТЦ Галерея — северный въезд», районы Москвы |

#### Уровень 3 — Зависимые сущности

| Таблица | Количество | Примечания |
|---|---|---|
| parking_zone_point | ~1 000 | 4–6 вершин на каждую зону, координаты в районе Москвы (55.7–55.9, 37.3–37.9) |
| track | 5 000 | Каждый автомобиль имеет ~50 треков за год |
| parking_session | 5 000 | 1 сессия на каждый трек, entry_time = start_time трека |
| alert_event | 500 | ~5 событий на автомобиль |
| geo_request | 3 000 | ~100 запросов на сотрудника |

#### Уровень 4 — Самые зависимые

| Таблица | Количество | Примечания |
|---|---|---|
| track_point | ~285 000 | На каждый трек ~57 точек (поездка ~15 мин, точка каждые 15 сек). Координаты чуть смещаются от предыдущей точки, скорость 0–120 км/ч |

### Итоговое распределение

```
Справочники:   ~38
Уровень 2:      330
Уровень 3:   14 500
Уровень 4:  285 000
─────────────────────
Итого:      ~300 000
```

### Важные правила

1. Вставлять строго по уровням — сначала уровень 1, потом 2, 3, 4
2. После вставки каждого уровня делать commit транзакции
3. Уровень 4 вставлять батчами по 1000 записей
4. После каждого уровня выводить в консоль сколько записей вставлено
5. Все вставки оборачивать в обработку ошибок с выводом в консоль