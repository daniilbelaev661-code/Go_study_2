package repository

import (
    "database/sql"
    "errors"
    "time"

    "url-shortener/internal/model"
)

type sqliteRepo struct {
    db *sql.DB
}

func NewSQLiteURLRepository(db *sql.DB) URLRepository {
    return &sqliteRepo{db: db}
}

// Migrate создаёт таблицу, если её нет
func Migrate(db *sql.DB) error {
    _, err := db.Exec(`CREATE TABLE IF NOT EXISTS urls (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        original_url TEXT NOT NULL,
        short_code TEXT UNIQUE NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
        expires_at TIMESTAMP NULL,
        clicks INTEGER DEFAULT 0
    )`)
    return err
}

func (r *sqliteRepo) Create(url *model.URL) error {
    res, err := r.db.Exec(
        "INSERT INTO urls (original_url, short_code, expires_at) VALUES (?, ?, ?)",
        url.OriginalURL, url.ShortCode, url.ExpiresAt,
    )
    if err != nil {
        return err
    }
    id, err := res.LastInsertId()
    if err != nil {
        return err
    }
    url.ID = id
    url.CreatedAt = time.Now()
    return nil
}

func (r *sqliteRepo) GetByCode(code string) (*model.URL, error) {
    url := &model.URL{}
    err := r.db.QueryRow(
        "SELECT id, original_url, short_code, created_at, expires_at, clicks FROM urls WHERE short_code = ?",
        code,
    ).Scan(&url.ID, &url.OriginalURL, &url.ShortCode, &url.CreatedAt, &url.ExpiresAt, &url.Clicks)
    if errors.Is(err, sql.ErrNoRows) {
        return nil, nil // не найдено
    }
    if err != nil {
        return nil, err
    }
    return url, nil
}

func (r *sqliteRepo) IncrementClicks(code string) error {
    _, err := r.db.Exec("UPDATE urls SET clicks = clicks + 1 WHERE short_code = ?", code)
    return err
}

func (r *sqliteRepo) CodeExists(code string) (bool, error) {
    var count int
    err := r.db.QueryRow("SELECT COUNT(*) FROM urls WHERE short_code = ?", code).Scan(&count)
    if err != nil {
        return false, err
    }
    return count > 0, nil
}