#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <numeric>
#include <unistd.h>
#include <vector>

void append_to_csv(const std::string &filename, const std::string &data) {

  // Create the file if it does not exist
  if (!std::filesystem::exists(filename)) {
    std::ofstream file;
    file.open(filename, std::ios_base::app);
    file << "DataSize(GB),AvgPingPong(s),Bandwidth(GB/s),Latency(s)\n";
    file.close();
  }

  std::ofstream file;
  file.open(filename, std::ios_base::app);
  file << data;
  file.close();
}

double data_size_to_GB(int data_size_bytes) {
  return (double)data_size_bytes / (1024 * 1024 * 1024);
}

double data_size_to_MB(int data_size_bytes) {
  return (double)data_size_bytes / (1024 * 1024);
}

double get_time() { return MPI_Wtime(); }

int main(int argc, char *argv[]) {

  // Get the DATA_SIZE from the argument list
  int DATA_SIZE = 10000;

  if (argc > 1) {
    DATA_SIZE = atoi(argv[1]);
  }

  // Get the NUM_EXCHANGE from the argument list
  int NUM_EXCHANGES = 10000;
  if (argc > 2) {
    NUM_EXCHANGES = atoi(argv[2]);
  }

  // Get the POSITION_SHIFT from the argument list
  int POSITION_SHIFT = 1;
  if (argc > 3) {
    POSITION_SHIFT = atoi(argv[3]);
  }

  std::string OUTPUT_REPORT_PATH = "ring_shift.csv";
  if (argc > 4) {
    OUTPUT_REPORT_PATH = argv[4];
  }

  MPI_Init(&argc, &argv);

  int rank;
  int size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // RingShift requires exactly 2 MPI ranks
  if (size < 2) {
    if (rank == 0) {
      printf("This program requires at least 2 MPI ranks\n");
    }
    MPI_Finalize();
    return 1;
  }

  if (rank == 0) {
    printf("-----------------\n");
    printf("|RingShift Session|\n");
    printf("-----------------\n");

    printf("\t>DATA_SIZE: %fMB\n", data_size_to_MB(DATA_SIZE));
    printf("\t>NUM_EXCHANGE: %d\n", NUM_EXCHANGES);
    printf("\t>POSITION_SHIFT: %d\n", POSITION_SHIFT);
    printf("\t>NUM_MPI_RANKS: %d\n", size);

     // Allocate memory for the data
    if (rank == 0) {
      printf("\t\t? DATA_SIZE must be divisible by the number of MPI ranks\n");

      const auto &modified_data_size = DATA_SIZE - (DATA_SIZE % size);

      printf("\t\t? Changing DATA_SIZE from %d to %d\n", DATA_SIZE,
             modified_data_size);
      DATA_SIZE = modified_data_size;
    }
    printf("\t>DATA_SIZE: %fMB\n", data_size_to_MB(DATA_SIZE));
  }

  const int &data_size_per_rank = DATA_SIZE / size;
  char *data;
  if (rank == 0) {
    data = new char[DATA_SIZE];
  }

  char* sub_data = new char[data_size_per_rank];

  MPI_Scatter(data, data_size_per_rank, MPI_BYTE, sub_data, data_size_per_rank,
              MPI_BYTE, 0, MPI_COMM_WORLD);

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  std::vector<double> ringshift_time_vec(NUM_EXCHANGES);

  int next = (rank + 1) % size;
  int prev = (rank - 1 + size) % size;

  for (int i = 0; i < NUM_EXCHANGES; i++) {

    const auto &start_time = get_time();

    MPI_Send(sub_data, data_size_per_rank, MPI_BYTE, next, 0, MPI_COMM_WORLD);
    MPI_Recv(sub_data, data_size_per_rank, MPI_BYTE, prev, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);

    const auto &end_time = get_time();

    const auto &delta_time = end_time - start_time;
    ringshift_time_vec[i] = delta_time;
  }

  const auto &mean_time = std::accumulate(ringshift_time_vec.begin(),
                                          ringshift_time_vec.end(), 0.0) /
                          ringshift_time_vec.size();

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // Get mean of mean_time taken by all MPI ranks
  std::vector<double> all_ringshift_time_vec(size);
  MPI_Gather(&mean_time, 1, MPI_DOUBLE, all_ringshift_time_vec.data(), 1,
             MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0) {

    printf("RingShift completed\n");
    // print all_ringshift_time_vec with bracets
    std::cout << "All RingShift Time: [";
    for (int i = 0; i < all_ringshift_time_vec.size(); i++) {
      std::cout << all_ringshift_time_vec[i] << ", ";
    }
    std::cout << "]\n";

    const auto &mean_exec_time =
        std::accumulate(all_ringshift_time_vec.begin(),
                        all_ringshift_time_vec.end(), 0.0) /
        all_ringshift_time_vec.size();
    const auto &bandwidth = data_size_to_GB(data_size_per_rank) / mean_exec_time;
    const auto &latency = mean_exec_time / 2;

    printf("\t> Mean RingShift Time: %f\n", mean_exec_time);
    printf("\t> Bandwidth: %fGB/s\n", bandwidth);
    printf("\t> Latency: %fs\n", latency);
  }

  MPI_Finalize();
  return 0;
}