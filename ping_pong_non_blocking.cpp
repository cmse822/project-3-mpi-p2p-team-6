#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <unistd.h>
#include <vector>
#include <numeric>
#include <fstream>
#include <filesystem>

void append_to_csv(const std::string& filename, const std::string& data) {

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

double get_time() {
	return MPI_Wtime();
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

	std::string OUTPUT_REPORT_PATH = "pingpong_nonblocking.csv";
	if (argc > 3) {
		OUTPUT_REPORT_PATH = argv[3];
	}


  MPI_Init(&argc, &argv);
  
	int rank;
  int size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Synchronize all MPI ranks
	MPI_Barrier(MPI_COMM_WORLD);

	// PingPong requires exactly 2 MPI ranks
	if (size != 2) {
		if (rank == 0) {
			printf("This program requires exactly 2 MPI ranks\n");
		}
		MPI_Finalize();
		return 1;
	}

	if(rank == 0) {
		printf("-----------------\n");
		printf("|PingPong Session|\n");
		printf("-----------------\n");
		
		printf("\t>DATA_SIZE: %fMB\n", data_size_to_MB(DATA_SIZE));
		printf("\t>NUM_EXCHANGE: %d\n", NUM_EXCHANGES);
	}


  int ping = 0;
  int pong = 1;

	// Allocate memory for the data
  char *data = new char[DATA_SIZE];

	std::vector<double> pingpong_time_vec(NUM_EXCHANGES);
    MPI_Request request;

	for (int i = 0; i < NUM_EXCHANGES; i++) {

		if (rank == ping) {
		
			const auto& start_time = get_time();
			
			MPI_Isend(data, DATA_SIZE, MPI_BYTE, pong, 0, MPI_COMM_WORLD, &request);
			MPI_Irecv(data, DATA_SIZE, MPI_BYTE, pong, 0, MPI_COMM_WORLD, &request);
			
			const auto& end_time = get_time();
			
			const auto& delta_time = end_time - start_time;
			pingpong_time_vec[i] = delta_time;
		
		} else if (rank == pong) {
		
			MPI_Irecv(data, DATA_SIZE, MPI_BYTE, ping, 0, MPI_COMM_WORLD, &request);
			MPI_Isend(data, DATA_SIZE, MPI_BYTE, ping, 0, MPI_COMM_WORLD, &request);
		}

		MPI_Wait(&request, MPI_STATUS_IGNORE);

	}
		
	// Synchronize all MPI ranks
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == ping) {
		const auto& avg_pingpong = std::reduce(pingpong_time_vec.begin(), pingpong_time_vec.end()) / NUM_EXCHANGES;

		// Report the results
		printf("-----------------\n");
		printf("|PingPong Report|\n");
		printf("-----------------\n");

		const auto& data_size_GB = data_size_to_GB(DATA_SIZE);
		const auto& bandwidth = data_size_GB / avg_pingpong;
		const auto& latency = avg_pingpong / 2.0f;


		printf("\t> Average PingPong time: %f\n", avg_pingpong);
		printf("\t> Bandwidth: %fGB/s\n", bandwidth);
		printf("\t> Latency: %fs\n", latency);

		// Append the results to a CSV file
		std::string data = std::to_string(data_size_GB) + "," + std::to_string(avg_pingpong) + "," + std::to_string(bandwidth) + "," + std::to_string(latency) + "\n";
		append_to_csv(OUTPUT_REPORT_PATH, data);
		printf("\t> Results appended to %s\n", OUTPUT_REPORT_PATH.c_str());
	}


  MPI_Finalize();
  return 0;
}
