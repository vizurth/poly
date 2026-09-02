package seeder

import (
	"context"
	"fmt"
	"math/rand"
	"time"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/pgx/v5/pgxpool"
)

func seedLevel2(ctx context.Context, pool *pgxpool.Pool, r *rand.Rand, cfg Config, l1 *level1Refs) (*level2Refs, int, error) {
	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, 0, fmt.Errorf("level2 begin: %w", err)
	}
	defer tx.Rollback(ctx)

	refs := &level2Refs{}
	n := 0

	refs.EmployeeIDs, err = seedEmployees(ctx, tx, r, cfg, l1)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.EmployeeIDs)
	fmt.Printf("  employees: %d\n", len(refs.EmployeeIDs))

	refs.CarIDs, err = seedCars(ctx, tx, r, cfg, l1)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.CarIDs)
	fmt.Printf("  cars: %d\n", len(refs.CarIDs))

	refs.ParkingZoneIDs, err = seedParkingZones(ctx, tx, r, cfg, l1)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.ParkingZoneIDs)
	fmt.Printf("  parking_zones: %d\n", len(refs.ParkingZoneIDs))

	if err = tx.Commit(ctx); err != nil {
		return nil, 0, fmt.Errorf("level2 commit: %w", err)
	}
	return refs, n, nil
}

func seedEmployees(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs) ([]int, error) {
	ids := make([]int, 0, cfg.Employees)
	for i := 0; i < cfg.Employees; i++ {
		surname := surnames[i%len(surnames)]
		firstName := firstNamesMale[r.Intn(len(firstNamesMale))]
		patronymic := patronymicsMale[r.Intn(len(patronymicsMale))]
		fullName := fmt.Sprintf("%s %s %s", surname, firstName, patronymic)
		login := fmt.Sprintf("%s%02d", transliterate(surname), i+1)

		var id int
		query := "INSERT INTO employee(employee_type_id, full_name, login, employee_status_id) VALUES ($1, $2, $3, $4) RETURNING employee_id"
		err := tx.QueryRow(ctx, query,
			l1.EmpTypeIDs[r.Intn(len(l1.EmpTypeIDs))],
			fullName,
			login,
			l1.EmpStatusIDs[r.Intn(len(l1.EmpStatusIDs))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert employee[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

func seedCars(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs) ([]int, error) {
	ids := make([]int, 0, cfg.Cars)
	currentYear := time.Now().Year()
	minYear := currentYear - 15
	for i := 0; i < cfg.Cars; i++ {
		brand := carData[r.Intn(len(carData))]
		model := brand.models[r.Intn(len(brand.models))]
		regNum := genRegNumber(i)
		vin := fmt.Sprintf("X%016d", i+1)
		year := minYear + r.Intn(currentYear-minYear+1)

		var id int
		query := "INSERT INTO car(reg_number, brand, model, manufacture_year, vin, car_status_id) VALUES ($1, $2, $3, $4, $5, $6) RETURNING car_id"
		err := tx.QueryRow(ctx, query,
			regNum,
			brand.brand,
			model,
			year,
			vin,
			l1.CarStatusIDs[r.Intn(len(l1.CarStatusIDs))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert car[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

func seedParkingZones(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs) ([]int, error) {
	ids := make([]int, 0, cfg.ParkingZones)
	for i := 0; i < cfg.ParkingZones; i++ {
		prefix := zoneNamePrefixes[r.Intn(len(zoneNamePrefixes))]
		suffix := zoneNameSuffixes[r.Intn(len(zoneNameSuffixes))]
		name := fmt.Sprintf("%s — %s", prefix, suffix)
		if len([]rune(name)) > 60 {
			name = string([]rune(name)[:60])
		}
		district := moscowDistricts[r.Intn(len(moscowDistricts))]

		var id int
		query := "INSERT INTO parking_zone(name, zone_type_id, city_district, max_cars, zone_status_id) VALUES ($1, $2, $3, $4, $5) RETURNING parking_zone_id"
		err := tx.QueryRow(ctx, query,
			name,
			l1.ZoneTypeIDs[r.Intn(len(l1.ZoneTypeIDs))],
			district,
			20+r.Intn(80),
			l1.ZoneStatusIDs[r.Intn(len(l1.ZoneStatusIDs))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert parking_zone[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}
