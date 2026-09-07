package config

import "os"

type Config struct {
    ServerAddr string
    DBPath     string
    BaseURL    string // базовый URL для формирования коротких ссылок
}

func Load() Config {
    return Config{
        ServerAddr: getEnv("SERVER_ADDR", ":8080"),
        DBPath:     getEnv("DB_PATH", "./shortener.db"),
        BaseURL:    getEnv("BASE_URL", "http://localhost:8080"),
    }
}

func getEnv(key, defaultValue string) string {
    if value, exists := os.LookupEnv(key); exists {
        return value
    }
    return defaultValue
}