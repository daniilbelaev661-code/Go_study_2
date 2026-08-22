package service

import (
	"errors"
	"math/rand"
	"net/url"
	"strings"
	"time"

	"url-shortener/internal/model"
	"url-shortener/internal/repository"
)

const charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

type ShortenerService struct {
	repo    repository.URLRepository
	baseURL string
}

func NewShortenerService(repo repository.URLRepository, baseURL string) *ShortenerService {
	return &ShortenerService{repo: repo, baseURL: baseURL}
}

// Shorten создаёт короткую ссылку
func (s *ShortenerService) Shorten(originalURL, customAlias string, expiresAt *time.Time) (*model.URL, error) {
	// Валидация URL
	if !isValidURL(originalURL) {
		return nil, errors.New("invalid URL")
	}

	// Кастомный алиас или генерация
	var code string
	if customAlias != "" {
		if !isValidAlias(customAlias) {
			return nil, errors.New("invalid custom alias")
		}
		exists, err := s.repo.CodeExists(customAlias)
		if err != nil {
			return nil, err
		}
		if exists {
			return nil, errors.New("alias already in use")
		}
		code = customAlias
	} else {
		for {
			code = generateRandomString(6)
			exists, err := s.repo.CodeExists(code)
			if err != nil {
				return nil, err
			}
			if !exists {
				break
			}
		}
	}

	url := &model.URL{
		OriginalURL: originalURL,
		ShortCode:   code,
		ExpiresAt:   expiresAt,
	}
	if err := s.repo.Create(url); err != nil {
		return nil, err
	}
	return url, nil
}

// GetByCode возвращает оригинальный URL по коду и увеличивает счётчик
func (s *ShortenerService) GetByCode(code string) (*model.URL, error) {
	url, err := s.repo.GetByCode(code)
	if err != nil {
		return nil, err
	}
	if url == nil {
		return nil, errors.New("not found")
	}
	// Проверка срока действия
	if url.ExpiresAt != nil && time.Now().After(*url.ExpiresAt) {
		return nil, errors.New("link expired")
	}
	// Увеличиваем счётчик (ошибка не критична для пользователя)
	_ = s.repo.IncrementClicks(code)
	return url, nil
}

// GetStats возвращает статистику по коду
func (s *ShortenerService) GetStats(code string) (*model.URL, error) {
	url, err := s.repo.GetByCode(code)
	if err != nil {
		return nil, err
	}
	if url == nil {
		return nil, errors.New("not found")
	}
	return url, nil
}

// BuildShortURL формирует полный короткий URL
func (s *ShortenerService) BuildShortURL(code string) string {
	return strings.TrimRight(s.baseURL, "/") + "/" + code
}

// Вспомогательные функции
func isValidURL(rawURL string) bool {
	u, err := url.ParseRequestURI(rawURL)
	if err != nil {
		return false
	}
	return u.Scheme == "http" || u.Scheme == "https"
}

func isValidAlias(alias string) bool {
	if len(alias) < 4 || len(alias) > 20 {
		return false
	}
	for _, r := range alias {
		if !strings.ContainsRune(charset, r) {
			return false
		}
	}
	return true
}

func generateRandomString(n int) string {
	b := make([]byte, n)
	for i := range b {
		b[i] = charset[rand.Intn(len(charset))]
	}
	return string(b)
}
