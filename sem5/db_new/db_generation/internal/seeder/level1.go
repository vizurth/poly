package seeder

import (
	"context"
	"fmt"
	"math/rand"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/pgx/v5/pgxpool"
)

func seedLevel1(ctx context.Context, pool *pgxpool.Pool, _ *rand.Rand) (*level1Refs, int, error) {
	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, 0, fmt.Errorf("level1 begin: %w", err)
	}
	defer tx.Rollback(ctx)

	refs := &level1Refs{}
	n := 0

	refs.EmpTypeIDs, err = insertLookupWithDesc(ctx, tx, "employee_type", "employee_type_id", empTypeData)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.EmpTypeIDs)

	refs.EmpStatusIDs, err = insertLookupNames(ctx, tx, "employee_status", "employee_status_id", empStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.EmpStatusIDs)

	refs.CarStatusIDs, err = insertLookupNames(ctx, tx, "car_status", "car_status_id", carStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.CarStatusIDs)

	refs.ZoneTypeIDs, err = insertLookupNames(ctx, tx, "parking_zone_type", "zone_type_id", zoneTypeNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.ZoneTypeIDs)

	refs.ZoneStatusIDs, err = insertLookupNames(ctx, tx, "parking_zone_status", "zone_status_id", zoneStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.ZoneStatusIDs)

	refs.TrackStatusIDs, err = insertLookupNames(ctx, tx, "track_status_type", "track_status_id", trackStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.TrackStatusIDs)

	refs.TrackKindIDs, err = insertLookupNames(ctx, tx, "track_kind_type", "track_kind_id", trackKindNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.TrackKindIDs)

	refs.DataSourceIDs, err = insertLookupNames(ctx, tx, "data_source_type", "data_source_id", dataSourceNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.DataSourceIDs)

	refs.AlertEventTypeIDs, err = insertLookupWithDesc(ctx, tx, "alert_event_type", "alert_event_type_id", alertEventTypeData)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.AlertEventTypeIDs)

	refs.AlertProcStatusIDs, err = insertLookupNames(ctx, tx, "alert_event_process_status", "status_id", alertProcStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.AlertProcStatusIDs)

	refs.GeoReqTypeIDs, err = insertLookupWithDesc(ctx, tx, "geo_request_type", "geo_request_type_id", geoReqTypeData)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.GeoReqTypeIDs)

	if err = tx.Commit(ctx); err != nil {
		return nil, 0, fmt.Errorf("level1 commit: %w", err)
	}
	return refs, n, nil
}

func insertLookupWithDesc(ctx context.Context, tx pgx.Tx, table, idCol string, rows []struct{ name, desc string }) ([]int, error) {
	ids := make([]int, 0, len(rows))
	for _, row := range rows {
		var id int
		query := fmt.Sprintf(
			"INSERT INTO %s(name, description) VALUES ($1, $2) RETURNING %s",
			table, idCol,
		)
		if err := tx.QueryRow(ctx, query, row.name, row.desc).Scan(&id); err != nil {
			return nil, fmt.Errorf("insert %s(%q): %w", table, row.name, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

func insertLookupNames(ctx context.Context, tx pgx.Tx, table, idCol string, names []string) ([]int, error) {
	ids := make([]int, 0, len(names))
	for _, name := range names {
		var id int
		query := fmt.Sprintf(
			"INSERT INTO %s(name) VALUES ($1) RETURNING %s",
			table, idCol,
		)
		if err := tx.QueryRow(ctx, query, name).Scan(&id); err != nil {
			return nil, fmt.Errorf("insert %s(%q): %w", table, name, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}
