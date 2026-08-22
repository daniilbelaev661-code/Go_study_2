package handler

import (
    "encoding/json"
    "net/http"

    "url-shortener/internal/service"

    "github.com/go-chi/chi/v5"
)

func StatsHandler(svc *service.ShortenerService) http.HandlerFunc {
    return func(w http.ResponseWriter, r *http.Request) {
        code := chi.URLParam(r, "code")
        url, err := svc.GetStats(code)
        if err != nil {
            http.NotFound(w, r)
            return
        }
        w.Header().Set("Content-Type", "application/json")
        json.NewEncoder(w).Encode(url)
    }
}