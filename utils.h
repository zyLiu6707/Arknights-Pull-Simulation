#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "error_flag.h"

// Pre-defined parameters for Arknights
const double limited_banner_on_banner_star6_conditional_rate = 0.7;
const double regular_banner_on_banner_star6_conditional_rate = 0.5;

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


// int check_and_convert_to_int() {}

// TODO: finish this
void display_help_message() {
  std::cout << "Usage:" << std::endl;
  exit(EXIT_SUCCESS);
}

// void display_error_message(const ErrorFlag& error_flag) {
//   display_help_message();
// }

// TODO: finish this!
void process_cmd_input_and_set_prob_wrapper(int argc, char* argv[], ProbabilityWrapper& probability_wrapper) {
  const int expected_max_arg_num = 6;
  if (argc > expected_max_arg_num) {
    std::cerr << "Too many arguments!" << std::endl;
    display_help_message();
  }
  if (argc == 1) {
    display_help_message();
  }

  // Control arguments are those that can specify the behavior of the
  // program, i.e., display help message, simulate a regular banner or 
  // a limited banner, etc.
  std::unordered_set<std::string> expected_control_arg( {"--help", "-t", "--total-pull-time",
                                                         "--limited", "--regular", "-p", "--pity"} );

  // Store control argument in expected_control_arg as key and the non-control
  // argument follows it as elements in value, which is a vector of string.
  // The content in this map will be checked and conrresponding error flag will
  // be set if the command line input is invalid.
  std::unordered_map<std::string, std::vector<std::string>> arg_map;

  // A wrapper class for different kinds of error flags
  ErrorFlag error_flag;

  // Parse the inputs
  for (int i = 1; i < argc; ++i) {
    if (expected_control_arg.find(argv[i]) != expected_control_arg.end()) {
      if (arg_map.count(argv[i]) == 0) {
        for (int j = i + 1; j < argc; ++j) {
          if (expected_control_arg.find(argv[j]) != expected_control_arg.end()) {
            break;
          }
          arg_map[argv[i]].push_back(argv[j]);
        }
      } 
      else {
        // Situation that exact same control arg appears more than once
        error_flag.err_redundant_identical_ctrl_arg_flag = true;
        //break;
      }
    }
  }
  // If --help is specified, ignore all other arguments and print the help message
  if (arg_map.find("--help") != arg_map.end()) {
    display_help_message();
  }

  // Argument error priority order:
  // too many arg > redundant arg > conflict arg > missing value > invalid value

  // Check another situation of redundant control arguments
  if (arg_map.find("-t") != arg_map.end() &&
      arg_map.find("--total-pull-time") != arg_map.end()) {
    error_flag.err_redundant_total_pull_time_ctrl_arg_flag = true;
  }
  if (arg_map.find("-p") != arg_map.end() &&
      arg_map.find("--pity") != arg_map.end()) {
    error_flag.err_redundant_pity_ctrl_arg_flag = true;
  }

  // Check whether conflict control arguments are provided,
  // i.e., --regular and --limited are both provided
  if (arg_map.find("--regular") != arg_map.end() &&
      arg_map.find("--limited") != arg_map.end()) {
    error_flag.err_conflict_ctrl_arg_flag = true;
  }

  // Check whether missing a specific value for control arguments that need one,
  // i.e., -t/--total-pull-time, -p/--pity
  const auto it_total_pull_time = arg_map.count("-t") == 1 ? arg_map.find("-t") : arg_map.find("--total-pull-time");
  if (it_total_pull_time != arg_map.cend() && it_total_pull_time->second.size() == 0) {
    error_flag.err_missing_value_for_ctrl_arg_total_pull_time_flag = true;
  }
  const auto it_pity = arg_map.count("-p") == 1 ? arg_map.find("-p") : arg_map.find("--pity");
  if (it_pity != arg_map.cend() && it_pity->second.size() == 0) {
    error_flag.err_missing_value_for_ctrl_arg_total_pull_time_flag = true;
  }

  // Check the format of specific value for control arguments that need one,
  // i.e., -t/--total-pull-time, -p/--pity
  // the specific values are required to be positive integer; If the format is
  // correct, then retrieve the argument value
  long int total_pull_time = -1;
  if (it_total_pull_time != arg_map.cend()) {
    if (it_total_pull_time->second.size() > 1) {
      error_flag.err_invalid_value_for_ctrl_arg_total_pull_time_flag = true;
    } else {
      char* p_end = nullptr;
      total_pull_time = strtol(it_total_pull_time->second[0].c_str(), &p_end, 10);
      if (*p_end != '\0' || total_pull_time < 0) {
        error_flag.err_invalid_value_for_ctrl_arg_total_pull_time_flag = true;
      }
    }
  }
  long int pity_starting = -1;
  if (it_pity != arg_map.cend()) {
    if (it_pity->second.size() > 1) {
      error_flag.err_invalid_value_for_ctrl_arg_pity_flag = true;
    } else {
      char* p_end = nullptr;
      pity_starting = strtol(it_pity->second[0].c_str(), &p_end, 10);
      if (*p_end != '\0' || pity_starting < 0) {
        error_flag.err_invalid_value_for_ctrl_arg_pity_flag = true;
      }
    }
  }

  // TODO: deal with different errors here
  // display_error_message(error_flag);

  // Command line input is valid, now set the ProbabilityWrapper
  if (arg_map.find("--regular") != arg_map.end()) {
    probability_wrapper.set_on_banner_star6_conditional_rate(
        limited_banner_on_banner_star6_conditional_rate);
  } else if (arg_map.find("--limited") != arg_map.end()) {
    probability_wrapper.set_on_banner_star6_conditional_rate(
        regular_banner_on_banner_star6_conditional_rate);
  }
}

#endif  // UTILS_H