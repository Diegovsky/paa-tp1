#!/usr/bin/env python
import os
import sys
import json
import collector

counts_str = "10 20 50 100 1000 5000 10000 20000 50000 100000".split(" ")
counts = list(map(int, counts_str)) # type: list[int]

filenames = []

def gen(gen_files):
    for y in counts:
        x = y*10
        filename = f'test_{x}_{y}.txt'
        if gen_files or not os.path.isfile(filename):
            os.system(f'./gen {x} {y} {filename}')
        filenames.append(filename)

def analyze():
    iotimes = []
    runtimes = []
    for f in filenames:
        io, run = collector.avgs_of(f)
        iotimes.append(io)
        runtimes.append(run)

    obj = {
        'counts': counts,
        'iotimes': list(map(collector.AvgTimes.toJSON, iotimes)),
        'runtimes': list(map(collector.AvgTimes.toJSON, runtimes)),
    }
    with open('results.json', 'w') as f:
        json.dump(obj, f)

    return obj

def graph(times: list[collector.Times], counts, output):
    import matplotlib.pyplot as plt

    x_values = [t.phys for t in times]
    _, ax = plt.subplots()

    ax.plot(x_values, counts)
    ax.set_yscale('log')

    ax.set_xlabel('Tempo de execução (físico)')
    ax.set_ylabel('Quantidade de arestas')
    ax.set_title('Logarithmic Plot')

    plt.savefig(output)


gen_files = len(sys.argv) > 1 and sys.argv[1] == '-g'
gen(gen_files)

try:
    with open('results.json') as r:
        obj = json.load(r)
except FileNotFoundError:
    obj = analyze()

counts = obj['counts']
graph(obj['iotimes'], counts, 'iotimes.pdf')
graph(obj['runtimes'], counts, 'runtimes.pdf')



