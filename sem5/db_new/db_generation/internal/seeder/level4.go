package seeder

import (
	"context"
	"fmt"
	"math/rand"

	"github.com/jackc/pgx/v5/pgxpool"
)

func seedLevel4(ctx context.Context, pool *pgxpool.Pool, r *rand.Rand, cfg Config, tracks []trackRef, dataSourceIDs []int) (int, error) {
	type row struct {
		trackID      int
		carID        int
		lat, lon     float64
		speed        float64
		dataSourceID int
	}

	batch := make([]row, 0, cfg.BatchSize)
	total := 0

	flush := func() error {
		if len(batch) == 0 {
			return nil
		}
		tx, err := pool.Begin(ctx)
		if err != nil {
			return fmt.Errorf("level4 begin: %w", err)
		}
		defer tx.Rollback(ctx)

		for _, b := range batch {
			query := "INSERT INTO track_point(track_id, car_id, latitude, longitude, speed_kmh, data_source_id) VALUES ($1, $2, $3, $4, $5, $6)"
			if _, err := tx.Exec(ctx, query, b.trackID, b.carID, b.lat, b.lon, b.speed, b.dataSourceID); err != nil {
				return fmt.Errorf("insert track_point: %w", err)
			}
		}
		if err := tx.Commit(ctx); err != nil {
			return fmt.Errorf("level4 commit: %w", err)
		}
		total += len(batch)
		batch = batch[:0]
		return nil
	}

	for _, tr := range tracks {
		lat := 59.790891 + r.Float64()*0.2
		lon := 30.324453 + r.Float64()*0.6
		pointCount := cfg.TrackPointsMin + r.Intn(cfg.TrackPointsMax-cfg.TrackPointsMin+1)
		for p := 0; p < pointCount; p++ {
			lat += (r.Float64() - 0.5) * 0.002
			lon += (r.Float64() - 0.5) * 0.003
			if lat < 59.8 {
				lat = 59.8
			}
			if lat > 60.0 {
				lat = 60.0
			}
			if lon < 30.0 {
				lon = 30.0
			}
			if lon > 30.7 {
				lon = 30.7
			}

			batch = append(batch, row{
				trackID:      tr.ID,
				carID:        tr.CarID,
				lat:          lat,
				lon:          lon,
				speed:        float64(r.Intn(121)),
				dataSourceID: dataSourceIDs[r.Intn(len(dataSourceIDs))],
			})

			if len(batch) >= cfg.BatchSize {
				if err := flush(); err != nil {
					return 0, err
				}
			}
		}
	}
	if err := flush(); err != nil {
		return 0, err
	}
	return total, nil
}
