[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=13989339&assignment_repo_type=AssignmentRepo)
# Project 3: MPI Ping-Pong and Ring Shift

## Background

The ping-pong problem is a benchmark often used to evaluate the performance of message passing interfaces (MPI) in parallel computing. In this problem, two processes exchange messages back and forth a specified number of times, with each process sending a message and receiving a message alternately. In the ping-pong, process `i` sends a message of size `m` to process `j`, then receives a message of size `m` back from `j`. The values of `i`, `j`, and `m` to use are given below.

The "ring shift" problem is similar to ping-pong. In the MPI ring shift, a group of processes is arranged in a ring, with each process holding a unique subset of a larger array of data. The goal is to shift the data elements by a specified number of positions around the ring, wrapping around the ends of the ring as necessary.

## Part 1: Blocking Ping-Pong

Your task is to implement the ping-pong problem using MPI in C or C++ and analyze the behavior and performance of your code. Specifically, you should:

1. Implement the ping-pong problem using MPI in C or C++. Use blocking `MPI_Send()` and `MPI_Recv()` calls. You should define the number of iterations and the size of the message to be exchanged.

   Done.
2. Measure the time taken to complete the ping-pong exchange for different message sizes. You should use the `MPI_Wtime()` function to obtain the time before and after the exchange and calculate the elapsed time. Vary the message size from 2 bytes to 4 kilobytes in powers of 2 (i.e., 2 bytes, 4 bytes, 8 bytes,..., 2048 bytes, 4096 bytes). For each message size, perform 100 iterations of the ping-pong to build up statistical significance.

   Done.
3. Record the total amount of data sent and received during the ping-pong exchange for each configuration.

   Done. Results are in `results/part1_pingpong.csv`.
4. Repeat steps 2 and 3 but ensure that the 2 processes that are communicating reside on different physical hardware nodes on HPCC.

   Done. Results are in `results/part1_pingpong_multiple_nodes.csv`.
5. Plot the average communication time of a single exchange (send and receive) as a function of message size for the two cases. Using this plot, estimate the _latency_ and _bandwidth_ for each case. Are they different? Explain your results.

   Below are the plotted graphs of the two ping pong results that we were able to achieve for both scenarios. That is, the blocking intra-node communication and inter-node communications.

   **Blocking - Single Node**

   ![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part1_pingpong_blocking_avgpingpong.png)

   **Blocking - Multiple Nodes**

   ![Blocking Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part1_pingpong_blocking_multiple_nodes_avgpingpong.png)

   Additionally, we provided the bandwidth and latency graphs below

   **Blocking Bandwidth Over Message Sizes - Single Node**

   ![Blocking Bandwidth Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part1_pingpong_blocking_bandwidth.png)

   **Blocking Latency Over Message Sizes - Single Node**

   ![Blocking Latency Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part1_pingpong_blocking_latency.png)

   **Blocking Bandwidth Over Message Sizes - Multiple Nodes**

   ![Blocking Bandwidth Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part1_pingpong_blocking_multiple_nodes_bandwidth.png)

   **Blocking Latency Over Message Sizes - Multiple Nodes**

   ![Blocking Latency Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part1_pingpong_blocking_multiple_nodes_latency.png)

   And here is the chart for the two scenarios displaying the estimated latency and bandwidth. I got the bandwidth numbers by taking 1/slope of the communication time graphs. I got latency by taking the intercept of the linear regression line for the communication time graphs.

   | Scenario                 | Bandwidth (GB/s) | Latency (s) |
   |--------------------------|------------------|-------------|
   | Blocking Same Node       |       4.56GB/s   |   5.58E-6   |
   | Blocking Multiple Nodes  |       2.37GB/s   |   5.18E-5   |

   To answer the question, yes, they are different. The bandwidth is nearly half that on the same node versus different nodes, which makes sense since there's no network communication overhead (latency, congestion, infrastructure limitations). The latency is pretty similar, but still slightly different due to that inter-node communication.

6. Analyze and discuss your results. Explain the behavior of the resulting curves.

   The latency does differ slightly, because of  the cross-node communication. The plots show that as well as the latency graph for the multiple node has higher Y values.  The real difference between one and multiple nodes appears in the bandwidth. Multiple nodes seems to have a much smoother curve where singular jumps a lot. Also the single node seems to reach a higher max bandwidth of around 4.56GB/s where multiple nodes reaches around 2.37GB/s

## Part 2: Non-block Ping-Pong

Repeat Part 1 using non-blocking MPI communication, i.e., using `MPI_Isend()` and `MPI_Irecv()`. You will need to include explicit process synchronization using, e.g., `MPI_Wait()` calls. Compare the results to the blocking case.

**Non-Blocking - Single Node**

![Non-Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part2_pingpong_non_blocking_avgpingpong.png)

**Non-Blocking - Multiple Nodes**

![Non-Blocking Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part2_pingpong_non_blocking_multiple_nodes_avgpingpong.png)

Again, we provide the non-blocking plots for the bandwidth and latency since we had the data and it could be a useful visual to visualize how bandwidth and latency changes with respect to message size.

**Non-Blocking Bandwidth Over Message Sizes - Single Node**

![Non-Blocking Bandwidth Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part2_pingpong_non_blocking_bandwidth.png)

**Non-Blocking Latency Over Message Sizes - Single Node**

![Non-Blocking Latency Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part2_pingpong_non_blocking_latency.png)

**Non-Blocking Bandwidth Over Message Sizes - Multiple Nodes**

![Non-Blocking Bandwidth Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part2_pingpong_non_blocking_multiple_nodes_bandwidth.png)

**Non-Blocking Latency Over Message Sizes - Multiple Nodes**

![Non-Blocking Latency Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part2_pingpong_non_blocking_multiple_nodes_latency.png)

And here is the chart for the two scenarios displaying the estimated latency and bandwidth. I got the bandwidth numbers by taking 1/slope of the communication time graphs. I got latency by taking the intercept of the linear regression line for the communication time graphs.

| Scenario                     | Bandwidth (GB/s) | Latency (s) |
|------------------------------|------------------|-------------|
| Non-Blocking Same Node       |       6.48GB/s   |    1.57E-6  |
| Non-Blocking Multiple Nodes  |       4.18GB/s   |    4.43E-5  |

These results appear to be mostly the same as the blocking ping-pong results when just looking at the average ping and latency curves. The major differences come from the bandwidth curves. In the single node curve the bandwidth seems to have a more severe drop before leveling out. The multiple node curve does not appear to have this. Another difference would be the max bandwidth is almost double that of the respective curves from the blocking results.

## Part 3: MPI Ring Shift

1. Implement the MPI ring shift in C or C++ for an arbitrary number of processes in the ring and arbitrary message size (i.e., number of elements per process). In your implementation, use `MPI_Sendrecv()` instead of separate `MPI_Send()` and `MPI_Recv()` calls.

   Done.
2. As in Parts 1 and 2, vary the message size from 2 bytes to 4 kb, in powers of 2. Also vary the number of processes used from 2 to `N`, in powers of 2, where `N` is sufficiently large that rank 0 and rank `N-1` are guaranteed to reside on separate nodes (`N` will depend on which cluster you are using on HPCC).

   Done.
3. Compute the bandwidth and latency, as above. Plot the bandwidth as a function of message size. Include separate lines for each number of processes used.

   Below is our plotted bandwidth graph. Please note that this particular part doesn't really specify whether we have to run on on single node vs multiple nodes, but does state that N needs to big enough to run on multiple nodes. So I am going into this with the assumption that we are going to be communicating across nodes (for the larger N) and not needing to have graphs for solely single nodes. This question also wants us to just plot the bandwidth, so we do just that below:

   **Blocking Bandwidth for Ring Shift**

   ![Blocking Bandwidth Ring Shift](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part3_ringshift_blocking_bandwidth.png)

   And here is the tabular view of the computed bandwidth and latency.

   | Scenario                 | Bandwidth (GB/s) | Latency (s) |
   |--------------------------|------------------|-------------|
   | Ring Shift Blocking      |       1.35GB/s   |  2.12E-5    |

4. Analyze and discuss your results. Explain the behavior of the resulting curves.

The results show that the lower ranks resulted in the higher bandwidth, based on the graph above. Additionally, all curves increased as the data size increased. Latency looks to be aligned with the results from the blocking cases in the ping-pong part. This latency looks to be slightly lower, however.

## Part 4: Non-blocking MPI Ring Shift

Repeat Part 3 but using non-blocking communication via MPI_Isend() and MPI_Irecv(). Compare the results to the blocking case.

Here is the non-blocking plots for MPI Ring shift:

**Non-Blocking Bandwidth for Ring Shift**

![Non-Blocking Bandwidth Ring Shift](https://github.com/cmse822/project-3-mpi-p2p-team-6/blob/main/plots/part4_ringshift_non_blocking_bandwidth.png)

And here is the tabular view of the computed bandwidth and latency.

| Scenario                 | Bandwidth (GB/s) | Latency (s) |
|--------------------------|------------------|-------------|
| Ring Shift Non-Blocking  |       2.40GB/s   |   2.70E-5   |

The results are somewhat similar to the bl;ocking case in part 3 from a visual perspective. Each of the lines (indicating the number of processes in the ring) increase in bandwidth as the data size gets larger. The lower rank numbers again did better then the higher rank numbers. However, the difference here is that the bandwidth is higher. Latency still remained around the same for both non-blocking and blocking, varying slightly.

## What to turn-in

To your git project repo, commit your final working code for the above exercises and a concise write-up including all plots, and detailed responses to the questions posed concerning your results.

## Notes on how to run

### Modules to load
Follow this process:
```sh
module purge
module load intel/2020a
module load CMake/3.16.4
```


### Building with cmake
```sh
mkdir build
cd build
cmake ..
make
make install
```
