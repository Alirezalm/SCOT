import os
from benchmark import plot_results_from_file

files = os.listdir()

for file in files:
    if "json" in file:
        plot_results_from_file(file)
