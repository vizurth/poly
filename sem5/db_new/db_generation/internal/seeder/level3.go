package seeder

import (
	"context"
	"fmt"
	"math/rand"
	"time"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/pgx/v5/pgxpool"
)

func seedLevel3(ctx context.Context, pool *pgxpool.Pool, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) ([]trackRef, int, error) {
	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, 0, fmt.Errorf("level3 begin: %w", err)
	}
	defer tx.Rollback(ctx)

	n := 0

	pts, err := seedParkingZonePoints(ctx, tx, r, cfg, l2.ParkingZoneIDs)
	if err != nil {
		return nil, 0, err
	}
	n += pts
	fmt.Printf("  parking_zone_points: %d\n", pts)

	tracks, err := seedTracks(ctx, tx, r, cfg, l1, l2)
	if err != nil {
		return nil, 0, err
	}
	n += len(tracks)
	fmt.Printf("  tracks: %d\n", len(tracks))

	sessions, err := seedParkingSessions(ctx, tx, r, cfg, l2)
	if err != nil {
		return nil, 0, err
	}
	n += sessions
	fmt.Printf("  parking_sessions: %d\n", sessions)

	alerts, err := seedAlertEvents(ctx, tx, r, cfg, l1, l2)
	if err != nil {
		return nil, 0, err
	}
	n += alerts
	fmt.Printf("  alert_events: %d\n", alerts)

	geoReqs, err := seedGeoRequests(ctx, tx, r, cfg, l1, l2)
	if err != nil {
		return nil, 0, err
	}
	n += geoReqs
	fmt.Printf("  geo_requests: %d\n", geoReqs)

	if err = tx.Commit(ctx); err != nil {
		return nil, 0, fmt.Errorf("level3 commit: %w", err)
	}
	return tracks, n, nil
}

func seedParkingZonePoints(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, zoneIDs []int) (int, error) {
	total := 0
	for _, zoneID := range zoneIDs {
		count := cfg.ZonePointsMin + r.Intn(cfg.ZonePointsMax-cfg.ZonePointsMin+1)
		for v := 1; v <= count; v++ {
			lat := 55.7 + r.Float64()*0.2
			lon := 37.3 + r.Float64()*0.6
			query := "INSERT INTO parking_zone_point(parking_zone_id, vertex_number, latitude, longitude) VALUES ($1, $2, $3, $4)"
			if _, err := tx.Exec(ctx, query, zoneID, v, lat, lon); err != nil {
				return 0, fmt.Errorf("insert parking_zone_point(zone=%d, v=%d): %w", zoneID, v, err)
			}
			total++
		}
	}
	return total, nil
}

func seedTracks(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) ([]trackRef, error) {
	refs := make([]trackRef, 0, len(l2.CarIDs)*cfg.TracksPerCarMax)
	now := time.Now()

	for _, carID := range l2.CarIDs {
		trackCount := cfg.TracksPerCarMin + r.Intn(cfg.TracksPerCarMax-cfg.TracksPerCarMin+1)
		for j := 0; j < trackCount; j++ {
			startTime := now.Add(-time.Duration(r.Intn(365*24)) * time.Hour)
			duration := time.Duration(10+r.Intn(50)) * time.Minute
			endTime := startTime.Add(duration)

			statusID := l1.TrackStatusIDs[r.Intn(len(l1.TrackStatusIDs))]
			var endPtr *time.Time
			if statusID != l1.TrackStatusIDs[0] {
				endPtr = &endTime
			}

			var id int
			query := "INSERT INTO track(car_id, start_time, end_time, track_status_id, track_kind_id) VALUES ($1, $2, $3, $4, $5) RETURNING track_id"
			err := tx.QueryRow(ctx, query,
				carID,
				startTime,
				endPtr,
				statusID,
				l1.TrackKindIDs[r.Intn(len(l1.TrackKindIDs))],
			).Scan(&id)
			if err != nil {
				return nil, fmt.Errorf("insert track(car=%d, j=%d): %w", carID, j, err)
			}
			refs = append(refs, trackRef{
				ID:        id,
				CarID:     carID,
				ZoneID:    l2.ParkingZoneIDs[r.Intn(len(l2.ParkingZoneIDs))],
				StartTime: startTime,
				EndTime:   endPtr,
			})
		}
	}
	return refs, nil
}

func seedParkingSessions(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l2 *level2Refs) (int, error) {
	total := 0
	now := time.Now()

	for _, carID := range l2.CarIDs {
		sessionCount := cfg.SessionsPerCarMin + r.Intn(cfg.SessionsPerCarMax-cfg.SessionsPerCarMin+1)
		for i := 0; i < sessionCount; i++ {
			entryTime := now.Add(-time.Duration(r.Intn(365*24)) * time.Hour)
			exitTime := entryTime.Add(time.Duration(30+r.Intn(480)) * time.Minute)
			zoneID := l2.ParkingZoneIDs[r.Intn(len(l2.ParkingZoneIDs))]

			query := "INSERT INTO parking_session(car_id, parking_zone_id, entry_time, exit_time) VALUES ($1, $2, $3, $4)"
			if _, err := tx.Exec(ctx, query, carID, zoneID, entryTime, exitTime); err != nil {
				return 0, fmt.Errorf("insert parking_session(car=%d, idx=%d): %w", carID, i, err)
			}
			total++
		}
	}

	return total, nil
}

func seedAlertEvents(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) (int, error) {
	if len(l2.CarIDs) == 0 {
		return 0, fmt.Errorf("alert_event: no cars")
	}
	if len(l2.EmployeeIDs) == 0 {
		return 0, fmt.Errorf("alert_event: no employees")
	}
	if len(l1.AlertEventTypeIDs) == 0 {
		return 0, fmt.Errorf("alert_event: no alert event types")
	}
	if len(l1.AlertProcStatusIDs) == 0 {
		return 0, fmt.Errorf("alert_event: no alert process statuses")
	}

	inserted := 0
	query := "INSERT INTO alert_event(car_id, employee_id, alert_event_type_id, latitude, longitude, description, status_id) VALUES ($1, $2, $3, $4, $5, $6, $7)"

	if cfg.AlertEventPerCarMin > 0 || cfg.AlertEventPerCarMax > 0 {
		for _, carID := range l2.CarIDs {
			count := cfg.AlertEventPerCarMin + r.Intn(cfg.AlertEventPerCarMax-cfg.AlertEventPerCarMin+1)
			for j := 0; j < count; j++ {
				lat := 59.899901 + r.Float64()*0.2
				lon := 30.265473 + r.Float64()*0.6
				inserted++
				_, err := tx.Exec(ctx, query,
					carID,
					l2.EmployeeIDs[r.Intn(len(l2.EmployeeIDs))],
					l1.AlertEventTypeIDs[r.Intn(len(l1.AlertEventTypeIDs))],
					lat,
					lon,
					fmt.Sprintf("Событие #%d", inserted),
					l1.AlertProcStatusIDs[r.Intn(len(l1.AlertProcStatusIDs))],
				)
				if err != nil {
					return 0, fmt.Errorf("insert alert_event[%d]: %w", inserted-1, err)
				}
			}
		}
		return inserted, nil
	}

	total := cfg.AlertEvents
	if total <= 0 {
		return 0, nil
	}

	base := total / len(l2.CarIDs)
	rem := total % len(l2.CarIDs)
	perm := r.Perm(len(l2.CarIDs))
	for i, carIdx := range perm {
		carID := l2.CarIDs[carIdx]
		count := base
		if i < rem {
			count++
		}
		for j := 0; j < count; j++ {
			lat := 59.899901 + r.Float64()*0.2
			lon := 30.265473 + r.Float64()*0.6
			inserted++
			_, err := tx.Exec(ctx, query,
				carID,
				l2.EmployeeIDs[r.Intn(len(l2.EmployeeIDs))],
				l1.AlertEventTypeIDs[r.Intn(len(l1.AlertEventTypeIDs))],
				lat,
				lon,
				fmt.Sprintf("Событие #%d", inserted),
				l1.AlertProcStatusIDs[r.Intn(len(l1.AlertProcStatusIDs))],
			)
			if err != nil {
				return 0, fmt.Errorf("insert alert_event[%d]: %w", inserted-1, err)
			}
		}
	}
	return inserted, nil
}

func seedGeoRequests(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) (int, error) {
	total := 0
	for _, employeeID := range l2.EmployeeIDs {
		requestCount := cfg.GeoRequestsMin + r.Intn(cfg.GeoRequestsMax-cfg.GeoRequestsMin+1)
		for i := 0; i < requestCount; i++ {
			query := "INSERT INTO geo_request(employee_id, car_id, geo_request_type_id, request_goal) VALUES ($1, $2, $3, $4)"
			_, err := tx.Exec(ctx, query,
				employeeID,
				l2.CarIDs[r.Intn(len(l2.CarIDs))],
				l1.GeoReqTypeIDs[r.Intn(len(l1.GeoReqTypeIDs))],
				fmt.Sprintf("Запрос сотрудника %d #%d", employeeID, i+1),
			)
			if err != nil {
				return 0, fmt.Errorf("insert geo_request(employee=%d, idx=%d): %w", employeeID, i, err)
			}
			total++
		}
	}
	return total, nil
}
