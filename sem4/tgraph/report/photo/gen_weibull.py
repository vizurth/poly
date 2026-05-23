"""
Генерация 1000 чисел по распределению Вейбулла той же формулой,
что используется в C++-реализации:
    X = a * (-ln(r))^(1/c),  r ~ Uniform(0, 1)
Параметры из main.cpp: a=10, c=2
"""

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from math import gamma

np.random.seed(42)

# --- параметры (как в main.cpp) ---
a = 10.0
c = 2.0
N = 1000

# --- генерация (точно как в weibull.cpp) ---
r = np.random.uniform(0, 1, N)
samples = a * (-np.log(r)) ** (1.0 / c)

# --- теоретическая PDF ---
x = np.linspace(0.01, np.max(samples) * 1.05, 500)
pdf = (c / a) * (x / a) ** (c - 1) * np.exp(-(x / a) ** c)

# --- статистики ---
mean_theory = a * gamma(1 + 1.0 / c)   # 10 * Gamma(1.5) ≈ 8.86
mode_theory = a * ((c - 1) / c) ** (1.0 / c)  # 10 * 0.5^0.5 ≈ 7.07

# --- стиль ---
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 11,
    'axes.linewidth': 0.8,
})

fig, ax = plt.subplots(figsize=(7, 4))

ax.hist(samples, bins=40, density=True,
        color='steelblue', alpha=0.65,
        edgecolor='white', linewidth=0.4,
        label='Выборка ($n=1000$)')

ax.plot(x, pdf, color='firebrick', linewidth=1.8,
        label=fr'Теор. плотность ($a={a:.0f},\; c={c:.0f}$)')

ax.axvline(mean_theory, color='darkgreen', linestyle='--', linewidth=1.2,
           label=fr'Среднее $\approx {mean_theory:.2f}$')

ax.set_xlabel('$x$')
ax.set_ylabel('Плотность')
ax.set_title('Распределение Вейбулла')
ax.legend(fontsize=9)
ax.set_xlim(left=0)
ax.grid(True, linestyle=':', linewidth=0.5, alpha=0.7)

plt.tight_layout()
out = '/Users/timuradiatullin/narnia/spbpu/poly/sem4/tgraph/report/photo/weibull_plot.pdf'
plt.savefig(out, dpi=200, bbox_inches='tight')
print(f'Saved: {out}')
print(f'Sample mean={samples.mean():.3f}, std={samples.std():.3f}')
print(f'Theory mean={mean_theory:.3f}, mode={mode_theory:.3f}')
