import csv
import datetime

from pathlib import Path
import os

import matplotlib.pyplot as plt


def compute_avg_latency(tsv_file_path):
    with open(tsv_file_path, "r") as tsv_file:
        tsv_reader = csv.reader(tsv_file, delimiter="\t")
        latencies = []
        for row in tsv_reader:
            if len(row) < 3:
                continue
            start_time = datetime.datetime.strptime(row[2], "%Y-%m-%d %H:%M:%S")
            end_time = datetime.datetime.strptime(row[1], "%Y-%m-%d %H:%M:%S")
            latency = (end_time - start_time).total_seconds()
            latencies.append(latency)
        avg_latency = sum(latencies) / len(latencies)
        return avg_latency


def plot_graph(num_requests, avg_latencies):
    fig, ax = plt.subplots()
    ax.plot(num_requests, avg_latencies)
    ax.set_ylim(0, 8)
    ax.set_xlabel("Number of messages")
    ax.set_ylabel("Latency (ms)")
    ax.set_title("Latency vs Number of messages")
    plt.savefig("latency_vs_num_messages.png")
    plt.show()


output_folder_path = Path("./output")
num_requests = []
avg_latencies = []
for file in os.listdir(output_folder_path):
    if file.startswith("subscriber"):
        file = Path(file)
        num_requests.append(int(file.stem.split("_")[-1]))
        avg_latencies.append(compute_avg_latency(output_folder_path / file))


temp = sorted(zip(num_requests, avg_latencies))
num_requests = [x for x, _ in temp]
avg_latencies = [y for _, y in temp]


plot_graph(num_requests, avg_latencies)
