"""Baseline benchmark: DuckDB and Polars computing mean + population variance
over every column of a CSV, equivalent to what the C Fast Parser does.

Methodology: one discarded warmup run to warm the OS page cache, then the
median of 5 timed runs (min/max also reported). Times are in-process work only
(imports excluded) and throughput is file_bytes / seconds.

Requires: pip install duckdb polars
"""
import os
import sys
import time
import statistics

import duckdb
import polars as pl

FILE_PATH = sys.argv[1] if len(sys.argv) > 1 else "data/gigantic_test.csv"
RUNS = 5


def bench(label, fn):
    fn()  # warmup (discarded) — warms the page cache
    times = [ ]
    for _ in range(RUNS):
        t0 = time.perf_counter()
        fn()
        times.append(time.perf_counter() - t0)
    med = statistics.median(times)
    gbps = os.path.getsize(FILE_PATH) / med / 1e9
    print(f"{label:10} median={med:7.4f}s  min={min(times):7.4f}s  "
          f"max={max(times):7.4f}s  {gbps:5.2f} GB/s")


def run_duckdb():
    con = duckdb.connect()
    con.execute("PRAGMA threads=14")
    q = (f"SELECT avg(COLUMNS(*)), var_pop(COLUMNS(*)) "
         f"FROM read_csv_auto('{FILE_PATH}', header=false)")
    con.execute(q).fetchall()


def run_polars():
    lf = pl.scan_csv(FILE_PATH, has_header=False)
    lf.select(
        pl.all().mean().name.suffix("_mean"),
        pl.all().var(ddof=0).name.suffix("_var"),
    ).collect()


if __name__ == "__main__":
    if not os.path.exists(FILE_PATH):
        print(f"Could not find {FILE_PATH}")
        sys.exit(1)

    print(f"--- Baseline benchmark on {FILE_PATH} "
          f"({os.path.getsize(FILE_PATH):,} bytes), median of {RUNS} warm runs ---")
    bench("DuckDB", run_duckdb)
    bench("Polars", run_polars)
