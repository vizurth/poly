# EXPLAIN ANALYZE

## Запрос 1.1

**Planning Time:** 3.444 ms  
**Execution Time:** 248.770 ms

```
Unique  (cost=137.20..137.23 rows=2 width=28) (actual time=194.206..248.148 rows=58 loops=1)
  ->  Sort  (cost=137.20..137.21 rows=2 width=28) (actual time=194.205..226.159 rows=132712 loops=1)
        Sort Key: c.car_id, c.reg_number, c.brand, c.model
        Sort Method: external merge  Disk: 5120kB
        ->  Hash Join  (cost=9.77..137.19 rows=2 width=28) (actual time=0.101..152.211 rows=132712 loops=1)
              Hash Cond: (tp.data_source_id = dst.data_source_id)
              ->  Nested Loop  (cost=1.58..124.52 rows=1704 width=32) (actual time=0.073..119.243 rows=527613 loops=1)
                    ->  Nested Loop  (cost=1.16..16.46 rows=20 width=32) (actual time=0.064..9.420 rows=6210 loops=1)
                          ->  Nested Loop  (cost=0.88..15.58 rows=1 width=36) (actual time=0.060..5.947 rows=303 loops=1)
                                ->  Nested Loop  (cost=0.74..15.41 rows=1 width=8) (actual time=0.053..5.253 rows=303 loops=1)
                                      ->  Nested Loop  (cost=0.59..13.52 rows=6 width=12) (actual time=0.047..3.468 rows=1810 loops=1)
                                            ->  Nested Loop  (cost=0.44..12.88 rows=3 width=4) (actual time=0.041..1.686 rows=935 loops=1)
                                                  ->  Nested Loop  (cost=0.16..11.63 rows=1 width=4) (actual time=0.030..0.325 rows=64 loops=1)
                                                        ->  Seq Scan on parking_zone pz  (cost=0.00..5.00 rows=200 width=8) (actual time=0.005..0.083 rows=200 loops=1)
                                                        ->  Memoize  (cost=0.16..0.42 rows=1 width=4) (actual time=0.001..0.001 rows=0 loops=200)
                                                              Cache Key: pz.zone_type_id
                                                              Cache Mode: logical
                                                              Hits: 197  Misses: 3  Evictions: 0  Overflows: 0  Memory Usage: 1kB
                                                              ->  Index Scan using parking_zone_type_pkey on parking_zone_type pzt  (cost=0.15..0.41 rows=1 width=4) (actual time=0.007..0.007 rows=0 loops=3)
                                                                    Index Cond: (zone_type_id = pz.zone_type_id)
                                                                    Filter: ((name)::text = 'Бесплатная'::text)
                                                                    Rows Removed by Filter: 1
                                                  ->  Index Scan using idx_parking_session_parking_zone_id on parking_session ps  (cost=0.28..1.10 rows=15 width=8) (actual time=0.007..0.018 rows=15 loops=64)
                                                        Index Cond: (parking_zone_id = pz.parking_zone_id)
                                            ->  Index Scan using idx_alert_event_car_id on alert_event ae  (cost=0.15..0.19 rows=2 width=8) (actual time=0.001..0.001 rows=2 loops=935)
                                                  Index Cond: (car_id = ps.car_id)
                                      ->  Index Scan using alert_event_type_pkey on alert_event_type aet  (cost=0.15..0.29 rows=1 width=4) (actual time=0.001..0.001 rows=0 loops=1810)
                                            Index Cond: (alert_event_type_id = ae.alert_event_type_id)
                                            Filter: ((name)::text = 'Потеря связи'::text)
                                            Rows Removed by Filter: 1
                                ->  Index Scan using car_pkey on car c  (cost=0.14..0.17 rows=1 width=28) (actual time=0.001..0.001 rows=1 loops=303)
                                      Index Cond: (car_id = ps.car_id)
                          ->  Index Scan using idx_track_car_id on track t  (cost=0.28..0.68 rows=20 width=8) (actual time=0.003..0.008 rows=20 loops=303)
                                Index Cond: (car_id = ps.car_id)
                    ->  Index Scan using idx_track_point_track_id on track_point tp  (cost=0.42..4.55 rows=85 width=8) (actual time=0.002..0.009 rows=85 loops=6210)
                          Index Cond: (track_id = t.track_id)
              ->  Hash  (cost=8.17..8.17 rows=1 width=4) (actual time=0.021..0.025 rows=1 loops=1)
                    Buckets: 1024  Batches: 1  Memory Usage: 9kB
                    ->  Index Scan using data_source_type_name_key on data_source_type dst  (cost=0.15..8.17 rows=1 width=4) (actual time=0.018..0.022 rows=1 loops=1)
                          Index Cond: ((name)::text = 'GPS'::text)
```

---

## Запрос 1.2

**Planning Time:** 2.730 ms  
**Execution Time:** 113.795 ms

```
GroupAggregate  (cost=137.20..137.24 rows=1 width=36) (actual time=101.074..113.312 rows=45 loops=1)
  Group Key: c.car_id
  Filter: (count(DISTINCT ps.parking_session_id) >= 3)
  Rows Removed by Filter: 13
  ->  Sort  (cost=137.20..137.21 rows=2 width=32) (actual time=100.738..105.760 rows=132712 loops=1)
        Sort Key: c.car_id
        Sort Method: external merge  Disk: 5792kB
        ->  Hash Join  (cost=9.77..137.19 rows=2 width=32) (actual time=0.122..81.443 rows=132712 loops=1)
              Hash Cond: (tp.data_source_id = dst.data_source_id)
              ->  Nested Loop  (cost=1.58..124.52 rows=1704 width=36) (actual time=0.079..61.180 rows=527613 loops=1)
                    ->  Nested Loop  (cost=1.16..16.46 rows=20 width=36) (actual time=0.071..3.255 rows=6210 loops=1)
                          ->  Nested Loop  (cost=0.88..15.58 rows=1 width=40) (actual time=0.060..2.065 rows=303 loops=1)
                                ->  Nested Loop  (cost=0.74..15.41 rows=1 width=12) (actual time=0.048..1.853 rows=303 loops=1)
                                      ->  Nested Loop  (cost=0.59..13.52 rows=6 width=16) (actual time=0.037..1.187 rows=1810 loops=1)
                                            ->  Nested Loop  (cost=0.44..12.88 rows=3 width=8) (actual time=0.025..0.501 rows=935 loops=1)
                                                  ->  Nested Loop  (cost=0.16..11.63 rows=1 width=4) (actual time=0.011..0.080 rows=64 loops=1)
                                                        ->  Seq Scan on parking_zone pz  (cost=0.00..5.00 rows=200 width=8) (actual time=0.003..0.019 rows=200 loops=1)
                                                        ->  Memoize  (cost=0.16..0.42 rows=1 width=4) (actual time=0.000..0.000 rows=0 loops=200)
                                                              Cache Key: pz.zone_type_id
                                                              Cache Mode: logical
                                                              Hits: 197  Misses: 3  Evictions: 0  Overflows: 0  Memory Usage: 1kB
                                                              ->  Index Scan using parking_zone_type_pkey on parking_zone_type pzt  (cost=0.15..0.41 rows=1 width=4) (actual time=0.002..0.002 rows=0 loops=3)
                                                                    Index Cond: (zone_type_id = pz.zone_type_id)
                                                                    Filter: ((name)::text = 'Бесплатная'::text)
                                                                    Rows Removed by Filter: 1
                                                  ->  Index Scan using idx_parking_session_parking_zone_id on parking_session ps  (cost=0.28..1.10 rows=15 width=12) (actual time=0.001..0.006 rows=15 loops=64)
                                                        Index Cond: (parking_zone_id = pz.parking_zone_id)
                                            ->  Index Scan using idx_alert_event_car_id on alert_event ae  (cost=0.15..0.19 rows=2 width=8) (actual time=0.000..0.001 rows=2 loops=935)
                                                  Index Cond: (car_id = ps.car_id)
                                      ->  Index Scan using alert_event_type_pkey on alert_event_type aet  (cost=0.15..0.29 rows=1 width=4) (actual time=0.000..0.000 rows=0 loops=1810)
                                            Index Cond: (alert_event_type_id = ae.alert_event_type_id)
                                            Filter: ((name)::text = 'Потеря связи'::text)
                                            Rows Removed by Filter: 1
                                ->  Index Scan using car_pkey on car c  (cost=0.14..0.17 rows=1 width=28) (actual time=0.000..0.000 rows=1 loops=303)
                                      Index Cond: (car_id = ps.car_id)
                          ->  Index Scan using idx_track_car_id on track t  (cost=0.28..0.68 rows=20 width=8) (actual time=0.001..0.003 rows=20 loops=303)
                                Index Cond: (car_id = ps.car_id)
                    ->  Index Scan using idx_track_point_track_id on track_point tp  (cost=0.42..4.55 rows=85 width=8) (actual time=0.001..0.005 rows=85 loops=6210)
                          Index Cond: (track_id = t.track_id)
              ->  Hash  (cost=8.17..8.17 rows=1 width=4) (actual time=0.028..0.029 rows=1 loops=1)
                    Buckets: 1024  Batches: 1  Memory Usage: 9kB
                    ->  Index Scan using data_source_type_name_key on data_source_type dst  (cost=0.15..8.17 rows=1 width=4) (actual time=0.024..0.024 rows=1 loops=1)
                          Index Cond: ((name)::text = 'GPS'::text)
```

---

## Запрос 2

**Planning Time:** 0.427 ms  
**Execution Time:** 0.166 ms

```
Aggregate  (cost=33.40..33.41 rows=1 width=8) (actual time=0.124..0.125 rows=1 loops=1)
  ->  Hash Join  (cost=16.77..33.39 rows=3 width=4) (actual time=0.094..0.113 rows=2 loops=1)
        Hash Cond: (ps.parking_zone_id = pz.parking_zone_id)
        ->  Nested Loop  (cost=11.23..27.36 rows=183 width=8) (actual time=0.044..0.086 rows=223 loops=1)
              ->  HashAggregate  (cost=10.95..11.08 rows=13 width=4) (actual time=0.035..0.036 rows=15 loops=1)
                    Group Key: ae.car_id
                    Batches: 1  Memory Usage: 24kB
                    ->  Nested Loop  (cost=4.25..10.92 rows=13 width=4) (actual time=0.025..0.031 rows=15 loops=1)
                          ->  Seq Scan on employee e  (cost=0.00..1.38 rows=1 width=4) (actual time=0.001..0.003 rows=1 loops=1)
                                Filter: ((full_name)::text = 'Иванов Игорь Николаевич'::text)
                                Rows Removed by Filter: 29
                          ->  Bitmap Heap Scan on alert_event ae  (cost=4.25..9.41 rows=13 width=8) (actual time=0.023..0.026 rows=15 loops=1)
                                Recheck Cond: (employee_id = e.employee_id)
                                Heap Blocks: exact=4
                                ->  Bitmap Index Scan on idx_alert_event_employee_id  (cost=0.00..4.25 rows=13 width=0) (actual time=0.021..0.021 rows=15 loops=1)
                                      Index Cond: (employee_id = e.employee_id)
              ->  Index Scan using idx_parking_session_car_id on parking_session ps  (cost=0.28..1.10 rows=15 width=8) (actual time=0.001..0.002 rows=15 loops=15)
                    Index Cond: (car_id = ae.car_id)
        ->  Hash  (cost=5.50..5.50 rows=3 width=4) (actual time=0.013..0.013 rows=3 loops=1)
              Buckets: 1024  Batches: 1  Memory Usage: 9kB
              ->  Seq Scan on parking_zone pz  (cost=0.00..5.50 rows=3 width=4) (actual time=0.003..0.011 rows=3 loops=1)
                    Filter: ((name)::text = 'ТЦ Мега — парковка В'::text)
                    Rows Removed by Filter: 197
```

---

## Запрос 3

**Planning Time:** 0.288 ms  
**Execution Time:** 0.297 ms

```
GroupAggregate  (cost=28.72..32.95 rows=30 width=238) (actual time=0.170..0.243 rows=30 loops=1)
  Group Key: e.employee_id
  ->  Sort  (cost=28.72..29.70 rows=393 width=230) (actual time=0.154..0.166 rows=393 loops=1)
        Sort Key: e.employee_id
        Sort Method: quicksort  Memory: 80kB
        ->  Hash Right Join  (cost=1.68..11.78 rows=393 width=230) (actual time=0.025..0.082 rows=393 loops=1)
              Hash Cond: (ae.employee_id = e.employee_id)
              ->  Seq Scan on alert_event ae  (cost=0.00..8.93 rows=393 width=12) (actual time=0.002..0.019 rows=393 loops=1)
              ->  Hash  (cost=1.30..1.30 rows=30 width=222) (actual time=0.016..0.016 rows=30 loops=1)
                    Buckets: 1024  Batches: 1  Memory Usage: 11kB
                    ->  Seq Scan on employee e  (cost=0.00..1.30 rows=30 width=222) (actual time=0.004..0.006 rows=30 loops=1)
```

---

## Запрос 4 MAX

**Planning Time:** 0.419 ms  
**Execution Time:** 1.743 ms

```
GroupAggregate  (cost=188.63..399.87 rows=1 width=61) (actual time=0.913..1.672 rows=3 loops=1)
  Group Key: pz.parking_zone_id
  Filter: (count(DISTINCT ps.car_id) = $0)
  Rows Removed by Filter: 197
  InitPlan 1 (returns $0)
    ->  Aggregate  (cost=175.69..175.70 rows=1 width=8) (actual time=0.692..0.693 rows=1 loops=1)
          ->  GroupAggregate  (cost=0.28..173.19 rows=200 width=12) (actual time=0.011..0.684 rows=200 loops=1)
                Group Key: ps2.parking_zone_id
                ->  Index Scan using idx_parking_session_parking_zone_id on parking_session ps2  (cost=0.28..156.46 rows=2946 width=8) (actual time=0.004..0.417 rows=2946 loops=1)
  ->  Merge Join  (cost=12.92..206.93 rows=2946 width=57) (actual time=0.050..0.677 rows=2946 loops=1)
        Merge Cond: (ps.parking_zone_id = pz.parking_zone_id)
        ->  Index Scan using idx_parking_session_parking_zone_id on parking_session ps  (cost=0.28..156.46 rows=2946 width=8) (actual time=0.005..0.376 rows=2946 loops=1)
        ->  Sort  (cost=12.64..13.14 rows=200 width=53) (actual time=0.042..0.049 rows=200 loops=1)
              Sort Key: pz.parking_zone_id
              Sort Method: quicksort  Memory: 49kB
              ->  Seq Scan on parking_zone pz  (cost=0.00..5.00 rows=200 width=53) (actual time=0.003..0.018 rows=200 loops=1)
```

## Запрос 4 MIN

**Planning Time:** 0.335 ms  
**Execution Time:** 1.449 ms

```
GroupAggregate  (cost=188.63..399.87 rows=1 width=61) (actual time=1.185..1.395 rows=1 loops=1)
  Group Key: pz.parking_zone_id
  Filter: (count(DISTINCT ps.car_id) = $0)
  Rows Removed by Filter: 199
  InitPlan 1 (returns $0)
    ->  Aggregate  (cost=175.69..175.70 rows=1 width=8) (actual time=0.574..0.574 rows=1 loops=1)
          ->  GroupAggregate  (cost=0.28..173.19 rows=200 width=12) (actual time=0.009..0.567 rows=200 loops=1)
                Group Key: ps2.parking_zone_id
                ->  Index Scan using idx_parking_session_parking_zone_id on parking_session ps2  (cost=0.28..156.46 rows=2946 width=8) (actual time=0.004..0.343 rows=2946 loops=1)
  ->  Merge Join  (cost=12.92..206.93 rows=2946 width=57) (actual time=0.045..0.590 rows=2946 loops=1)
        Merge Cond: (ps.parking_zone_id = pz.parking_zone_id)
        ->  Index Scan using idx_parking_session_parking_zone_id on parking_session ps  (cost=0.28..156.46 rows=2946 width=8) (actual time=0.005..0.327 rows=2946 loops=1)
        ->  Sort  (cost=12.64..13.14 rows=200 width=53) (actual time=0.038..0.044 rows=200 loops=1)
              Sort Key: pz.parking_zone_id
              Sort Method: quicksort  Memory: 49kB
              ->  Seq Scan on parking_zone pz  (cost=0.00..5.00 rows=200 width=53) (actual time=0.004..0.017 rows=200 loops=1)
```

---

## Запрос 5

**Planning Time:** 0.437 ms  
**Execution Time:** 4.812 ms

```
GroupAggregate  (cost=704.09..720.65 rows=828 width=234) (actual time=4.395..4.708 rows=120 loops=1)
  Group Key: e.full_name, (count(DISTINCT ae.car_id))
  ->  Sort  (cost=704.09..706.16 rows=828 width=226) (actual time=4.387..4.480 rows=3612 loops=1)
        Sort Key: e.full_name, (count(DISTINCT ae.car_id))
        Sort Method: quicksort  Memory: 604kB
        ->  Hash Join  (cost=483.59..663.96 rows=828 width=226) (actual time=1.819..3.603 rows=3612 loops=1)
              Hash Cond: (ae.employee_id = e.employee_id)
              ->  GroupAggregate  (cost=481.92..592.30 rows=5519 width=16) (actual time=1.797..3.272 rows=3612 loops=1)
                    Group Key: ae.employee_id, ps.parking_zone_id
                    ->  Sort  (cost=481.92..495.71 rows=5519 width=12) (actual time=1.792..1.965 rows=5773 loops=1)
                          Sort Key: ae.employee_id, ps.parking_zone_id
                          Sort Method: quicksort  Memory: 463kB
                          ->  Hash Join  (cost=13.84..138.91 rows=5519 width=12) (actual time=0.052..0.604 rows=5773 loops=1)
                                Hash Cond: (ps.car_id = ae.car_id)
                                ->  Seq Scan on parking_session ps  (cost=0.00..51.46 rows=2946 width=8) (actual time=0.001..0.115 rows=2946 loops=1)
                                ->  Hash  (cost=8.93..8.93 rows=393 width=8) (actual time=0.047..0.047 rows=393 loops=1)
                                      Buckets: 1024  Batches: 1  Memory Usage: 24kB
                                      ->  Seq Scan on alert_event ae  (cost=0.00..8.93 rows=393 width=8) (actual time=0.002..0.023 rows=393 loops=1)
              ->  Hash  (cost=1.30..1.30 rows=30 width=222) (actual time=0.012..0.012 rows=30 loops=1)
                    Buckets: 1024  Batches: 1  Memory Usage: 11kB
                    ->  Seq Scan on employee e  (cost=0.00..1.30 rows=30 width=222) (actual time=0.006..0.008 rows=30 loops=1)
```

---

## Запрос 6

**Planning Time:** 0.331 ms  
**Execution Time:** 0.217 ms

```
Sort  (cost=25.25..25.27 rows=10 width=230) (actual time=0.166..0.168 rows=5 loops=1)
  Sort Key: (count(ae.alert_event_id)) DESC
  Sort Method: quicksort  Memory: 25kB
  InitPlan 1 (returns $1)
    ->  Aggregate  (cost=10.95..10.96 rows=1 width=8) (actual time=0.024..0.025 rows=1 loops=1)
          ->  Nested Loop  (cost=4.25..10.92 rows=13 width=4) (actual time=0.017..0.023 rows=15 loops=1)
                ->  Seq Scan on employee e2  (cost=0.00..1.38 rows=1 width=4) (actual time=0.002..0.003 rows=1 loops=1)
                      Filter: ((full_name)::text = 'Иванов Игорь Николаевич'::text)
                      Rows Removed by Filter: 29
                ->  Bitmap Heap Scan on alert_event ae2  (cost=4.25..9.41 rows=13 width=4) (actual time=0.014..0.017 rows=15 loops=1)
                      Recheck Cond: (employee_id = e2.employee_id)
                      Heap Blocks: exact=4
                      ->  Bitmap Index Scan on idx_alert_event_employee_id  (cost=0.00..4.25 rows=13 width=0) (actual time=0.011..0.011 rows=15 loops=1)
                            Index Cond: (employee_id = e2.employee_id)
  ->  HashAggregate  (cost=13.75..14.12 rows=10 width=230) (actual time=0.152..0.155 rows=5 loops=1)
        Group Key: e.employee_id
        Filter: (count(ae.alert_event_id) > $1)
        Batches: 1  Memory Usage: 24kB
        Rows Removed by Filter: 25
        ->  Hash Join  (cost=1.68..11.78 rows=393 width=226) (actual time=0.026..0.088 rows=393 loops=1)
              Hash Cond: (ae.employee_id = e.employee_id)
              ->  Seq Scan on alert_event ae  (cost=0.00..8.93 rows=393 width=8) (actual time=0.002..0.023 rows=393 loops=1)
              ->  Hash  (cost=1.30..1.30 rows=30 width=222) (actual time=0.012..0.012 rows=30 loops=1)
                    Buckets: 1024  Batches: 1  Memory Usage: 11kB
                    ->  Seq Scan on employee e  (cost=0.00..1.30 rows=30 width=222) (actual time=0.005..0.007 rows=30 loops=1)
```

---

## Запрос 7

**Planning Time:** 0.403 ms  
**Execution Time:** 0.100 ms

```
Seq Scan on parking_zone pz  (cost=14.23..19.73 rows=100 width=53) (actual time=0.033..0.059 rows=186 loops=1)
  Filter: (NOT (hashed SubPlan 1))
  Rows Removed by Filter: 14
  SubPlan 1
    ->  Nested Loop  (cost=0.28..14.19 rows=15 width=4) (actual time=0.006..0.019 rows=15 loops=1)
          ->  Seq Scan on car c  (cost=0.00..5.50 rows=1 width=4) (actual time=0.002..0.013 rows=1 loops=1)
                Filter: ((reg_number)::text = 'А001АА77'::text)
                Rows Removed by Filter: 199
          ->  Index Scan using idx_parking_session_car_id on parking_session ps  (cost=0.28..8.54 rows=15 width=8) (actual time=0.003..0.004 rows=15 loops=1)
                Index Cond: (car_id = c.car_id)
```

---

## Запрос 8

**Planning Time:** 0.432 ms  
**Execution Time:** 139.903 ms

```
Incremental Sort  (cost=605.10..73391.34 rows=162000 width=96) (actual time=8.252..139.822 rows=800 loops=1)
  Sort Key: c.car_id, dst.name
  Presorted Key: c.car_id
  Full-sort Groups: 25  Sort Method: quicksort  Average Memory: 28kB  Peak Memory: 28kB
  ->  GroupAggregate  (cost=250.55..58710.05 rows=162000 width=96) (actual time=0.647..139.649 rows=800 loops=1)
        Group Key: c.car_id, dst.data_source_id
        ->  Incremental Sort  (cost=250.55..54577.16 rows=335051 width=92) (actual time=0.616..124.452 rows=335051 loops=1)
              Sort Key: c.car_id, dst.data_source_id
              Presorted Key: c.car_id
              Full-sort Groups: 200  Sort Method: quicksort  Average Memory: 30kB  Peak Memory: 30kB
              Pre-sorted Groups: 200  Sort Method: quicksort  Average Memory: 335kB  Peak Memory: 340kB
              ->  Merge Join  (cost=0.73..21582.88 rows=335051 width=92) (actual time=0.030..89.733 rows=335051 loops=1)
                    Merge Cond: (tp.car_id = c.car_id)
                    ->  Nested Loop  (cost=0.58..17377.10 rows=335051 width=80) (actual time=0.025..71.970 rows=335051 loops=1)
                          ->  Index Scan using idx_track_point_car_id on track_point tp  (cost=0.42..9006.19 rows=335051 width=12) (actual time=0.013..21.259 rows=335051 loops=1)
                          ->  Memoize  (cost=0.16..0.18 rows=1 width=72) (actual time=0.000..0.000 rows=1 loops=335051)
                                Cache Key: tp.data_source_id
                                Cache Mode: logical
                                Hits: 335047  Misses: 4  Evictions: 0  Overflows: 0  Memory Usage: 1kB
                                ->  Index Scan using data_source_type_pkey on data_source_type dst  (cost=0.15..0.17 rows=1 width=72) (actual time=0.003..0.003 rows=1 loops=4)
                                      Index Cond: (data_source_id = tp.data_source_id)
                    ->  Index Scan using car_pkey on car c  (cost=0.14..17.14 rows=200 width=16) (actual time=0.003..0.036 rows=200 loops=1)
```

---

## Запрос 9

**Planning Time:** 0.266 ms  
**Execution Time:** 0.078 ms

```
Update on track  (cost=22.12..30.52 rows=0 width=0) (actual time=0.041..0.041 rows=0 loops=1)
  InitPlan 1 (returns $0)
    ->  Index Scan using track_status_type_name_key on track_status_type  (cost=0.15..8.17 rows=1 width=4) (never executed)
          Index Cond: ((name)::text = 'Завершён'::text)
  InitPlan 2 (returns $1)
    ->  Seq Scan on car  (cost=0.00..5.50 rows=1 width=4) (actual time=0.004..0.016 rows=1 loops=1)
          Filter: ((reg_number)::text = 'А001АА77'::text)
          Rows Removed by Filter: 199
  InitPlan 3 (returns $2)
    ->  Index Scan using track_status_type_name_key on track_status_type track_status_type_1  (cost=0.15..8.17 rows=1 width=4) (actual time=0.010..0.010 rows=1 loops=1)
          Index Cond: ((name)::text = 'Активный'::text)
  ->  Index Scan using idx_track_car_id on track  (cost=0.28..8.68 rows=7 width=10) (actual time=0.040..0.040 rows=0 loops=1)
        Index Cond: (car_id = $1)
        Filter: (track_status_id = $2)
        Rows Removed by Filter: 13
```
