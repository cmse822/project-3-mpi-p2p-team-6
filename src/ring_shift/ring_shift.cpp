#include "common.hpp"
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mpi.h>
#include <numeric>
#include <unistd.h>
#include <vector>

void append_to_csv(const std::string &filename, const std::string &data) {

  // Create the file if it does not exist
  if (!std::filesystem::exists(filename)) {
    std::ofstream file;
    file.open(filename);
    file << "DataSize(MB),DataSizePerRank(MB),NumMPIRanks,MeanRingShiftTime(ms)"
            ",Bandwidth(GB/s),Latency(ms)\n";
    file.close();
  }

  std::ofstream file;
  file.open(filename, std::ios_base::app);
  file << data;
  file.close();
}

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

  int world_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // RingShift requires minimum 2 MPI ranks
  if (world_size < 2) {

    g_printf("This program requires at least 2 MPI ranks\n");
    MPI_Finalize();
    return 0;
  }

  if (DATA_SIZE < world_size) {

    g_printf("DATA_SIZE should be greater than or equal to the number of MPI "
             "ranks\n");
    MPI_Finalize();
    return 0;
  }

  if (rank == 0) {
    const auto &modified_data_size = DATA_SIZE - (DATA_SIZE % world_size);
    DATA_SIZE = modified_data_size;
  }

  g_printf("-----------------\n");
  g_printf("|RingShift Session|\n");
  g_printf("-----------------\n");

  g_printf("\t>DATA_SIZE: %fMB\n", data_size_to_MB(DATA_SIZE));
  g_printf("\t>NUM_EXCHANGE: %d\n", NUM_EXCHANGES);
  g_printf("\t>POSITION_SHIFT: %d\n", POSITION_SHIFT);
  g_printf("\t>NUM_MPI_RANKS: %d\n", world_size);

  const int &data_size_per_rank = DATA_SIZE / world_size;
  g_printf("\t>DATA_SIZE_PER_RANK: %fMB\n",
           data_size_to_MB(data_size_per_rank));

  char *data;
  if (rank == 0) {
    data = new char[DATA_SIZE];
  }

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // Scatter the data to all MPI ranks
  char *sub_data = new char[data_size_per_rank];

  MPI_Scatter(data, data_size_per_rank, MPI_BYTE, sub_data, data_size_per_rank,
              MPI_BYTE, 0, MPI_COMM_WORLD);

  // Individual RingShift times per MPI rank
  std::vector<double> ringshift_time_vec(NUM_EXCHANGES);

  // Calculate the next and previous MPI rank
  int next = (rank + 1) % world_size;
  int prev = (rank - 1 + world_size) % world_size;

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
  std::vector<double> all_ringshift_time_vec(world_size);
  MPI_Gather(&mean_time, 1, MPI_DOUBLE, all_ringshift_time_vec.data(), 1,
             MPI_DOUBLE, 0, MPI_COMM_WORLD);

  g_printf("---------------------\n");
  g_printf("|RingShift completed|\n");
  g_printf("---------------------\n");

  if (rank == 0) {

    const auto &mean_exec_time =
        std::accumulate(all_ringshift_time_vec.begin(),
                        all_ringshift_time_vec.end(), 0.0) /
        all_ringshift_time_vec.size();
    const auto &bandwidth =
        data_size_to_GB(data_size_per_rank) / mean_exec_time;
    const auto &latency = mean_exec_time / 2;

    const auto &data_size_mb = data_size_to_MB(DATA_SIZE);
    const auto &data_size_per_rank_mb = data_size_to_MB(data_size_per_rank);

    const auto &latency_ms = seconds_to_milliseconds(latency);
    const auto &mean_exec_time_ms = seconds_to_milliseconds(mean_exec_time);

    const auto &output_data = std::to_string(data_size_mb) + "," +
                              std::to_string(data_size_per_rank_mb) + "," +
                              std::to_string(world_size) + "," +
                              std::to_string(mean_exec_time_ms) + "," +
                              std::to_string(bandwidth) + "," +
                              std::to_string(latency_ms) + "\n";
    append_to_csv(OUTPUT_REPORT_PATH, output_data);

    g_printf("\t> Mean RingShift Time: %fms\n", mean_exec_time_ms);
    g_printf("\t> Bandwidth: %fGB/s\n", bandwidth);
    g_printf("\t> Latency: %fms\n", latency_ms);
  }

  MPI_Finalize();
  return 0;
}