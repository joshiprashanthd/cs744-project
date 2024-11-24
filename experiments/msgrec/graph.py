import csv
import datetime

from pathlib import Path
import os

import matplotlib.pyplot as plt


def num_rows(tsv_file_path):
    with open(tsv_file_path, "r") as tsv_file:
        tsv_reader = csv.reader(tsv_file, delimiter="\t")
        return sum(1 for _ in tsv_reader)


def plot_graph(x_values, y_values):
    fig, ax = plt.subplots()
    ax.plot(x_values, y_values)
    ax.set_ylim(0, 100)
    ax.set_xlabel("Number of messages")
    ax.set_ylabel("Miss Count")
    ax.set_title("Miss Count vs Number of messages")
    plt.savefig("num_req_vs_miss_count.png")
    plt.show()


output_folder_path = Path("./output")
num_requests = []
num_recv = []
for file in os.listdir(output_folder_path):
    if file.startswith("subscriber"):
        file = Path(file)
        num_req = int(file.stem.split("_")[-1])
        num_requests.append(num_req)
        num_recv.append(num_req - num_rows(output_folder_path / file))


temp = sorted(zip(num_requests, num_recv))
num_requests = [x for x, _ in temp]
num_recv = [y for _, y in temp]


plot_graph(num_requests, num_recv)
