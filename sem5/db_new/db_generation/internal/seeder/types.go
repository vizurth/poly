package seeder

import "time"

type Config struct {
	Employees           int
	Cars                int
	ParkingZones        int
	ZonePointsMin       int
	ZonePointsMax       int
	TracksPerCarMin     int
	TracksPerCarMax     int
	SessionsPerCarMin   int
	SessionsPerCarMax   int
	TrackPointsMin      int
	TrackPointsMax      int
	AlertEvents         int
	AlertEventPerCarMin int
	AlertEventPerCarMax int
	GeoRequestsMin      int
	GeoRequestsMax      int
	BatchSize           int
}

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
