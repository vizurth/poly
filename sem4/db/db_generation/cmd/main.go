package main

import (
	"context"
	"db_generation/internal/config"
	"db_generation/internal/logger"
	"db_generation/internal/postgres"
	"db_generation/internal/seeder"
	"sort"
	"time"

	"go.uber.org/zap"
)

func main() {
	ctx, cancel := context.WithTimeout(context.Background(), 20*time.Minute)
	defer cancel()

	ctx, err := logger.New(ctx)
	if err != nil {
		panic("failed to create logger")
	}

	log, err := logger.GetLoggerFromCtx(ctx)
	if err != nil {
		panic("failed to create logger")
	}

	cfg, err := config.New()
	if err != nil {
		log.Fatal(ctx, "failed to parse config")
	}

	pool, err := postgres.New(ctx, cfg.Postgres)
	if err != nil {
		log.Fatal(ctx, "failed to create postgres pool")
	}
	defer pool.Close()

	if err := postgres.Migrate(ctx, cfg.Postgres); err != nil {
		log.Fatal(ctx, "failed to run migrations")
	}

	report, err := seeder.SeedWithReport(ctx, pool, seeder.DefaultConfig())
	if err != nil {
		log.Fatal(ctx, "failed to seed database", zap.Error(err))
	}

	tables := make([]string, 0, len(report.TableCounts))
	for table := range report.TableCounts {
		tables = append(tables, table)
	}
	sort.Strings(tables)

	for _, table := range tables {
		log.Info(ctx, "seed report", zap.String("table", table), zap.Int("count", report.TableCounts[table]))
	}
	log.Info(ctx, "seed report total", zap.Int("total_rows", report.TotalRows))

	log.Info(ctx, "db generation completed successfully")
}