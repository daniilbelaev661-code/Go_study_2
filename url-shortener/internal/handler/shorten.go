package handler

import (
	"encoding/json"
	"net/http"
	"time"

	"url-shortener/internal/service"
)

type shortenRequest struct {
	URL         string `json:"url"`
	CustomAlias string `json:"custom_alias"`
	ExpiresAt   string `json:"expires_at"`
}

type shortenResponse struct {
	ShortURL string `json:"short_url"`
}

func ShortenHandler(svc *service.ShortenerService) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		var req shortenRequest
		if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
			http.Error(w, "Invalid JSON", http.StatusBadRequest)
			return
		}

		var expiresAt *time.Time
		if req.ExpiresAt != "" {
			t, err := time.Parse(time.RFC3339, req.ExpiresAt)
			if err != nil {
				http.Error(w, "Invalid expires_at format, use RFC3339", http.StatusBadRequest)
				return
			}
			expiresAt = &t
		}

		url, err := svc.Shorten(req.URL, req.CustomAlias, expiresAt)
		if err != nil {
			http.Error(w, err.Error(), http.StatusBadRequest)
			return
		}

		resp := shortenResponse{
			ShortURL: svc.BuildShortURL(url.ShortCode),
		}
		w.Header().Set("Content-Type", "application/json")
		json.NewEncoder(w).Encode(resp)
	}
}
