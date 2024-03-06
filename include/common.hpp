#pragma once

#include <iostream>
#include <mpi.h>
#include <stdarg.h>

int rank;

void g_printf(const char *fmt, ...) {

    if (rank != 0) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

double seconds_to_milliseconds(double seconds) { return seconds * 1000; }

double data_size_to_GB(int data_size_bytes) {
  return (double)data_size_bytes / (1024 * 1024 * 1024);
}

double data_size_to_MB(int data_size_bytes) {
  return (double)data_size_bytes / (1024 * 1024);
}

double get_time() { return MPI_Wtime(); }