CREATE USER user_a WITH PASSWORD '123';
CREATE USER user_b WITH PASSWORD '123';

GRANT CONNECT ON DATABASE db TO user_a, user_b;
GRANT USAGE ON SCHEMA public TO user_a, user_b;

-- пользователь А читает только таблицу car_stats
GRANT SELECT ON car_stats TO user_a;

-- пользователь Б читает и изменяет исходные таблицы, но не имеет прямого доступа к car_stats
GRANT SELECT, INSERT, UPDATE, DELETE 
ON TABLE car, alert_event, parking_session, parking_zone
TO user_b;

-- для пользователя Б разрешаем автоинкрементные id
GRANT USAGE, SELECT ON SEQUENCE
	car_car_id_seq, 
	alert_event_alert_event_id_seq, 
	parking_session_parking_session_id_seq, 
	parking_zone_parking_zone_id_seq
TO user_b;

ALTER FUNCTION car_stats_change() SECURITY DEFINER  SET search_path = public;

ALTER FUNCTION alert_event_stats_change() SECURITY DEFINER SET search_path = public;

ALTER FUNCTION parking_session_stats_delete() SECURITY DEFINER SET search_path = public;