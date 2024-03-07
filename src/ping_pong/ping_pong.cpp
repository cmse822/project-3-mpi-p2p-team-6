#include "common.hpp"
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
    file << "DataSize(B),AvgPingPong(ms),Bandwidth(GB/s),Latency(ms)\n";
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

  std::string OUTPUT_REPORT_PATH = "pingpong.csv";
  if (argc > 3) {
    OUTPUT_REPORT_PATH = argv[3];
  }

  MPI_Init(&argc, &argv);

  int size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // PingPong requires exactly 2 MPI ranks
  if (size != 2) {
    g_printf("This program requires exactly 2 MPI ranks\n");
    MPI_Finalize();
    return 1;
  }

  g_printf("-----------------\n");
  g_printf("|PingPong Session|\n");
  g_printf("-----------------\n");

  g_printf("\t>DATA_SIZE: %fMB\n", data_size_to_MB(DATA_SIZE));
  g_printf("\t>NUM_EXCHANGE: %d\n", NUM_EXCHANGES);

  int ping = 0;
  int pong = 1;

  // Allocate memory for the data
  char *data = new char[DATA_SIZE];

  std::vector<double> pingpong_time_vec(NUM_EXCHANGES);

  for (int i = 0; i < NUM_EXCHANGES; i++) {

    if (rank == ping) {

      const auto &start_time = get_time();

      MPI_Send(data, DATA_SIZE, MPI_BYTE, pong, 0, MPI_COMM_WORLD);
      MPI_Recv(data, DATA_SIZE, MPI_BYTE, pong, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

      const auto &end_time = get_time();

      const auto &delta_time = end_time - start_time;
      pingpong_time_vec[i] = delta_time;

    } else if (rank == pong) {

      MPI_Recv(data, DATA_SIZE, MPI_BYTE, ping, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      MPI_Send(data, DATA_SIZE, MPI_BYTE, ping, 0, MPI_COMM_WORLD);
    }
  }

  // Synchronize all MPI ranks
  MPI_Barrier(MPI_COMM_WORLD);

  // Report the results
  g_printf("-----------------\n");
  g_printf("|PingPong Report|\n");
  g_printf("-----------------\n");

  if (rank == ping) {
    const auto &avg_pingpong =
        std::reduce(pingpong_time_vec.begin(), pingpong_time_vec.end()) /
        NUM_EXCHANGES;

    const auto &data_size_GB = data_size_to_GB(DATA_SIZE);
    const auto &bandwidth = data_size_GB / avg_pingpong;
    const auto &latency = avg_pingpong / 2.0;

    const auto &latency_ms = seconds_to_milliseconds(latency);
    const auto &avg_pingpong_ms = seconds_to_milliseconds(avg_pingpong);

    g_printf("\t> Average PingPong time: %fms\n",
             seconds_to_milliseconds(avg_pingpong));
    g_printf("\t> Bandwidth: %fGB/s\n", bandwidth);
    g_printf("\t> Latency: %fms\n", seconds_to_milliseconds(latency));

    // Append the results to a CSV file
    std::string data = std::to_string(DATA_SIZE) + "," +
                       std::to_string(avg_pingpong_ms) + "," +
                       std::to_string(bandwidth) + "," +
                       std::to_string(latency_ms) + "\n";
    append_to_csv(OUTPUT_REPORT_PATH, data);
    g_printf("\t> Results appended to %s\n", OUTPUT_REPORT_PATH.c_str());
  }

  MPI_Finalize();
  return 0;
}
