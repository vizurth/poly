package main

import (
	"context"
	"db_generation/internal/config"
	"db_generation/internal/logger"
	"db_generation/internal/postgres"
	"time"
)

func main() {
	ctx, cancel := context.WithTimeout(context.Background(), time.Second * 10)
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

	_, err = postgres.New(ctx, cfg.Postgres)
	if err != nil {
		log.Fatal(ctx, "failed to create postgres pool")
	}

	if err := postgres.Migrate(ctx, cfg.Postgres); err != nil {
		log.Fatal(ctx, "failed to run migrations")
	}

	// Здесь можно добавить код для генерации данных в базу, если это необходимо

	log.Info(ctx, "db generation completed successfully")
}