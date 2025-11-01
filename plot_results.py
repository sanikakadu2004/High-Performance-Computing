# plot_results.py
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")
# Convert timestamp to readable times if desired
# df['time_readable'] = pd.to_datetime(df['timestamp'], unit='s')

# We will plot for each algorithm: size vs avg_time_s, one plot per distribution
distributions = df['distribution'].unique()
algos = df['algo'].unique()

for dist in distributions:
    sub = df[df['distribution']==dist]
    plt.figure(figsize=(8,5))
    for algo in algos:
        s = sub[sub['algo']==algo]
        if s.empty:
            continue
        plt.plot(s['size'], s['avg_time_s'], marker='o', label=algo)
    plt.xscale('log')  # log scale often useful
    plt.yscale('log')
    plt.xlabel('Input size (n)')
    plt.ylabel('Average time (s)')
    plt.title(f"Sorting performance — distribution: {dist}")
    plt.legend()
    plt.grid(True, which="both", ls="--", linewidth=0.5)
    plt.tight_layout()
    plt.savefig(f"plot_{dist}.png")
    plt.show()
