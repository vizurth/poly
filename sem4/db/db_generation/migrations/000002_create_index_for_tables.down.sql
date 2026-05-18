DROP INDEX IF EXISTS idx_employee_employee_type_id;
DROP INDEX IF EXISTS idx_employee_employee_status_id;

DROP INDEX IF EXISTS idx_parking_zone_zone_type_id;
DROP INDEX IF EXISTS idx_parking_zone_zone_status_id;

DROP INDEX IF EXISTS idx_parking_zone_point_parking_zone_id;

DROP INDEX IF EXISTS idx_car_car_status_id;

DROP INDEX IF EXISTS idx_parking_session_car_id;
DROP INDEX IF EXISTS idx_parking_session_parking_zone_id;

DROP INDEX IF EXISTS idx_track_car_id;
DROP INDEX IF EXISTS idx_track_track_status_id;
DROP INDEX IF EXISTS idx_track_track_kind_id;

DROP INDEX IF EXISTS idx_track_point_track_id;
DROP INDEX IF EXISTS idx_track_point_car_id;
DROP INDEX IF EXISTS idx_track_point_data_source_id;

DROP INDEX IF EXISTS idx_alert_event_car_id;
DROP INDEX IF EXISTS idx_alert_event_employee_id;
DROP INDEX IF EXISTS idx_alert_event_alert_event_type_id;
DROP INDEX IF EXISTS idx_alert_event_status_id;

DROP INDEX IF EXISTS idx_geo_request_employee_id;
DROP INDEX IF EXISTS idx_geo_request_car_id;
DROP INDEX IF EXISTS idx_geo_request_geo_request_type_id;