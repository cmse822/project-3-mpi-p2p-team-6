import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('../results/part1_pingpong_multiple_nodes.csv')
df2 = pd.read_csv('../results/part2_pingpong_non_blocking.csv')

df_plot = df.set_index('DataSize(B)')
df2_plot = df2.set_index('DataSize(B)')

fig, axes = plt.subplots(3, 1, figsize=(10, 15))

df.plot.scatter(x='DataSize(B)', y='AvgPingPong(ms)', ax=axes[0], color='red', title='AvgPingPong(ms)')
df.plot.scatter(x='DataSize(B)', y='Bandwidth(GB/s)', ax=axes[1], color='blue', title='Bandwidth(GB/s)')
df.plot.scatter(x='DataSize(B)', y='Latency(ms)', ax=axes[2], color='green', title='Latency(ms)')

plt.tight_layout()

plt.legend(loc="upper left")

plt.show()

fig, axes = plt.subplots(3, 1, figsize=(10, 15))

df2.plot.scatter(x='DataSize(B)', y='AvgPingPong(ms)', ax=axes[0], color='red', title='AvgPingPong(ms)')
df2.plot.scatter(x='DataSize(B)', y='Bandwidth(GB/s)', ax=axes[1], color='blue', title='Bandwidth(GB/s)')
df2.plot.scatter(x='DataSize(B)', y='Latency(ms)', ax=axes[2], color='green', title='Latency(ms)')

plt.tight_layout()

plt.legend(loc="upper left")

plt.show()

