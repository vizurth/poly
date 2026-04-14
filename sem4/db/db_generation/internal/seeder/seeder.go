package seeder

import (
	"context"
	"fmt"
	"math/rand"
	"time"

	"github.com/jackc/pgx/v5/pgxpool"
)

func Seed(ctx context.Context, pool *pgxpool.Pool) error {
	_, err := SeedWithReport(ctx, pool, DefaultConfig())
	return err
}

func SeedWithReport(ctx context.Context, pool *pgxpool.Pool, cfg Config) (*Report, error) {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))

	if err := truncateAll(ctx, pool); err != nil {
		return nil, err
	}

	fmt.Println("=== Уровень 1: справочники ===")
	l1, n1, err := seedLevel1(ctx, pool, r)
	if err != nil {
		return nil, fmt.Errorf("level1: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n1)

	fmt.Println("=== Уровень 2: основные сущности ===")
	l2, n2, err := seedLevel2(ctx, pool, r, cfg, l1)
	if err != nil {
		return nil, fmt.Errorf("level2: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n2)

	fmt.Println("=== Уровень 3: зависимые сущности ===")
	tracks, n3, err := seedLevel3(ctx, pool, r, cfg, l1, l2)
	if err != nil {
		return nil, fmt.Errorf("level3: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n3)

	fmt.Println("=== Уровень 4: точки треков ===")
	n4, err := seedLevel4(ctx, pool, r, cfg, tracks, l1.DataSourceIDs)
	if err != nil {
		return nil, fmt.Errorf("level4: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n4)

	fmt.Printf("=== Итого вставлено: %d ===\n", n1+n2+n3+n4)
	return buildReport(ctx, pool)
}

func truncateAll(ctx context.Context, pool *pgxpool.Pool) error {
	query := `TRUNCATE TABLE
		geo_request,
		alert_event,
		track_point,
		parking_session,
		track,
		car,
		parking_zone_point,
		parking_zone,
		employee,
		employee_type,
		employee_status,
		car_status,
		parking_zone_type,
		parking_zone_status,
		track_status_type,
		track_kind_type,
		data_source_type,
		alert_event_type,
		alert_event_process_status,
		geo_request_type
	RESTART IDENTITY CASCADE`

	if _, err := pool.Exec(ctx, query); err != nil {
		return fmt.Errorf("truncate tables: %w", err)
	}
	return nil
}