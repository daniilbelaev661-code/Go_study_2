package repository

import "url-shortener/internal/model"

type URLRepository interface {
	Create(url *model.URL) error
	GetByCode(code string) (*model.URL, error)
	IncrementClicks(code string) error
	CodeExists(code string) (bool, error)
}
