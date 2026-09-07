package main

import (
    "database/sql"
    "log"
    "net/http"

    "url-shortener/config"
    "url-shortener/internal/handler"
    "url-shortener/internal/repository"
    "url-shortener/internal/service"

    "github.com/go-chi/chi/v5"
    _ "github.com/mattn/go-sqlite3"
)

func main() {
    cfg := config.Load()

    db, err := sql.Open("sqlite3", cfg.DBPath)
    if err != nil {
        log.Fatal(err)
    }
    defer db.Close()

    // Инициализация схемы БД
    if err := repository.Migrate(db); err != nil {
        log.Fatal(err)
    }

    repo := repository.NewSQLiteURLRepository(db)
    svc := service.NewShortenerService(repo, cfg.BaseURL)

    r := chi.NewRouter()
    r.Post("/shorten", handler.ShortenHandler(svc))
    r.Get("/{code}", handler.RedirectHandler(svc))
    r.Get("/{code}/stats", handler.StatsHandler(svc))

    log.Printf("Server started at %s", cfg.ServerAddr)
    if err := http.ListenAndServe(cfg.ServerAddr, r); err != nil {
        log.Fatal(err)
    }
}