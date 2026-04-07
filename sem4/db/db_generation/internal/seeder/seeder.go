package seeder

import (
	"context"
	"fmt"
	"math/rand"
	"strings"
	"time"

	"github.com/jackc/pgx/v5"
	"github.com/jackc/pgx/v5/pgxpool"
)

// Config holds seeder parameters.
type Config struct {
	Employees           int
	Cars                int
	ParkingZones        int
	ZonePointsMin       int
	ZonePointsMax       int
	TracksPerCar        int
	TrackPointsPerTrack int
	AlertEvents         int
	GeoRequests         int
	BatchSize           int
}

// DefaultConfig returns config matching the prompt (~300 000 rows total).
func DefaultConfig() Config {
	return Config{
		Employees:           30,
		Cars:                100,
		ParkingZones:        200,
		ZonePointsMin:       4,
		ZonePointsMax:       6,
		TracksPerCar:        50,
		TrackPointsPerTrack: 57,
		AlertEvents:         500,
		GeoRequests:         3000,
		BatchSize:           1000,
	}
}

// Report holds row counts per table after seeding.
type Report struct {
	TableCounts map[string]int
	TotalRows   int
}

type trackRef struct {
	ID        int
	CarID     int
	ZoneID    int
	StartTime time.Time
	EndTime   *time.Time
}

type level1Refs struct {
	EmpTypeIDs         []int
	EmpStatusIDs       []int
	CarStatusIDs       []int
	ZoneTypeIDs        []int
	ZoneStatusIDs      []int
	TrackStatusIDs     []int
	TrackKindIDs       []int
	DataSourceIDs      []int
	AlertEventTypeIDs  []int
	AlertProcStatusIDs []int
	GeoReqTypeIDs      []int
}

type level2Refs struct {
	EmployeeIDs    []int
	CarIDs         []int
	ParkingZoneIDs []int
}

// Фиксированные значения справочников.

var empTypeData = []struct{ name, desc string }{
	{"Оператор колл-центра", "Обрабатывает входящие обращения клиентов"},
	{"Администратор системы", "Управляет системой и пользователями"},
	{"Сервисный специалист", "Обслуживает автомобили и оборудование"},
	{"Аналитик", "Анализирует данные и формирует отчёты"},
}

var empStatusNames = []string{"Активен", "В отпуске", "Уволен", "Заблокирован"}
var carStatusNames = []string{"Свободен", "В аренде", "На обслуживании", "Неисправен"}
var zoneTypeNames = []string{"Бесплатная", "Платная", "Запрещённая"}
var zoneStatusNames = []string{"Активна", "Временно закрыта", "Недоступна"}
var trackStatusNames = []string{"Активный", "Завершён", "Архивный"}
var trackKindNames = []string{"Пользовательский", "Сервисный"}
var dataSourceNames = []string{"GPS", "ГЛОНАСС", "GPS/ГЛОНАСС", "Инерциальная система"}

var alertEventTypeData = []struct{ name, desc string }{
	{"Потеря связи", "Автомобиль не выходит на связь"},
	{"Превышение скорости", "Скорость выше допустимого предела"},
	{"Резкое торможение", "Зафиксировано экстренное торможение"},
	{"Удар", "Зафиксировано столкновение"},
	{"Попытка угона", "Несанкционированное использование"},
	{"Выезд за границу", "Автомобиль покинул разрешённую зону"},
}

var alertProcStatusNames = []string{"Новое", "В работе", "Закрыто"}

var geoReqTypeData = []struct{ name, desc string }{
	{"Текущие координаты", "Запрос текущего местоположения"},
	{"История событий", "Запрос истории перемещений"},
}

// Данные для генерации русских ФИО.

var surnames = []string{
	"Иванов", "Смирнов", "Кузнецов", "Попов", "Васильев",
	"Петров", "Соколов", "Михайлов", "Новиков", "Фёдоров",
	"Морозов", "Волков", "Алексеев", "Лебедев", "Семёнов",
	"Егоров", "Павлов", "Козлов", "Степанов", "Николаев",
	"Орлов", "Андреев", "Макаров", "Никитин", "Захаров",
	"Зайцев", "Соловьёв", "Борисов", "Яковлев", "Григорьев",
}

var firstNamesMale = []string{
	"Александр", "Алексей", "Андрей", "Артём", "Василий",
	"Виктор", "Владимир", "Григорий", "Дмитрий", "Евгений",
	"Иван", "Игорь", "Кирилл", "Константин", "Максим",
	"Михаил", "Николай", "Павел", "Роман", "Сергей",
}

var patronymicsMale = []string{
	"Александрович", "Алексеевич", "Андреевич", "Артёмович", "Васильевич",
	"Викторович", "Владимирович", "Григорьевич", "Дмитриевич", "Евгеньевич",
	"Иванович", "Игоревич", "Кириллович", "Константинович", "Максимович",
	"Михайлович", "Николаевич", "Павлович", "Романович", "Сергеевич",
}

// Марки и модели автомобилей российского рынка.

var carData = []struct {
	brand  string
	models []string
}{
	{"Lada", []string{"Vesta", "Granta", "Niva Travel", "Largus", "Vesta SW"}},
	{"Kia", []string{"Rio", "Sportage", "Ceed", "K5", "Sorento"}},
	{"Hyundai", []string{"Solaris", "Tucson", "Creta", "Santa Fe", "Elantra"}},
	{"Toyota", []string{"Camry", "RAV4", "Land Cruiser", "Corolla", "Highlander"}},
	{"Volkswagen", []string{"Polo", "Tiguan", "Passat", "Taos", "Touareg"}},
	{"Renault", []string{"Logan", "Sandero", "Duster", "Arkana", "Kaptur"}},
	{"Nissan", []string{"Qashqai", "X-Trail", "Juke", "Almera", "Pathfinder"}},
	{"Skoda", []string{"Octavia", "Rapid", "Superb", "Kodiaq", "Karoq"}},
	{"Mazda", []string{"3", "6", "CX-5", "CX-9", "MX-5"}},
	{"BMW", []string{"3 Series", "5 Series", "X5", "X3", "7 Series"}},
}

// Russian Cyrillic letters allowed in licence plates.
var plateLetters = []rune("АВЕКМНОРСТУХ")

// ─── parking zone name data ───────────────────────────────────────────────────

var zoneNamePrefixes = []string{
	"ТЦ Мега", "ТЦ Галерея", "ТЦ Авиапарк", "ТЦ Columbus",
	"ТЦ Европейский", "ТЦ Атриум", "ТЦ Рио", "ТЦ Ереван Плаза",
	"БЦ Москва-Сити", "Парк Горького", "ВДНХ", "Красная площадь",
	"Лужники", "Сокольники", "Измайловский парк", "Коломенское",
	"ЖК Северный", "ЖК Западный", "МЦД Одинцово", "МЦД Подольск",
}

var zoneNameSuffixes = []string{
	"северный въезд", "южный въезд", "восточный въезд", "западный въезд",
	"парковка А", "парковка Б", "парковка В", "подземная парковка",
	"уровень 1", "уровень 2", "главный вход", "боковой вход",
}

var moscowDistricts = []string{
	"Центральный", "Северный", "Северо-Восточный", "Восточный",
	"Юго-Восточный", "Южный", "Юго-Западный", "Западный", "Северо-Западный",
}

// ─── helpers ──────────────────────────────────────────────────────────────────

func transliterate(s string) string {
	table := map[rune]string{
		'а': "a", 'б': "b", 'в': "v", 'г': "g", 'д': "d",
		'е': "e", 'ё': "yo", 'ж': "zh", 'з': "z", 'и': "i",
		'й': "y", 'к': "k", 'л': "l", 'м': "m", 'н': "n",
		'о': "o", 'п': "p", 'р': "r", 'с': "s", 'т': "t",
		'у': "u", 'ф': "f", 'х': "kh", 'ц': "ts", 'ч': "ch",
		'ш': "sh", 'щ': "shch", 'ъ': "", 'ы': "y", 'ь': "",
		'э': "e", 'ю': "yu", 'я': "ya",
	}
	var b strings.Builder
	for _, r := range strings.ToLower(s) {
		if t, ok := table[r]; ok {
			b.WriteString(t)
		} else {
			b.WriteRune(r)
		}
	}
	return b.String()
}

// genRegNumber returns a unique Russian licence plate like А000АА77.
func genRegNumber(i int) string {
	n := len(plateLetters)
	l1 := plateLetters[i%n]
	digits := i % 1000
	l2 := plateLetters[(i/n)%n]
	l3 := plateLetters[(i/(n*n))%n]
	return fmt.Sprintf("%c%03d%c%c77", l1, digits, l2, l3)
}

// randIDs inserts fixed string values into a lookup table and returns the generated IDs.
func insertLookupWithDesc(ctx context.Context, tx pgx.Tx, table, idCol string, rows []struct{ name, desc string }) ([]int, error) {
	ids := make([]int, 0, len(rows))
	for _, row := range rows {
		var id int
		query := fmt.Sprintf(
			"INSERT INTO %s(name, description) VALUES ($1, $2) RETURNING %s",
			table, idCol,
		)
		if err := tx.QueryRow(ctx, query, row.name, row.desc).Scan(&id); err != nil {
			return nil, fmt.Errorf("insert %s(%q): %w", table, row.name, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

func insertLookupNames(ctx context.Context, tx pgx.Tx, table, idCol string, names []string) ([]int, error) {
	ids := make([]int, 0, len(names))
	for _, name := range names {
		var id int
		query := fmt.Sprintf(
			"INSERT INTO %s(name) VALUES ($1) RETURNING %s",
			table, idCol,
		)
		if err := tx.QueryRow(ctx, query, name).Scan(&id); err != nil {
			return nil, fmt.Errorf("insert %s(%q): %w", table, name, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

// ─── truncate ─────────────────────────────────────────────────────────────────

func truncateAll(ctx context.Context, pool *pgxpool.Pool) error {
	query := `TRUNCATE TABLE
		geo_request,
		alert_event,
		track_point,
		parking_session,
		track,
		car,
		parking_zone_point,
		parking_zone,
		employee,
		employee_type,
		employee_status,
		car_status,
		parking_zone_type,
		parking_zone_status,
		track_status_type,
		track_kind_type,
		data_source_type,
		alert_event_type,
		alert_event_process_status,
		geo_request_type
	RESTART IDENTITY CASCADE`
	if _, err := pool.Exec(ctx, query); err != nil {
		return fmt.Errorf("truncate tables: %w", err)
	}
	return nil
}

// ─── level 1 — справочники ────────────────────────────────────────────────────

func seedLevel1(ctx context.Context, pool *pgxpool.Pool, _ *rand.Rand) (*level1Refs, int, error) {
	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, 0, fmt.Errorf("level1 begin: %w", err)
	}
	defer tx.Rollback(ctx)

	refs := &level1Refs{}
	n := 0

	refs.EmpTypeIDs, err = insertLookupWithDesc(ctx, tx, "employee_type", "employee_type_id", empTypeData)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.EmpTypeIDs)

	refs.EmpStatusIDs, err = insertLookupNames(ctx, tx, "employee_status", "employee_status_id", empStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.EmpStatusIDs)

	refs.CarStatusIDs, err = insertLookupNames(ctx, tx, "car_status", "car_status_id", carStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.CarStatusIDs)

	refs.ZoneTypeIDs, err = insertLookupNames(ctx, tx, "parking_zone_type", "zone_type_id", zoneTypeNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.ZoneTypeIDs)

	refs.ZoneStatusIDs, err = insertLookupNames(ctx, tx, "parking_zone_status", "zone_status_id", zoneStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.ZoneStatusIDs)

	refs.TrackStatusIDs, err = insertLookupNames(ctx, tx, "track_status_type", "track_status_id", trackStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.TrackStatusIDs)

	refs.TrackKindIDs, err = insertLookupNames(ctx, tx, "track_kind_type", "track_kind_id", trackKindNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.TrackKindIDs)

	refs.DataSourceIDs, err = insertLookupNames(ctx, tx, "data_source_type", "data_source_id", dataSourceNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.DataSourceIDs)

	refs.AlertEventTypeIDs, err = insertLookupWithDesc(ctx, tx, "alert_event_type", "alert_event_type_id", alertEventTypeData)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.AlertEventTypeIDs)

	refs.AlertProcStatusIDs, err = insertLookupNames(ctx, tx, "alert_event_process_status", "status_id", alertProcStatusNames)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.AlertProcStatusIDs)

	refs.GeoReqTypeIDs, err = insertLookupWithDesc(ctx, tx, "geo_request_type", "geo_request_type_id", geoReqTypeData)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.GeoReqTypeIDs)

	if err = tx.Commit(ctx); err != nil {
		return nil, 0, fmt.Errorf("level1 commit: %w", err)
	}
	return refs, n, nil
}

// ─── level 2 — основные сущности ─────────────────────────────────────────────

func seedLevel2(ctx context.Context, pool *pgxpool.Pool, r *rand.Rand, cfg Config, l1 *level1Refs) (*level2Refs, int, error) {
	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, 0, fmt.Errorf("level2 begin: %w", err)
	}
	defer tx.Rollback(ctx)

	refs := &level2Refs{}
	n := 0

	refs.EmployeeIDs, err = seedEmployees(ctx, tx, r, cfg, l1)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.EmployeeIDs)
	fmt.Printf("  employees: %d\n", len(refs.EmployeeIDs))

	refs.CarIDs, err = seedCars(ctx, tx, r, cfg, l1)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.CarIDs)
	fmt.Printf("  cars: %d\n", len(refs.CarIDs))

	refs.ParkingZoneIDs, err = seedParkingZones(ctx, tx, r, cfg, l1)
	if err != nil {
		return nil, 0, err
	}
	n += len(refs.ParkingZoneIDs)
	fmt.Printf("  parking_zones: %d\n", len(refs.ParkingZoneIDs))

	if err = tx.Commit(ctx); err != nil {
		return nil, 0, fmt.Errorf("level2 commit: %w", err)
	}
	return refs, n, nil
}

func seedEmployees(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs) ([]int, error) {
	ids := make([]int, 0, cfg.Employees)
	for i := 0; i < cfg.Employees; i++ {
		surname := surnames[i%len(surnames)]
		firstName := firstNamesMale[r.Intn(len(firstNamesMale))]
		patronymic := patronymicsMale[r.Intn(len(patronymicsMale))]
		fullName := fmt.Sprintf("%s %s %s", surname, firstName, patronymic)
		login := fmt.Sprintf("%s%02d", transliterate(surname), i+1)

		var id int
		query := "INSERT INTO employee(employee_type_id, full_name, login, employee_status_id) VALUES ($1, $2, $3, $4) RETURNING employee_id"
		err := tx.QueryRow(ctx, query,
			l1.EmpTypeIDs[r.Intn(len(l1.EmpTypeIDs))],
			fullName,
			login,
			l1.EmpStatusIDs[r.Intn(len(l1.EmpStatusIDs))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert employee[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

func seedCars(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs) ([]int, error) {
	ids := make([]int, 0, cfg.Cars)
	for i := 0; i < cfg.Cars; i++ {
		brand := carData[r.Intn(len(carData))]
		model := brand.models[r.Intn(len(brand.models))]
		regNum := genRegNumber(i)
		vin := fmt.Sprintf("X%016d", i+1)

		var id int
		query := "INSERT INTO car(reg_number, brand, model, manufacture_year, vin, car_status_id) VALUES ($1, $2, $3, $4, $5, $6) RETURNING car_id"
		err := tx.QueryRow(ctx, query,
			regNum,
			brand.brand,
			model,
			2010+r.Intn(15),
			vin,
			l1.CarStatusIDs[r.Intn(len(l1.CarStatusIDs))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert car[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

func seedParkingZones(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs) ([]int, error) {
	ids := make([]int, 0, cfg.ParkingZones)
	for i := 0; i < cfg.ParkingZones; i++ {
		prefix := zoneNamePrefixes[r.Intn(len(zoneNamePrefixes))]
		suffix := zoneNameSuffixes[r.Intn(len(zoneNameSuffixes))]
		name := fmt.Sprintf("%s — %s", prefix, suffix)
		if len([]rune(name)) > 60 {
			name = string([]rune(name)[:60])
		}
		district := moscowDistricts[r.Intn(len(moscowDistricts))]

		var id int
		query := "INSERT INTO parking_zone(name, zone_type_id, city_district, max_cars, zone_status_id) VALUES ($1, $2, $3, $4, $5) RETURNING parking_zone_id"
		err := tx.QueryRow(ctx, query,
			name,
			l1.ZoneTypeIDs[r.Intn(len(l1.ZoneTypeIDs))],
			district,
			20+r.Intn(80),
			l1.ZoneStatusIDs[r.Intn(len(l1.ZoneStatusIDs))],
		).Scan(&id)
		if err != nil {
			return nil, fmt.Errorf("insert parking_zone[%d]: %w", i, err)
		}
		ids = append(ids, id)
	}
	return ids, nil
}

// ─── level 3 — зависимые сущности ────────────────────────────────────────────

func seedLevel3(ctx context.Context, pool *pgxpool.Pool, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) ([]trackRef, int, error) {
	tx, err := pool.Begin(ctx)
	if err != nil {
		return nil, 0, fmt.Errorf("level3 begin: %w", err)
	}
	defer tx.Rollback(ctx)

	n := 0

	pts, err := seedParkingZonePoints(ctx, tx, r, cfg, l2.ParkingZoneIDs)
	if err != nil {
		return nil, 0, err
	}
	n += pts
	fmt.Printf("  parking_zone_points: %d\n", pts)

	tracks, err := seedTracks(ctx, tx, r, cfg, l1, l2)
	if err != nil {
		return nil, 0, err
	}
	n += len(tracks)
	fmt.Printf("  tracks: %d\n", len(tracks))

	sessions, err := seedParkingSessions(ctx, tx, tracks)
	if err != nil {
		return nil, 0, err
	}
	n += sessions
	fmt.Printf("  parking_sessions: %d\n", sessions)

	alerts, err := seedAlertEvents(ctx, tx, r, cfg, l1, l2)
	if err != nil {
		return nil, 0, err
	}
	n += alerts
	fmt.Printf("  alert_events: %d\n", alerts)

	geoReqs, err := seedGeoRequests(ctx, tx, r, cfg, l1, l2)
	if err != nil {
		return nil, 0, err
	}
	n += geoReqs
	fmt.Printf("  geo_requests: %d\n", geoReqs)

	if err = tx.Commit(ctx); err != nil {
		return nil, 0, fmt.Errorf("level3 commit: %w", err)
	}
	return tracks, n, nil
}

func seedParkingZonePoints(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, zoneIDs []int) (int, error) {
	total := 0
	for _, zoneID := range zoneIDs {
		count := cfg.ZonePointsMin + r.Intn(cfg.ZonePointsMax-cfg.ZonePointsMin+1)
		for v := 1; v <= count; v++ {
			lat := 55.7 + r.Float64()*0.2
			lon := 37.3 + r.Float64()*0.6
			query := "INSERT INTO parking_zone_point(parking_zone_id, vertex_number, latitude, longitude) VALUES ($1, $2, $3, $4)"
			if _, err := tx.Exec(ctx, query, zoneID, v, lat, lon); err != nil {
				return 0, fmt.Errorf("insert parking_zone_point(zone=%d, v=%d): %w", zoneID, v, err)
			}
			total++
		}
	}
	return total, nil
}

func seedTracks(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) ([]trackRef, error) {
	refs := make([]trackRef, 0, len(l2.CarIDs)*cfg.TracksPerCar)
	now := time.Now()

	for _, carID := range l2.CarIDs {
		for j := 0; j < cfg.TracksPerCar; j++ {
			startTime := now.Add(-time.Duration(r.Intn(365*24)) * time.Hour)
			duration := time.Duration(10+r.Intn(50)) * time.Minute
			endTime := startTime.Add(duration)

			// Завершённые и архивные треки имеют end_time; активные — нет.
			statusID := l1.TrackStatusIDs[r.Intn(len(l1.TrackStatusIDs))]
			var endPtr *time.Time
			// track_status_id == TrackStatusIDs[0] → "Активный" (без end_time)
			if statusID != l1.TrackStatusIDs[0] {
				endPtr = &endTime
			}

			var id int
			query := "INSERT INTO track(car_id, start_time, end_time, track_status_id, track_kind_id) VALUES ($1, $2, $3, $4, $5) RETURNING track_id"
			err := tx.QueryRow(ctx, query,
				carID,
				startTime,
				endPtr,
				statusID,
				l1.TrackKindIDs[r.Intn(len(l1.TrackKindIDs))],
			).Scan(&id)
			if err != nil {
				return nil, fmt.Errorf("insert track(car=%d, j=%d): %w", carID, j, err)
			}
			refs = append(refs, trackRef{
				ID:        id,
				CarID:     carID,
				ZoneID:    l2.ParkingZoneIDs[r.Intn(len(l2.ParkingZoneIDs))],
				StartTime: startTime,
				EndTime:   endPtr,
			})
		}
	}
	return refs, nil
}

func seedParkingSessions(ctx context.Context, tx pgx.Tx, tracks []trackRef) (int, error) {
	for i, tr := range tracks {
		query := "INSERT INTO parking_session(car_id, parking_zone_id, entry_time, exit_time) VALUES ($1, $2, $3, $4)"
		if _, err := tx.Exec(ctx, query, tr.CarID, tr.ZoneID, tr.StartTime, tr.EndTime); err != nil {
			return 0, fmt.Errorf("insert parking_session[%d]: %w", i, err)
		}
	}
	return len(tracks), nil
}

func seedAlertEvents(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) (int, error) {
	for i := 0; i < cfg.AlertEvents; i++ {
		lat := 55.7 + r.Float64()*0.2
		lon := 37.3 + r.Float64()*0.6
		query := "INSERT INTO alert_event(car_id, employee_id, alert_event_type_id, latitude, longitude, description, status_id) VALUES ($1, $2, $3, $4, $5, $6, $7)"
		_, err := tx.Exec(ctx, query,
			l2.CarIDs[r.Intn(len(l2.CarIDs))],
			l2.EmployeeIDs[r.Intn(len(l2.EmployeeIDs))],
			l1.AlertEventTypeIDs[r.Intn(len(l1.AlertEventTypeIDs))],
			lat,
			lon,
			fmt.Sprintf("Событие #%d", i+1),
			l1.AlertProcStatusIDs[r.Intn(len(l1.AlertProcStatusIDs))],
		)
		if err != nil {
			return 0, fmt.Errorf("insert alert_event[%d]: %w", i, err)
		}
	}
	return cfg.AlertEvents, nil
}

func seedGeoRequests(ctx context.Context, tx pgx.Tx, r *rand.Rand, cfg Config, l1 *level1Refs, l2 *level2Refs) (int, error) {
	for i := 0; i < cfg.GeoRequests; i++ {
		query := "INSERT INTO geo_request(employee_id, car_id, geo_request_type_id, request_goal) VALUES ($1, $2, $3, $4)"
		_, err := tx.Exec(ctx, query,
			l2.EmployeeIDs[r.Intn(len(l2.EmployeeIDs))],
			l2.CarIDs[r.Intn(len(l2.CarIDs))],
			l1.GeoReqTypeIDs[r.Intn(len(l1.GeoReqTypeIDs))],
			fmt.Sprintf("Запрос #%d", i+1),
		)
		if err != nil {
			return 0, fmt.Errorf("insert geo_request[%d]: %w", i, err)
		}
	}
	return cfg.GeoRequests, nil
}

// ─── level 4 — track_point ────────────────────────────────────────────────────

func seedLevel4(ctx context.Context, pool *pgxpool.Pool, r *rand.Rand, cfg Config, tracks []trackRef, dataSourceIDs []int) (int, error) {
	type row struct {
		trackID      int
		carID        int
		lat, lon     float64
		speed        float64
		dataSourceID int
	}

	batch := make([]row, 0, cfg.BatchSize)
	total := 0

	flush := func() error {
		if len(batch) == 0 {
			return nil
		}
		tx, err := pool.Begin(ctx)
		if err != nil {
			return fmt.Errorf("level4 begin: %w", err)
		}
		defer tx.Rollback(ctx)

		for _, b := range batch {
			query := "INSERT INTO track_point(track_id, car_id, latitude, longitude, speed_kmh, data_source_id) VALUES ($1, $2, $3, $4, $5, $6)"
			if _, err := tx.Exec(ctx, query, b.trackID, b.carID, b.lat, b.lon, b.speed, b.dataSourceID); err != nil {
				return fmt.Errorf("insert track_point: %w", err)
			}
		}
		if err := tx.Commit(ctx); err != nil {
			return fmt.Errorf("level4 commit: %w", err)
		}
		total += len(batch)
		batch = batch[:0]
		return nil
	}

	for _, tr := range tracks {
		lat := 55.7 + r.Float64()*0.2
		lon := 37.3 + r.Float64()*0.6

		for p := 0; p < cfg.TrackPointsPerTrack; p++ {
			lat += (r.Float64() - 0.5) * 0.002
			lon += (r.Float64() - 0.5) * 0.003
			// keep within Moscow bounds
			if lat < 55.7 {
				lat = 55.7
			}
			if lat > 55.9 {
				lat = 55.9
			}
			if lon < 37.3 {
				lon = 37.3
			}
			if lon > 37.9 {
				lon = 37.9
			}

			batch = append(batch, row{
				trackID:      tr.ID,
				carID:        tr.CarID,
				lat:          lat,
				lon:          lon,
				speed:        float64(r.Intn(121)),
				dataSourceID: dataSourceIDs[r.Intn(len(dataSourceIDs))],
			})

			if len(batch) >= cfg.BatchSize {
				if err := flush(); err != nil {
					return 0, err
				}
			}
		}
	}
	if err := flush(); err != nil {
		return 0, err
	}
	return total, nil
}

// ─── public API ───────────────────────────────────────────────────────────────

// Seed fills the DB with DefaultConfig.
func Seed(ctx context.Context, pool *pgxpool.Pool) error {
	_, err := SeedWithReport(ctx, pool, DefaultConfig())
	return err
}

// SeedWithReport fills the DB and returns a row-count report.
func SeedWithReport(ctx context.Context, pool *pgxpool.Pool, cfg Config) (*Report, error) {
	r := rand.New(rand.NewSource(time.Now().UnixNano()))

	if err := truncateAll(ctx, pool); err != nil {
		return nil, err
	}

	fmt.Println("=== Уровень 1: справочники ===")
	l1, n1, err := seedLevel1(ctx, pool, r)
	if err != nil {
		return nil, fmt.Errorf("level1: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n1)

	fmt.Println("=== Уровень 2: основные сущности ===")
	l2, n2, err := seedLevel2(ctx, pool, r, cfg, l1)
	if err != nil {
		return nil, fmt.Errorf("level2: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n2)

	fmt.Println("=== Уровень 3: зависимые сущности ===")
	tracks, n3, err := seedLevel3(ctx, pool, r, cfg, l1, l2)
	if err != nil {
		return nil, fmt.Errorf("level3: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n3)

	fmt.Println("=== Уровень 4: точки треков ===")
	n4, err := seedLevel4(ctx, pool, r, cfg, tracks, l1.DataSourceIDs)
	if err != nil {
		return nil, fmt.Errorf("level4: %w", err)
	}
	fmt.Printf("  Вставлено: %d\n\n", n4)

	fmt.Printf("=== Итого вставлено: %d ===\n", n1+n2+n3+n4)
	return buildReport(ctx, pool)
}

// ─── report ───────────────────────────────────────────────────────────────────

var reportTables = []string{
	"employee_type", "employee_status",
	"car_status", "parking_zone_type", "parking_zone_status",
	"track_status_type", "track_kind_type", "data_source_type",
	"alert_event_type", "alert_event_process_status", "geo_request_type",
	"employee", "car", "parking_zone",
	"parking_zone_point", "track", "parking_session",
	"alert_event", "geo_request", "track_point",
}

func buildReport(ctx context.Context, pool *pgxpool.Pool) (*Report, error) {
	report := &Report{TableCounts: make(map[string]int, len(reportTables))}
	for _, table := range reportTables {
		var count int
		if err := pool.QueryRow(ctx, fmt.Sprintf("SELECT COUNT(*) FROM %s", table)).Scan(&count); err != nil {
			return nil, fmt.Errorf("count %s: %w", table, err)
		}
		report.TableCounts[table] = count
		report.TotalRows += count
	}
	return report, nil
}
