-- employee
CREATE INDEX IF NOT EXISTS idx_employee_employee_type_id
  	ON employee(employee_type_id);

CREATE INDEX IF NOT EXISTS idx_employee_employee_status_id
  	ON employee(employee_status_id);

-- parking_zone
CREATE INDEX IF NOT EXISTS idx_parking_zone_zone_type_id
  	ON parking_zone(zone_type_id);

CREATE INDEX IF NOT EXISTS idx_parking_zone_zone_status_id
  	ON parking_zone(zone_status_id);

-- parking_zone_point
CREATE INDEX IF NOT EXISTS idx_parking_zone_point_parking_zone_id
  	ON parking_zone_point(parking_zone_id);

-- car
CREATE INDEX IF NOT EXISTS idx_car_car_status_id
  	ON car(car_status_id);

-- parking_session
CREATE INDEX IF NOT EXISTS idx_parking_session_car_id
  	ON parking_session(car_id);

CREATE INDEX IF NOT EXISTS idx_parking_session_parking_zone_id
  	ON parking_session(parking_zone_id);

-- track
CREATE INDEX IF NOT EXISTS idx_track_car_id
  	ON track(car_id);

CREATE INDEX IF NOT EXISTS idx_track_track_status_id
  	ON track(track_status_id);

CREATE INDEX IF NOT EXISTS idx_track_track_kind_id
  	ON track(track_kind_id);

-- track_point
CREATE INDEX IF NOT EXISTS idx_track_point_track_id
  	ON track_point(track_id);

CREATE INDEX IF NOT EXISTS idx_track_point_car_id
  	ON track_point(car_id);

CREATE INDEX IF NOT EXISTS idx_track_point_data_source_id
  	ON track_point(data_source_id);

-- alert_event
CREATE INDEX IF NOT EXISTS idx_alert_event_car_id
  	ON alert_event(car_id);

CREATE INDEX IF NOT EXISTS idx_alert_event_employee_id
  	ON alert_event(employee_id);

CREATE INDEX IF NOT EXISTS idx_alert_event_alert_event_type_id
  	ON alert_event(alert_event_type_id);

CREATE INDEX IF NOT EXISTS idx_alert_event_status_id
  	ON alert_event(status_id);

-- geo_request
CREATE INDEX IF NOT EXISTS idx_geo_request_employee_id
	ON geo_request(employee_id);

CREATE INDEX IF NOT EXISTS idx_geo_request_car_id
  	ON geo_request(car_id);

CREATE INDEX IF NOT EXISTS idx_geo_request_geo_request_type_id
  	ON geo_request(geo_request_type_id);