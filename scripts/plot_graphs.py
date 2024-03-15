import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

PING_PONG_BLOCKING_FILENAME = '../results/part1_pingpong.csv'
PING_PONG_BLOCKING_MULTI_NODE_FILENAME = '../results/part1_pingpong_multiple_nodes.csv'

PING_PONG_NON_BLOCKING_FILENAME = '../results/part2_pingpong_non_blocking.csv'
PING_PONG_NON_BLOCKING_MULTI_NODE_FILENAME = '../results/part2_pingpong_non_blocking_multiple_nodes.csv'

RING_SHIFT_BLOCKING_FILENAME = '../results/part3_ringshift.csv'
RING_SHIFT_NON_BLOCKING_FILENAME = '../results/part4_ringshift_non_blocking.csv'

def plot_part1_part2_pingpong(filename, savefilename, prefix_title=""):
    df = pd.read_csv(filename)

    # Plot AvgPingPong
    plt.figure(figsize=(8, 6))
    plt.plot(df['DataSize(B)'], df['AvgPingPong(ms)'], color='red', linestyle='-')
    plt.scatter(df['DataSize(B)'], df['AvgPingPong(ms)'], color='red', marker='o')
    plt.title(prefix_title + ': ' + 'AvgPingPong(ms) Over Message Size')
    plt.xlabel('DataSize(B)')
    plt.ylabel('AvgPingPong(ms)')
    plt.tight_layout()
    plt.savefig('../plots/' + savefilename + '_avgpingpong.png')
    plt.close()

    # Plot Bandwidth
    plt.figure(figsize=(8, 6))
    plt.plot(df['DataSize(B)'], df['Bandwidth(GB/s)'], color='blue', linestyle='-')
    plt.scatter(df['DataSize(B)'], df['Bandwidth(GB/s)'], color='blue', marker='o')
    plt.title(prefix_title + ': ' + 'Bandwidth(GB/s) Over Message Size')
    plt.xlabel('DataSize(B)')
    plt.ylabel('Bandwidth(GB/s)')
    plt.tight_layout()
    plt.savefig('../plots/' + savefilename + '_bandwidth.png')
    plt.close()

    # Plot Latency
    plt.figure(figsize=(8, 6))
    plt.plot(df['DataSize(B)'], df['Latency(ms)'], color='green', linestyle='-')
    plt.scatter(df['DataSize(B)'], df['Latency(ms)'], color='green', marker='o')
    plt.title(prefix_title + ': ' + 'Latency(ms) Over Message Size')
    plt.xlabel('DataSize(B)')
    plt.ylabel('Latency(ms)')
    plt.tight_layout()
    plt.savefig('../plots/' + savefilename + '_latency.png')
    plt.close()

def plot_part3_part4_ringshift(filename, savefilename, prefix_title=""):
    df = pd.read_csv(filename)

    # Filter rows where DataSize(B) is a power of 2
    df = df[df['DataSize(B)'].apply(lambda x: x & (x - 1) == 0)]

    # Group by NumMPIRanks
    groups = df.groupby('NumMPIRanks')

    plt.figure(figsize=(8, 6))

    for name, group in groups:
        plt.plot(group['DataSize(B)'], group['Bandwidth(GB/s)'], label=f'{name} NumMPIRanks')

    plt.title(prefix_title + ': ' + 'Bandwidth(GB/s) vs DataSize(B) for Different NumMPIRanks')
    plt.xlabel('DataSize(B)')
    plt.ylabel('Bandwidth(GB/s)')
    plt.xscale('log', base=2)
    plt.xticks(df['DataSize(B)'].unique())  # Set x-axis ticks to unique DataSize(B) values
    plt.legend()
    plt.tight_layout()
    plt.savefig('../plots/' + savefilename + '_bandwidth.png')


if __name__ == "__main__":
    plot_part1_part2_pingpong(PING_PONG_BLOCKING_FILENAME, 'part1_pingpong_blocking', 'Single Node Blocking')
    plot_part1_part2_pingpong(PING_PONG_BLOCKING_MULTI_NODE_FILENAME, 'part1_pingpong_blocking_multiple_nodes', 'Multiple Node Blocking')
    
    plot_part1_part2_pingpong(PING_PONG_NON_BLOCKING_FILENAME, 'part2_pingpong_non_blocking', 'Single Node Non-Blocking')
    plot_part1_part2_pingpong(PING_PONG_NON_BLOCKING_MULTI_NODE_FILENAME, 'part2_pingpong_non_blocking_multiple_nodes', 'Multiple Node Non-Blocking')
    
    plot_part3_part4_ringshift(RING_SHIFT_BLOCKING_FILENAME, 'part3_ringshift_blocking', 'Ringshift Blocking')
    plot_part3_part4_ringshift(RING_SHIFT_NON_BLOCKING_FILENAME, 'part4_ringshift_non_blocking', 'Ringshift Non-Blocking')
