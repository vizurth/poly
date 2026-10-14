import csv
import os
import re
import subprocess
import tempfile
from pathlib import Path

cache_dir = Path(tempfile.gettempdir()) / "lab1-matplotlib-cache"
os.environ.setdefault("MPLCONFIGDIR", str(cache_dir))
os.environ.setdefault("XDG_CACHE_HOME", str(cache_dir))
os.environ.setdefault("GOCACHE", str(Path(tempfile.gettempdir()) / "lab1-go-build-cache"))

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt


REPORT_DIR = Path(__file__).resolve().parent
PROJECT_DIR = REPORT_DIR.parent
DATA_DIR = REPORT_DIR / "analysis_data"
IMAGE_DIR = REPORT_DIR / "myPhoto"
TEST_OUTPUT = subprocess.run(
    ["go", "test", "-run", "^TestRandomFieldMetrics$", "-v", "./internal/automation"],
    cwd=PROJECT_DIR,
    check=True,
    capture_output=True,
    text=True,
).stdout

rows = [
    {
        "step": int(step),
        "alive": int(alive),
        "dead": int(dead),
    }
    for step, alive, dead in re.findall(r"METRICS,(\d+),(\d+),(\d+)", TEST_OUTPUT)
]
if not rows:
    raise RuntimeError("Go test did not return any metrics")

for row in rows:
    row["alive_fraction"] = row["alive"] / (30 * 30)

cycle = re.search(r"CYCLE,start=(\d+),repeat=(\d+),period=(\d+)", TEST_OUTPUT)
stable_step = int(cycle.group(1)) if cycle else None
DATA_DIR.mkdir(exist_ok=True)
IMAGE_DIR.mkdir(exist_ok=True)
with (DATA_DIR / "random_30x30.csv").open("w", newline="", encoding="utf-8") as file:
    writer = csv.DictWriter(file, fieldnames=["step", "alive", "dead", "alive_fraction"])
    writer.writeheader()
    writer.writerows(rows)

steps = [row["step"] for row in rows]
alive = [row["alive"] for row in rows]
dead = [row["dead"] for row in rows]
alive_fraction = [row["alive_fraction"] for row in rows]

fig, axis = plt.subplots(figsize=(9, 4.5))
axis.plot(steps, alive_fraction, label="Доля живых клеток", color="#2878b5", linewidth=2)
if stable_step is not None:
    axis.axvline(
        stable_step,
        color="#555555",
        linestyle="--",
        linewidth=1,
        label=f"Стабилизация: шаг {stable_step}",
    )
axis.set_xlabel("Номер шага")
axis.set_ylabel("Доля живых клеток")
axis.set_title("Изменение доли живых клеток, поле 30×30")
axis.set_ylim(0, 0.55)
axis.grid(alpha=0.25)
axis.legend(loc="center right")

fig.tight_layout()
fig.savefig(IMAGE_DIR / "random_30x30_metrics.png", dpi=180, bbox_inches="tight")

print(f"rows={len(rows)}")
print(f"initial_alive={alive[0]}, initial_dead={dead[0]}")
if stable_step is not None:
    print(
        f"stable_alive={alive[stable_step]}, stable_dead={dead[stable_step]}, "
        f"stable_fraction={alive_fraction[stable_step]:.6f}"
    )
print(f"final_alive={alive[-1]}, final_dead={dead[-1]}, final_fraction={alive_fraction[-1]:.6f}")
if cycle:
    print(f"cycle_start={cycle.group(1)}, cycle_repeat={cycle.group(2)}, cycle_period={cycle.group(3)}")
print(f"chart={IMAGE_DIR / 'random_30x30_metrics.png'}")
