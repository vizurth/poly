package config

import (
	"db_generation/internal/postgres"
	"fmt"
	"os"

	"github.com/ilyakaznacheev/cleanenv"
)


type Config struct {
	Postgres  postgres.Config `yaml:"postgres"`
}

func New() (*Config, error) {
	var cfg Config

	configPaths := []string{
		"./configs/configs.yaml",
		"./configs/config.yaml",
		"configs/configs.yaml",
		"configs/config.yaml",
		"../configs/configs.yaml",
		"../configs/config.yaml",
		"../../configs/configs.yaml",
		"../../configs/config.yaml",
	}

	var configPath string
	for _, path := range configPaths {
		if _, err := os.Stat(path); err == nil {
			configPath = path
			break
		}
	}

	if configPath == "" {
		return &Config{}, fmt.Errorf("config file not found")
	}

	if err := cleanenv.ReadConfig(configPath, &cfg); err != nil {
		return &Config{}, fmt.Errorf("error reading config: %w", err)
	}

	return &cfg, nil
}