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

   ![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

   **Blocking - Multiple Nodes**

   ![Blocking Multiple Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/f6a53d6f-77d3-4a7f-b637-ac58d949bb34)

   Additionally, we provided the bandwidth and latency graphs below

   **Blocking Bandwidth Over Message Sizes - Single Node**

   ![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

   **Blocking Latency Over Message Sizes - Single Node**

   ![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

   **Blocking Bandwidth Over Message Sizes - Multiple Nodes**

   ![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

   **Blocking Latency Over Message Sizes - Multiple Nodes**

   ![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

   And here is the chart for the two scenarios displaying the estimated latency and bandwidth.

   | Scenario                 | Bandwidth (GB/s) | Latency (s) |
   |--------------------------|------------------|-------------|
   | Blocking Same Node       |       4GB/s      |             |
   | Blocking Multiple Nodes  |       2GB/s      |             |

   To answer the question, yes, they are different. The bandwidth is nearly half that on the same node versus different nodes, which makes sense since there's no network communication overhead (latency, congestion, infrastructure limitations).

6. Analyze and discuss your results. Explain the behavior of the resulting curves.

   The latency and average ping plots seems to be very similar if not exactly the same. They are about the same for both singular and multiple nodes as well. The real difference between one and multiple nodes appears in the bandwidth. Multiple nodes seems to have a much smoother curve where singular jumps a lot. Also the single node seems to reach a higher max bandwidth of around 4GB/s where multiple nodes reaches around 2GB/s

## Part 2: Non-block Ping-Pong

Repeat Part 1 using non-blocking MPI communication, i.e., using `MPI_Isend()` and `MPI_Irecv()`. You will need to include explicit process synchronization using, e.g., `MPI_Wait()` calls. Compare the results to the blocking case.

**Non-Blocking - Single Node**

![Non-Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/24263014-cb12-45ff-b136-5bac36bdab93)

**Non-Blocking - Multiple Nodes**

![Non-Blocking Different Nodes](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/63f96857-4e15-4ed9-80f4-7dcd554fe2af)

Again, we provide the non-blocking plots for the bandwidth and latency since we had the data and it could be a useful visual to visualize how bandwidth and latency changes with respect to message size.


**Non-Blocking Bandwidth Over Message Sizes - Single Node**

![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

**Non-Blocking Latency Over Message Sizes - Single Node**

![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

**Non-Blocking Bandwidth Over Message Sizes - Multiple Nodes**

![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

**Non-Blocking Latency Over Message Sizes - Multiple Nodes**

![Blocking Same Node](https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/7824761a-f681-4c06-8e6a-4b274bde679e)

And here is the chart for the two scenarios displaying the estimated latency and bandwidth.

| Scenario                     | Bandwidth (GB/s) | Latency (s) |
|------------------------------|------------------|-------------|
| Non-Blocking Same Node       |       4GB/s      |             |
| Non-Blocking Multiple Nodes  |       2GB/s      |             |


These results appear to be mostly the same as the blocking ping-pong results when just looking at the average ping and latency curves. The major differences come from the bandwidth curves. In the single node curve the bandwidth seems to have a more severe drop before leveling out. The multiple node curve does not appear to have this. Another difference would be the max bandwidth is almost double that of the respective curves from the blocking results.

## Part 3: MPI Ring Shift

1. Implement the MPI ring shift in C or C++ for an arbitrary number of processes in the ring and arbitrary message size (i.e., number of elements per process). In your implementation, use `MPI_Sendrecv()` instead of separate `MPI_Send()` and `MPI_Recv()` calls.

   Done.
2. As in Parts 1 and 2, vary the message size from 2 bytes to 4 kb, in powers of 2. Also vary the number of processes used from 2 to `N`, in powers of 2, where `N` is sufficiently large that rank 0 and rank `N-1` are guaranteed to reside on separate nodes (`N` will depend on which cluster you are using on HPCC).

   Done.
3. Compute the bandwidth and latency, as above. Plot the bandwidth as a function of message size. Include separate lines for each number of processes used.

   Below is our plotted bandwidth graph. Please note that this particular part doesn't really specify whether we have to run on on single node vs multiple nodes, but does state that N needs to big enough to run on multiple nodes. So I am going into this with the assumption that we are going to be communicating across nodes (for the larger N) and not needing to have graphs for solely single nodes. This question also wants us to just plot the bandwidth, so we do just that below:

<img width="500" alt="Screenshot 2024-03-13 at 9 08 42 AM" src="https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/13197ab2-2a1d-4147-8bfb-aebd4404bc85">

   And here is the tabular view of the computed bandwidth and latency.

1. Analyze and discuss your results. Explain the behavior of the resulting curves.

The results appear to be similar to the blocking results from the first part, but with the ring shift there is a range of points at each data size. This also resulted in a very different looking bandwidth curve with it looking to more gradually increase over time before leveling out where before the increase was much more severe.

## Part 4: Non-blocking MPI Ring Shift

Repeat Part 3 but using non-blocking communication via MPI_Isend() and MPI_Irecv(). Compare the results to the blocking case.

<img width="500" alt="Screenshot 2024-03-13 at 9 09 04 AM" src="https://github.com/cmse822/project-3-mpi-p2p-team-6/assets/94200328/ab6afa77-933a-4f3c-9149-2b0babf3ad81">

The results are again similar to the non-blocking results from part 2 but the ring shift gives a range of results at each point in datasize. The bandwidth curve is again less severe of an increase when compared to the curve in part two. Comparing it to the curve in part 3 the range of results at each point in datasize also seems more varied.

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
