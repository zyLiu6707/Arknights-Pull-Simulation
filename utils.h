#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#include <random>

// calculate the time interval between start and end
// return the result in second
double calc_time(const struct timespec& start, const struct timespec& end) {
  double start_sec =
      (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
  double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

  if (end_sec < start_sec) {
    return 0;
  } else {
    return (end_sec - start_sec) / 1000000000.0;
  }
}

auto get_random_seed() {
  std::random_device rd;  // uses RDRND or /dev/urandom
                          // if you want use a specific token, do not use
                          // "/dev/random" on Linux, which is much slower
  std::mt19937_64 mt64(rd());
  auto random_seed = mt64();
  return random_seed;
}

// TODO: finish this!
void process_command_line_input() {}

#endif