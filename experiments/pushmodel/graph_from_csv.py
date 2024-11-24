import csv

from pathlib import Path

import matplotlib.pyplot as plt

csv_file_path = Path("stats.csv")

num_requests = []
avg_latencies = []

with open(csv_file_path, "r") as csv_file:
    csv_reader = csv.reader(csv_file)
    for row in csv_reader:
        num_requests.append(int(row[0]))
        avg_latencies.append(float(row[1]))

fig, ax = plt.subplots()
ax.plot(num_requests, avg_latencies)
ax.set_ylim(0, 1)
ax.set_xlabel("Number of messages")
ax.set_ylabel("Latency (ms)")
ax.set_title("Latency vs Number of messages")
plt.savefig("latency_vs_num_messages.png")
