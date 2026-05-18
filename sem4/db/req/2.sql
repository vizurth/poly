-- Запрос 2
-- Для сотрудника "Иванов Игорь Николаевич", который обрабатывал тревожные события,
-- посчитать число автомобилей, которые парковались в зоне "ТЦ Мега — парковка В"
SELECT COUNT(DISTINCT ps.car_id) AS car_count
FROM parking_session ps
JOIN parking_zone pz ON pz.parking_zone_id = ps.parking_zone_id
WHERE pz.name = 'ТЦ Мега — парковка В'
  AND ps.car_id IN (
      SELECT ae.car_id
      FROM alert_event ae
      JOIN employee e ON e.employee_id = ae.employee_id
      WHERE e.full_name = 'Иванов Игорь Николаевич'
  );
