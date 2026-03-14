package seeder

import (
	"context"
	"fmt"
	"math/rand"
	"time"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/pgx/v5/pgxpool"
)

type Config struct {
	EmployeeTypes       int
	ParkingZones        int
	Employees           int
	ZonePointsPerZone   int
	Cars                int
	TracksPerCar        int
	TrackPointsPerTrack int
	TrackPointsPerCarWithoutTracks int
	AlertEvents         int
	GeoRequests         int
}

type Report struct {
	TableCounts map[string]int
	TotalRows   int
}

type trackRef struct {
	ID    int
	CarID int
}

func DefaultConfig() Config {
	return Config{
		EmployeeTypes:       5,
		ParkingZones:        20,
		Employees:           1500,
		ZonePointsPerZone:   50,
		Cars:                5000,
		TracksPerCar:        2,
		TrackPointsPerTrack: 20,
		TrackPointsPerCarWithoutTracks: 10,
		AlertEvents:         1500,
		GeoRequests:         1200,
	}
}


// Seed заполняет БД с параметрами DefaultConfig.
func Seed(ctx context.Context, pool *pgxpool.Pool) error {
	_, err := SeedWithReport(ctx, pool, DefaultConfig())
	return err
}

// SeedWithReport заполняет БД и возвращает отчёт по числу строк в каждой таблице.
func SeedWithReport(ctx context.Context, pool *pgxpool.Pool, cfg Config) (*Report, error) {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))

	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, fmt.Errorf("begin transaction: %w", err)
	}
	defer tx.Rollback(ctx)

	if err = truncateAll(ctx, tx); err != nil {
		return nil, err
	}

	// уровень 1
	employeeTypeIDs, err := seedEmployeeTypes(ctx, tx, r, cfg.EmployeeTypes)
	if err != nil {
		return nil, err
	}

	parkingZoneIDs, err := seedParkingZones(ctx, tx, r, cfg.ParkingZones)
	if err != nil {
		return nil, err
	}

	// уровень 2
	employeeIDs, err := seedEmployees(ctx, tx, r, employeeTypeIDs, cfg.Employees)
	if err != nil {
		return nil, err
	}

	if err = seedParkingZonePoints(ctx, tx, r, parkingZoneIDs, cfg.ZonePointsPerZone); err != nil {
		return nil, err
	}

	carIDs, err := seedCars(ctx, tx, r, parkingZoneIDs, cfg.Cars)
	if err != nil {
		return nil, err
	}

	// уровень 3
	tracks, err := seedTracks(ctx, tx, r, carIDs, cfg.TracksPerCar)
	if err != nil {
		return nil, err
	}

	if err = seedAlertEvents(ctx, tx, r, carIDs, employeeIDs, cfg.AlertEvents); err != nil {
		return nil, err
	}

	if err = seedGeoRequests(ctx, tx, r, employeeIDs, carIDs, cfg.GeoRequests); err != nil {
		return nil, err
	}

	// уровень 4
	if err = seedTrackPoints(ctx, tx, r, tracks, cfg.TrackPointsPerTrack, cfg.TrackPointsPerCarWithoutTracks); err != nil {
		return nil, err
	}

	if err = tx.Commit(ctx); err != nil {
		return nil, fmt.Errorf("commit transaction: %w", err)
	}

	return buildReport(ctx, pool)
}

// удаляет все данные из таблиц
func truncateAll(ctx context.Context, tx pgx.Tx) error {
	_, err := tx.Exec(ctx, `
		TRUNCATE TABLE
			geo_request,
			alert_event,
			track_point,
			track,
			car,
			parking_zone_point,
			employee,
			parking_zone,
			employee_type
		RESTART IDENTITY CASCADE
	`)
	if err != nil {
		return fmt.Errorf("truncate tables: %w", err)
	}
	return nil
}

// seedEmployeeTypes заполняет таблицу employee_type и возвращает слайс сгенерированных ID
func seedEmployeeTypes(ctx context.Context, tx pgx.Tx, r *rand.Rand, count int) ([]int, error) {
	names := []string{"operator", "dispatcher", "manager", "technician", "analyst"}
	ids := make([]int, 0, count)

	for i := 0; i < count; i++ {
		var id int
		err := tx.QueryRow(ctx,
			`INSERT INTO employee_type(name, description)
			 VALUES ($1, $2)
			 RETURNING employee_type_id`,
			fmt.Sprintf("%s_%d", names[i%len(names)], i+1),
			fmt.Sprintf("generated employee type %d", i+1),
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert employee_type[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

// seedParkingZones заполняет таблицу parking_zone и возвращает слайс сгенерированных ID
func seedParkingZones(ctx context.Context, tx pgx.Tx, r *rand.Rand, count int) ([]int, error) {
	zoneTypes := []string{"city", "suburban", "hub"}
	zoneStatuses := []string{"active", "maintenance", "limited"}
	ids := make([]int, 0, count)

	for i := 0; i < count; i++ {
		var id int
		err := tx.QueryRow(ctx,
			`INSERT INTO parking_zone(name, zone_type, city_district, max_cars, active_status)
			 VALUES ($1, $2, $3, $4, $5)
			 RETURNING parking_zone_id`,
			fmt.Sprintf("Zone-%03d", i+1),
			zoneTypes[r.Intn(len(zoneTypes))],
			fmt.Sprintf("district-%d", 1+r.Intn(8)),
			20+r.Intn(150),
			zoneStatuses[r.Intn(len(zoneStatuses))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert parking_zone[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

// seedEmployees заполняет таблицу employee и возвращает слайс сгенерированных ID
func seedEmployees(ctx context.Context, tx pgx.Tx, r *rand.Rand, employeeTypeIDs []int, count int) ([]int, error) {
	statuses := []string{"active", "vacation", "inactive"}
	positions := []string{"junior", "middle", "senior", "lead"}
	ids := make([]int, 0, count)

	for i := 0; i < count; i++ {
		var id int
		err := tx.QueryRow(ctx,
			`INSERT INTO employee(employee_type_id, full_name, login, status, position)
			 VALUES ($1, $2, $3, $4, $5)
			 RETURNING employee_id`,
			employeeTypeIDs[r.Intn(len(employeeTypeIDs))],
			fmt.Sprintf("Employee %04d", i+1),
			fmt.Sprintf("user_%04d", i+1),
			statuses[r.Intn(len(statuses))],
			positions[r.Intn(len(positions))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert employee[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

// seedParkingZonePoints заполняет таблицу parking_zone_point для каждой зоны по pointsPerZone точек
func seedParkingZonePoints(ctx context.Context, tx pgx.Tx, r *rand.Rand, parkingZoneIDs []int, pointsPerZone int) error {
	for _, zoneID := range parkingZoneIDs {
		for pointNo := 1; pointNo <= pointsPerZone; pointNo++ {
			_, err := tx.Exec(ctx,
				`INSERT INTO parking_zone_point(parking_zone_id, vertex_number, latitude, longitude)
				 VALUES ($1, $2, $3, $4)`,
				zoneID,
				pointNo,
				59.85+r.Float64()*0.2,
				30.15+r.Float64()*0.35,
			)
			if err != nil {
				return fmt.Errorf("insert parking_zone_point(zone=%d, vertex=%d): %w", zoneID, pointNo, err)
			}
		}
	}
	return nil
}

// seedCars заполняет таблицу car и возвращает слайс сгенерированных ID
func seedCars(ctx context.Context, tx pgx.Tx, r *rand.Rand, parkingZoneIDs []int, count int) ([]int, error) {
	brands := []string{"VW", "Ford", "Hyundai", "Kia", "Lada", "Toyota"}
	models := []string{"A", "B", "C", "D", "E"}
	carStatuses := []string{"available", "busy", "maintenance", "offline"}
	ids := make([]int, 0, count)

	for i := 0; i < count; i++ {
		var id int
		err := tx.QueryRow(ctx,
			`INSERT INTO car(parking_zone_id, reg_number, brand, model, manufacture_year, vin, current_status)
			 VALUES ($1, $2, $3, $4, $5, $6, $7)
			 RETURNING car_id`,
			parkingZoneIDs[r.Intn(len(parkingZoneIDs))],
			fmt.Sprintf("A%03dAA%03d", i%1000, 100+(i%799)),
			brands[r.Intn(len(brands))],
			models[r.Intn(len(models))],
			2012+r.Intn(14),
			fmt.Sprintf("VIN%014d", i+1),
			carStatuses[r.Intn(len(carStatuses))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert car[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

// seedTracks заполняет таблицу track для каждого автомобиля по tracksPerCar треков и возвращает слайс с их ID и ID автомобиля
func seedTracks(ctx context.Context, tx pgx.Tx, r *rand.Rand, carIDs []int, tracksPerCar int) ([]trackRef, error) {
	trackStatuses := []string{"finished", "active", "interrupted"}
	trackKinds := []string{"service", "relocation", "emergency"}
	refs := make([]trackRef, 0, len(carIDs)*tracksPerCar)

	for _, carID := range carIDs {
		for j := 0; j < tracksPerCar; j++ {
			startAt := time.Now().Add(-time.Duration(r.Intn(720)) * time.Hour)
			endAt := startAt.Add(time.Duration(15+r.Intn(180)) * time.Minute)
			var id int
			err := tx.QueryRow(ctx,
				`INSERT INTO track(car_id, start_time, end_time, track_status, track_kind)
				 VALUES ($1, $2, $3, $4, $5)
				 RETURNING track_id`,
				carID,
				startAt,
				endAt,
				trackStatuses[r.Intn(len(trackStatuses))],
				trackKinds[r.Intn(len(trackKinds))],
			).Scan(&id)
			if err != nil {
				return nil, fmt.Errorf("insert track(car=%d, j=%d): %w", carID, j, err)
			}
			refs = append(refs, trackRef{ID: id, CarID: carID})
		}
	}
	return refs, nil
}

// seedAlertEvents заполняет таблицу alert_event заданным числом строк, используя случайные car_id и employee_id из переданных слайсов
func seedAlertEvents(ctx context.Context, tx pgx.Tx, r *rand.Rand, carIDs, employeeIDs []int, count int) error {
	eventTypes := []string{"speeding", "zone_leave", "panic_button", "signal_lost"}
	processStatuses := []string{"new", "in_progress", "closed"}

	for i := 0; i < count; i++ {
		_, err := tx.Exec(ctx,
			`INSERT INTO alert_event(car_id, employee_id, event_type, latitude, longitude, description, process_status)
			 VALUES ($1, $2, $3, $4, $5, $6, $7)`,
			carIDs[r.Intn(len(carIDs))],
			employeeIDs[r.Intn(len(employeeIDs))],
			eventTypes[r.Intn(len(eventTypes))],
			59.85+r.Float64()*0.2,
			30.15+r.Float64()*0.35,
			fmt.Sprintf("generated alert event %d", i+1),
			processStatuses[r.Intn(len(processStatuses))],
		)
		if err != nil {
			return fmt.Errorf("insert alert_event[%d]: %w", i, err)
		}
	}
	return nil
}

// seedGeoRequests заполняет таблицу geo_request заданным числом строк, используя случайные employee_id и car_id из переданных слайсов
func seedGeoRequests(ctx context.Context, tx pgx.Tx, r *rand.Rand, employeeIDs, carIDs []int, count int) error {
	requestTypes := []string{"tracking", "history", "inspection", "incident_review"}

	for i := 0; i < count; i++ {
		_, err := tx.Exec(ctx,
			`INSERT INTO geo_request(employee_id, car_id, request_type, request_goal)
			 VALUES ($1, $2, $3, $4)`,
			employeeIDs[r.Intn(len(employeeIDs))],
			carIDs[r.Intn(len(carIDs))],
			requestTypes[r.Intn(len(requestTypes))],
			fmt.Sprintf("generated request goal %d", i+1),
		)
		if err != nil {
			return fmt.Errorf("insert geo_request[%d]: %w", i, err)
		}
	}
	return nil
}

// seedTrackPoints заполняет таблицу track_point для каждого трека по pointsPerTrack точек, используя координаты в окрестности Санкт-Петербурга и случайные значения скорости и источника данных
func seedTrackPoints(
	ctx context.Context,
	tx pgx.Tx,
	r *rand.Rand,
	tracks []trackRef,
	pointsPerTrack int,
	pointsPerCarWithoutTracks int,
) error {
	dataSources := []string{"gps", "glonass", "manual"}

	for _, tr := range tracks {
		for p := 0; p < pointsPerTrack; p++ {
			_, err := tx.Exec(ctx,
				`INSERT INTO track_point(track_id, car_id, latitude, longitude, speed_kmh, data_source)
				 VALUES ($1, $2, $3, $4, $5, $6)`,
				tr.ID,
				tr.CarID,
				59.85+r.Float64()*0.2,
				30.15+r.Float64()*0.35,
				float64(10+r.Intn(110)),
				dataSources[r.Intn(len(dataSources))],
			)
			if err != nil {
				return fmt.Errorf("insert track_point(track=%d, p=%d): %w", tr.ID, p, err)
			}
		}

		for p := 0; p < pointsPerCarWithoutTracks; p++ {
			_, err := tx.Exec(ctx,
				`INSERT INTO track_point(track_id, car_id, latitude, longitude, speed_kmh, data_source)
				 VALUES ($1, $2, $3, $4, $5, $6)`,
				nil,
				tr.CarID,
				59.85+r.Float64()*0.2,
				30.15+r.Float64()*0.35,
				float64(10+r.Intn(110)),
				dataSources[r.Intn(len(dataSources))],
			)
			if err != nil {
				return fmt.Errorf("insert track_point(car=%d, p=%d) without track: %w", tr.CarID, p, err)
			}
		}
	}

	return nil
}


var reportTables = []string{
	"employee_type",
	"parking_zone",
	"employee",
	"parking_zone_point",
	"car",
	"track",
	"alert_event",
	"geo_request",
	"track_point",
}

func buildReport(ctx context.Context, pool *pgxpool.Pool) (*Report, error) {
	report := &Report{TableCounts: make(map[string]int, len(reportTables))}

	for _, table := range reportTables {
		var count int
		if err := pool.QueryRow(ctx, fmt.Sprintf("SELECT COUNT(*) FROM %s", table)).Scan(&count); err != nil {
			return nil, fmt.Errorf("count %s: %w", table, err)
		}
		report.TableCounts[table] = count
		report.TotalRows += count
	}

	return report, nil
}
