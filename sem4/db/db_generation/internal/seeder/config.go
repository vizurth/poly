package seeder

import (
	"context"
	"fmt"

	"github.com/jackc/pgx/v5/pgxpool"
)

func DefaultConfig() Config {
	return Config{
		Employees:           30,
		Cars:                200,
		ParkingZones:        200,
		ZonePointsMin:       20,
		ZonePointsMax:       30,
		TracksPerCarMin:     10,
		TracksPerCarMax:     30,
		SessionsPerCarMin:   10,
		SessionsPerCarMax:   20,
		TrackPointsMin:      70,
		TrackPointsMax:      100,
		AlertEvents:         500,
		AlertEventPerCarMin: 1,
		AlertEventPerCarMax: 3,
		GeoRequestsMin:      8,
		GeoRequestsMax:      20,
		BatchSize:           1000,
	}
}

var reportTables = []string{
	"employee_type", "employee_status",
	"car_status", "parking_zone_type", "parking_zone_status",
	"track_status_type", "track_kind_type", "data_source_type",
	"alert_event_type", "alert_event_process_status", "geo_request_type",
	"employee", "car", "parking_zone",
	"parking_zone_point", "track", "parking_session",
	"alert_event", "geo_request", "track_point",
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