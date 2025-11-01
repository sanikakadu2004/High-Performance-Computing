import matplotlib.pyplot as plt

# Read data from file
methods = []
times = []

with open("sort_times.txt", "r") as f:
    for line in f:
        parts = line.strip().split()
        methods.append(parts[0])
        times.append(float(parts[1]))

# Create bar chart
plt.figure(figsize=(8,5))
plt.bar(methods, times, color=['skyblue', 'orange', 'lightgreen'])
plt.title("Comparison of Sorting Algorithms")
plt.xlabel("Sorting Method")
plt.ylabel("Time (seconds)")
plt.grid(axis='y', linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()
