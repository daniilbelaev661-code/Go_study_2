package handler

import (
    "net/http"

    "url-shortener/internal/service"

    "github.com/go-chi/chi/v5"
)

func RedirectHandler(svc *service.ShortenerService) http.HandlerFunc {
    return func(w http.ResponseWriter, r *http.Request) {
        code := chi.URLParam(r, "code")
        url, err := svc.GetByCode(code)
        if err != nil {
            if err.Error() == "not found" {
                http.NotFound(w, r)
            } else if err.Error() == "link expired" {
                http.Error(w, "Link expired", http.StatusGone)
            } else {
                http.Error(w, "Internal error", http.StatusInternalServerError)
            }
            return
        }
        http.Redirect(w, r, url.OriginalURL, http.StatusFound) 
    }
}