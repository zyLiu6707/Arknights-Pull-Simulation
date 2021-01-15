#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <time.h>

#include <algorithm>  // min
#include <cctype>     // isdigit
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "error_flag.h"
#include "probability_wrapper.h"

// Pre-defined parameters for Arknights
const double limited_banner_on_banner_star6_conditional_rate = 0.7;
const double standard_banner_on_banner_star6_conditional_rate = 0.5;
// Numbers of pulls that you are guaranteed to get a star6 operator
// it is equals to (100% - 2%)/2%
//      ~~~~~~~~~~~~~^     ^   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      100% chance of    base rate            amount of increased rate
//     getting a star6    of getting star6     after a failed pull
const unsigned long long int steps_to_guaranteed_star6 = 49;

const unsigned long long int max_pity_starting_point =
    4294967295;  // DO NOT USE ULONG_MAX - it can be (2^64 - 1) on some platform

// Pre-defined parameters for displaying the results
// Maximum number of raw data to show
const size_t raw_data_showing_limit = 100;
// Maximum number of rare event to show
const size_t rare_event_showing_limit = 10;
// Maximum number of estimated probability to show
const size_t estimated_prob_showing_limit = 1000;

/* For debugging purpose */
// Print the content in arg_map
#ifdef DEBUG
void print_arg_map_contents(
    const std::unordered_map<std::string, std::vector<std::string>>& arg_map) {
  std::cout << "\nArguments Map:\n";
  std::cout << "Map Size = " << arg_map.size() << std::endl;
  std::cout << "Key\t\tValue\n";
  for (auto it_kv = arg_map.cbegin(); it_kv != arg_map.cend(); ++it_kv) {
    std::cout << it_kv->first << "\t\t";
    for (auto& s : it_kv->second) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }
}
#endif

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

// Generate a random number using random_device to make sure the seed
// is varied from different times you run the Monte Carlo simulation
uint_fast64_t get_random_seed() {
  std::random_device rd;  // uses RDRND or /dev/urandom
                          // if you want use a specific token, do not use
                          // "/dev/random" on Linux, which is much slower
  std::mt19937_64 mt64(rd());
  auto random_seed = mt64();
  return random_seed;
}


// Display the help message
void display_help_message() {
  std::cout << "Usage: [--help] [-t|--total-pull-time <value>] [--standard|--limited] [-p|--pity <value>] [-n|--num-rate-up <value>] [-c|--current-pull <value>]\n\n"
               "--help : Display the help message\n"
               "-t|--total-pull-time : Set the time of pulling in a simulation\n"
               "                        Valid value is an integer between [1, 18446744073709551615] (inclusive) on Linux 64bit/C++11\n"
               "                        Note : This is not the experiment time\n"
               "                        Note : If you provide a number greather than 18446744073709551615, the\n"
               "                               program will run the simulation with maximum valid times (i.e., 18446744073709551615)\n"
               "            --standard : Simulation and get the estimated probability in a standard banner\n"
               "                        Cannot be specified with --limited at the same time\n"
               "            --limited : Simulation and get the estimated probability in a limited banner\n"
               "                        Cannot be specified with --standard at the same time\n"
               "               --pity : Set the starting point where the pity system comes into effect,\n"
               "                        i.e., you will get a higher probability on the specified pull's next pull\n"
               "                        Valid value is an integer between [0, 4294967295] on Linux 64bit/C+11\n"
               "                        Note: If you provide a number greater than 4294967295, the program will run the\n"
               "                              simulation with maximum valid value for pity starting time (i.e., 4294967295)\n"
               "     -n|--num-rate-up : Set the number of operator(s) that currently rate up.\n"
               "                        The valid values are 1 and 2\n"
               "    -c|--current-pull : Set how many times that you have already pulled without getting a star-6 operator\n"
               "                        Valid value is an integer between [0, <-p|--pity value> + 49) (inclusive, exclusive)\n"
               "Note that the order of these arguments does not matter.\n"
               << std::endl;

  return;
}

// Show the corresponding error message according to the flags in ErrorFlag
void display_error_detail(const ErrorFlag& error_flag) {
  if (error_flag.check_err()) {
    std::cerr << "The provided command line arguments are invalid due to the following error(s):\n";
    if (error_flag.err_help_ctrl_arg_with_other_args) {
      std::cerr << "\tOther argument(s) is(are) found with \"--help\"\n";
      std::cerr << "If you would like to see help message, please ony specify \"--help\"\n";
      std::cerr << "Other arguments are ignored. Will not run the simulation.\n";
      // Ignore other args and potential errors, aborting the program
      return;
    }
    if (error_flag.err_invalid_ctrl_args) {
      std::cerr << "\tInvalid control arguments: ";
      for (const auto& invalid_arg : error_flag.invalid_ctrl_args_list) {
        std::cerr << invalid_arg << " ";
      }
      std::cerr << std::endl;
    }
    if (error_flag.err_unexpected_arguments_at_the_beginning) {
      std::cerr << "\tUnexpected argument(s) at the beginning\n";
    }
    // Redundant argument
    if (error_flag.err_redundant_identical_ctrl_arg) {
      std::cerr << "\tSame arguments are specified more than once\n";
    }
    if (error_flag.err_redundant_total_pull_time_ctrl_arg) {
      std::cerr << "\tRedundant arguments: \"-t\" and \"--total-pull-time\" are specified at the same time\n";
    }
    if (error_flag.err_redundant_pity_ctrl_arg) {
      std::cerr << "\tRedundant arguments: \"-p\" and \"--pity\" are specified at the same time\n";
    }
    if (error_flag.err_redundant_num_rate_up_ctrl_arg) {
      std::cerr << "\tRedundant arguments: \"-n\" and \"--num-rate-up\" are specified at the same time\n";
    }
    if (error_flag.err_redundant_current_pull_ctrl_arg) {
      std::cerr << "\tRedundant arguments: \"-c\" and \"--current-pull\" are specified at the same time\n";
    }
    // Conflict arguments
    if (error_flag.err_conflict_ctrl_arg_flag) {
      std::cerr << "\tConflict arguments: \"--standard\" and \"--limited\" are specified at the same time\n";
    }
    // Missing detail value
    if (error_flag.err_missing_value_for_total_pull_time_ctrl_arg) {
      std::cerr << "\tMissing value for \"-t\"\n";
    }
    if (error_flag.err_missing_value_for_total_pull_time_long_name_ctrl_arg) {
      std::cerr << "\tMissing value for \"--total-pull-time\"\n";
    }
    if (error_flag.err_missing_value_for_pity_ctrl_arg) {
      std::cerr << "\tMissing value for \"-p\"\n";
    }
    if (error_flag.err_missing_value_for_pity_long_name_ctrl_arg) {
      std::cerr << "\tMissing value for \"--pity\"\n";
    }
    if (error_flag.err_missing_value_for_num_rate_up_ctrl_arg) {
      std::cerr << "\tMissing value for \"-n\"\n";
    }
    if (error_flag.err_missing_value_for_num_rate_up_long_name_ctrl_arg) {
      std::cerr << "\tMissing value for \"--num-rate-up\"\n";
    }
    if (error_flag.err_missing_value_for_current_pull_ctrl_arg) {
      std::cerr << "\tMissing value for \"-c\"\n";
    }
    if (error_flag.err_missing_value_for_current_pull_long_name_ctrl_arg) {
      std::cerr << "\tMissing value for \"--current-pull\"\n";
    }
    // Invalid value
    if (error_flag.err_invalid_value_for_total_pull_time_ctrl_arg) {
      std::cerr << "\tInvalid value for \"-t\" - it must be a positive integer\n";
    }
    if (error_flag.err_invalid_value_for_total_pull_time_long_name_ctrl_arg) {
      std::cerr << "\tInvalid value for \"--total-pull-time\" - it must be a positive integer\n";
    }
    if (error_flag.err_invalid_value_for_pity_ctrl_arg) {
      std::cerr << "\tInvalid value for \"-p\" - it must be a non-negative integer\n";
    }
    if (error_flag.err_invalid_value_for_pity_long_name_ctrl_arg) {
      std::cerr << "\tInvalid value for \"--pity\" - it must be a non-negative integer\n";
    }
    if (error_flag.err_invalid_value_for_num_rate_up_ctrl_arg) {
      std::cerr << "\tInvalid value for \"-n\" - it must be a 1 or 2\n";
    }
    if (error_flag.err_invalid_value_for_num_rate_up_long_name_ctrl_arg) {
      std::cerr << "\tInvalid value for \"--num-rate-up\" - it must be a 1 or 2\n";
    }
    if (error_flag.err_invalid_value_for_current_pull_ctrl_arg) {
      std::cerr << "\tInvalid value for \"-c\" - it must be an integer between [0, <-p|--pity value> + 49) (inclusive, exclusive)\n";
    }
    if (error_flag.err_invalid_value_for_current_pull_long_name_ctrl_arg) {
      std::cerr << "\tInvalid value for \"--current-pull\" - it must be an integer between [0, <-p|--pity value> + 49) (inclusive, exclusive)\n";
    }
    // Unexpected value
    if (error_flag.err_unexpected_value_for_ctrl_arg_standard) {
      std::cerr << "\tUnexpected value for \"--standard\"\n";
    }
    if (error_flag.err_unexpected_value_for_ctrl_arg_limited) {
      std::cerr << "\tUnexpected value for \"--limited\"\n";
    }
    std::cerr << "Please check and correct the error(s)\nYou can refer to help message, README, or visit the online repo:\n"
                 "https://github.com/zyLiu6707/Arknights-Gacha-Simulation\n" << std::endl;

    display_help_message();
  }
}

// Read the command line input, check the potential errors in the input.
// If the input is valid, set the corresponding field in ProbabilityWrapper
// and other variables that will be used during the simulation. Otherwise, raise
// the corresponding flags in ErrorFlag and show the appropriate message. 
// Return a bool flag with true value if the input is valid, which will indicates
// the main simulation program whether can continue running
bool process_cmd_input_and_set_corres_var(
    int argc, char* argv[], ProbabilityWrapper& probability_wrapper,
    unsigned long long int& total_pull_time,
    unsigned int& pity_starting_point, unsigned long long int& current_pull) {
  const int expected_max_arg_num = 10;
  if (argc > expected_max_arg_num) {
    std::cerr << "\nToo many arguments!\n" << std::endl;
    display_help_message();
    return false;
  }
  if (argc == 1) {
    std::cerr << "\nNote: No arguments are provided. Will simulate a double-rate-up limited banner.\n" << std::endl;
    return true;
  }

  // Control arguments are those that can specify the behavior of the
  // program, i.e., display help message, simulate a standard banner or
  // a limited banner, etc. Starts with at lease one dash '-'
  std::unordered_set<std::string> expected_control_arg(
      {"--help", "-t", "--total-pull-time", "--limited", "--standard", "-p",
       "--pity", "-n", "--num-rate-up", "-c", "--current-pull"});

  // Store control argument as arg_map's key, and ctrl arg's value as arg_map's
  // value. 
  // The content in this map will be checked and conrresponding error
  // flag will be set if corresponding type of error is detected.
  std::unordered_map<std::string, std::vector<std::string>> arg_map;

  // A wrapper class for different kinds of error flags
  ErrorFlag error_flag;

  // Parse the inputs
  int curr_ctrl_arg_idx = 0;
  bool skip_identical_ctrl_arg = false;
  for (int i = 1; i < argc; ++i) {
    // Starts with a '-' and the second char (if exists) is not a number,
    // then we regards this is an (attempted) control arugments
    // Note: isdigit will regard the '\0' in argv as a non-number character
    if (argv[i][0] == '-' && !isdigit(argv[i][1])) {
      if (arg_map.count(argv[i]) > 0) {
        skip_identical_ctrl_arg = true;
        if (expected_control_arg.find(argv[i]) != expected_control_arg.end()) {
          error_flag.err_redundant_identical_ctrl_arg = true;
        }
        continue;
      }
      arg_map[argv[i]];
      curr_ctrl_arg_idx = i;
      skip_identical_ctrl_arg = false;
    } else if (!skip_identical_ctrl_arg &&
               arg_map.size() > 0)  // to avoid the case like "./<prog_name> 2"
                                    // that will insert the value 2 into arg_map
    {
      arg_map[argv[curr_ctrl_arg_idx]].push_back(argv[i]);
    }
  }

#ifdef DEBUG  // this function will be executed when debugging
  print_arg_map_contents(arg_map);
#endif

  // If --help is specified, ignore all other arguments and print the help
  // message
  if (arg_map.find("--help") != arg_map.end()) {
    if (argc == 2) {
      display_help_message();
      return false;
    } else {
      // If --help and other arguments are specified at the same time, show the
      // appropriate message
      error_flag.err_help_ctrl_arg_with_other_args = true;
    }
  }

  // Now check whether the command line arguments are invalid
  const auto iter_total_pull_time = arg_map.find("-t");
  const auto iter_total_pull_time_long_name = arg_map.find("--total-pull-time");
  const auto iter_pity = arg_map.find("-p");
  const auto iter_pity_long_name = arg_map.find("--pity");
  const auto iter_num_rate_up = arg_map.find("-n");
  const auto iter_num_rate_up_long_name = arg_map.find("--num-rate-up");
  const auto iter_current_pull = arg_map.find("-c");
  const auto iter_current_pull_long_name = arg_map.find("--current-pull");

  // Check whether the first argument (i.e., argv[1]) is an unexpected arg
  if (argv[1][0] != '-' || isdigit(argv[1][1])) {
    error_flag.err_unexpected_arguments_at_the_beginning = true;
  }
  // Check whether there is invalid control arg
  for (const auto& p : arg_map) {
    if (expected_control_arg.find(p.first) == expected_control_arg.end()) {
      error_flag.err_invalid_ctrl_args = true;
      error_flag.invalid_ctrl_args_list.push_back(p.first);
    }
  }
  // Check whether the short name and the long name of the same type of ctrl
  // arg are specified at the same time
  if (iter_total_pull_time != arg_map.end() &&
      iter_total_pull_time_long_name != arg_map.end()) {
    error_flag.err_redundant_total_pull_time_ctrl_arg = true;
  }
  if (iter_pity != arg_map.end() && iter_pity_long_name != arg_map.end()) {
    error_flag.err_redundant_pity_ctrl_arg = true;
  }
  if (iter_num_rate_up != arg_map.end() &&
      iter_num_rate_up_long_name != arg_map.end()) {
    error_flag.err_redundant_num_rate_up_ctrl_arg = true;
  }
  if (iter_current_pull != arg_map.end() &&
      iter_current_pull_long_name != arg_map.end()) {
    error_flag.err_redundant_current_pull_ctrl_arg = true;
  }

  // Check whether conflict control arguments are provided,
  // i.e., --standard and --limited are both provided
  if (arg_map.find("--standard") != arg_map.end() &&
      arg_map.find("--limited") != arg_map.end()) {
    error_flag.err_conflict_ctrl_arg_flag = true;
  }

  // Check whether missing a specific value for control arguments that expect one,
  // i.e., -t/--total-pull-time, -p/--pity, -n/--num-rate-up and -c/--current-pull
  if (iter_total_pull_time != arg_map.cend() &&
      iter_total_pull_time->second.size() == 0) {
    error_flag.err_missing_value_for_total_pull_time_ctrl_arg = true;
  } else if (iter_total_pull_time_long_name != arg_map.cend() &&
             iter_total_pull_time_long_name->second.size() == 0) {
    error_flag.err_missing_value_for_total_pull_time_long_name_ctrl_arg = true;
  }

  if (iter_pity != arg_map.cend() && iter_pity->second.size() == 0) {
    error_flag.err_missing_value_for_pity_ctrl_arg = true;
  } else if (iter_pity_long_name != arg_map.cend() &&
             iter_pity_long_name->second.size() == 0) {
    error_flag.err_missing_value_for_pity_long_name_ctrl_arg = true;
  }

  if (iter_num_rate_up != arg_map.cend() &&
      iter_num_rate_up->second.size() == 0) {
    error_flag.err_missing_value_for_num_rate_up_ctrl_arg = true;
  } else if (iter_num_rate_up_long_name != arg_map.cend() &&
             iter_num_rate_up_long_name->second.size() == 0) {
    error_flag.err_missing_value_for_num_rate_up_long_name_ctrl_arg = true;
  }

  if (iter_current_pull != arg_map.cend() &&
      iter_current_pull->second.size() == 0) {
    error_flag.err_missing_value_for_current_pull_ctrl_arg = true;
  } else if (iter_current_pull_long_name != arg_map.cend() &&
             iter_current_pull_long_name->second.size() == 0) {
    error_flag.err_missing_value_for_current_pull_long_name_ctrl_arg = true;
  }

  // Check the format of specific value for control arguments that expect one.
  // If the format is correct, then retrieve the argument value by converting
  // string to integer
  // To receive the final value if valid, max value is 2^64 - 1
  unsigned long long int total_pull_time_temp = 0;
  // To check whether provided a negative value
  long long int total_pull_time_temp_compare = 0;
  if (iter_total_pull_time != arg_map.cend()) {
    if (iter_total_pull_time->second.size() > 1) {
      error_flag.err_invalid_value_for_total_pull_time_ctrl_arg = true;
    }
    // Must be a non-empty vector to be able call strtol
    else if (iter_total_pull_time->second.size() > 0) {
      char* p_end = nullptr;
      char* p_end_compare = nullptr;
      total_pull_time_temp =
          strtoull(iter_total_pull_time->second[0].c_str(), &p_end, 10);
      total_pull_time_temp_compare =
          strtoll(iter_total_pull_time->second[0].c_str(), &p_end_compare, 10);
      if (*p_end != '\0' || *p_end_compare != '\0' ||  // not a number
          total_pull_time_temp_compare <= 0            // non positive value
      ) {
        error_flag.err_invalid_value_for_total_pull_time_ctrl_arg = true;
      }
    }
  }
  if (iter_total_pull_time_long_name != arg_map.cend()) {
    if (iter_total_pull_time_long_name->second.size() > 1) {
      error_flag.err_invalid_value_for_total_pull_time_long_name_ctrl_arg = true;
    } else if (iter_total_pull_time_long_name->second.size() > 0) {
      char* p_end = nullptr;
      char* p_end_compare = nullptr;
      total_pull_time_temp = strtoull(
          iter_total_pull_time_long_name->second[0].c_str(), &p_end, 10);
      total_pull_time_temp_compare =
          strtoll(iter_total_pull_time_long_name->second[0].c_str(),
                  &p_end_compare, 10);
      if (*p_end != '\0' || *p_end_compare != '\0' ||  // not a number
          total_pull_time_temp_compare <= 0            // non positive value
      ) {
        error_flag.err_invalid_value_for_total_pull_time_long_name_ctrl_arg =
            true;
      }
    }
  }

  unsigned long long int pity_starting_temp = 50;
  long long int pity_starting_temp_compare = 50;
  if (iter_pity != arg_map.cend()) {
    if (iter_pity->second.size() > 1) {
      error_flag.err_invalid_value_for_pity_ctrl_arg = true;
    } else if (iter_pity->second.size() > 0) {
      char* p_end = nullptr;
      char* p_end_compare = nullptr;
      pity_starting_temp = strtoull(iter_pity->second[0].c_str(), &p_end, 10);
      pity_starting_temp_compare =
          strtoll(iter_pity->second[0].c_str(), &p_end_compare, 10);
      if (*p_end != '\0' || *p_end_compare != '\0' ||
          pity_starting_temp_compare < 0) {
        error_flag.err_invalid_value_for_pity_ctrl_arg = true;
      }
      // Shrink the big value to atmost 2^32 - 1
      pity_starting_temp = (pity_starting_temp <= max_pity_starting_point)
                                ? pity_starting_temp
                                : max_pity_starting_point;
    }
  }
  if (iter_pity_long_name != arg_map.cend()) {
    if (iter_pity_long_name->second.size() > 1) {
      error_flag.err_invalid_value_for_pity_long_name_ctrl_arg = true;
    } else if (iter_pity_long_name->second.size() > 0) {
      char* p_end = nullptr;
      char* p_end_compare = nullptr;
      pity_starting_temp =
          strtoul(iter_pity_long_name->second[0].c_str(), &p_end, 10);
      pity_starting_temp_compare =
          strtol(iter_pity_long_name->second[0].c_str(), &p_end_compare, 10);
      if (*p_end != '\0' || *p_end_compare != '\0' ||
          pity_starting_temp_compare < 0) {
        error_flag.err_invalid_value_for_pity_long_name_ctrl_arg = true;
      }
      // Shrink the big value to atmost 2^32 - 1
      pity_starting_temp = (pity_starting_temp <= max_pity_starting_point)
                                ? pity_starting_temp
                                : max_pity_starting_point;
    }
  }

  long int num_rate_up_temp = -1;
  if (iter_num_rate_up != arg_map.cend()) {
    if (iter_num_rate_up->second.size() > 1) {
      error_flag.err_invalid_value_for_num_rate_up_ctrl_arg = true;
    } else if (iter_num_rate_up->second.size() > 0) {
      char* p_end = nullptr;
      num_rate_up_temp =
          strtol(iter_num_rate_up->second[0].c_str(), &p_end, 10);
      if (*p_end != '\0' || (num_rate_up_temp != 1 && num_rate_up_temp != 2)) {
        error_flag.err_invalid_value_for_num_rate_up_ctrl_arg = true;
      }
    }
  }
  if (iter_num_rate_up_long_name != arg_map.cend()) {
    if (iter_num_rate_up_long_name->second.size() > 1) {
      error_flag.err_invalid_value_for_num_rate_up_long_name_ctrl_arg = true;
    } else if (iter_num_rate_up_long_name->second.size() > 0) {
      char* p_end = nullptr;
      num_rate_up_temp =
          strtol(iter_num_rate_up_long_name->second[0].c_str(), &p_end, 10);
      if (*p_end != '\0' || (num_rate_up_temp != 1 && num_rate_up_temp != 2)) {
        error_flag.err_invalid_value_for_num_rate_up_long_name_ctrl_arg = true;
      }
    }
  }

  unsigned long long int current_pull_temp = 0;
  long long int current_pull_temp_compare = 0;
  if (iter_current_pull != arg_map.cend()) {
    if (iter_current_pull->second.size() > 1) {
      error_flag.err_invalid_value_for_current_pull_ctrl_arg = true;
    } else if (iter_current_pull->second.size() > 0) {
      char* p_end = nullptr;
      char* p_end_compare = nullptr;
      current_pull_temp =
          strtoull(iter_current_pull->second[0].c_str(), &p_end, 10);
      current_pull_temp_compare =
          strtoll(iter_current_pull->second[0].c_str(), &p_end_compare, 10);
      if (*p_end != '\0' || *p_end_compare != '\0' ||
          current_pull_temp_compare < 0) {
        error_flag.err_invalid_value_for_current_pull_ctrl_arg = true;
      }
      // Now we need to check whether the value of -c|--current-pull exceeds its
      // biggest valid value, i.e., <-p|--pity + 49>
      if (current_pull_temp >= pity_starting_temp + steps_to_guaranteed_star6) {
        error_flag.err_invalid_value_for_current_pull_ctrl_arg = true;
      }
    }
  }
  if (iter_current_pull_long_name != arg_map.cend()) {
    if (iter_current_pull_long_name->second.size() > 1) {
      error_flag.err_invalid_value_for_current_pull_ctrl_arg = true;
    } else if (iter_current_pull_long_name->second.size() > 0) {
      char* p_end = nullptr;
      char* p_end_compare = nullptr;
      current_pull_temp =
          strtoull(iter_current_pull_long_name->second[0].c_str(), &p_end, 10);
      current_pull_temp_compare = strtoll(
          iter_current_pull_long_name->second[0].c_str(), &p_end_compare, 10);
      if (*p_end != '\0' || *p_end_compare != '\0' ||
          current_pull_temp_compare < 0) {
        error_flag.err_invalid_value_for_current_pull_ctrl_arg = true;
      }
      if (current_pull_temp >= pity_starting_temp + steps_to_guaranteed_star6) {
        error_flag.err_invalid_value_for_current_pull_long_name_ctrl_arg = true;
      }
    }
  }

  // Check whether there is unexpected values for the control
  // arguments --standard and --limited
  if (arg_map.count("--standard") == 1 && arg_map["--standard"].size() != 0) {
    error_flag.err_unexpected_value_for_ctrl_arg_standard = true;
  }
  if (arg_map.count("--limited") == 1 && arg_map["--limited"].size() != 0) {
    error_flag.err_unexpected_value_for_ctrl_arg_limited = true;
  }

  display_error_detail(error_flag);

  // Command line input is valid, now set the fields ProbabilityWrapper
  if (!error_flag.check_err()) {
    if (arg_map.find("--standard") != arg_map.end()) {
      probability_wrapper.set_on_banner_star6_conditional_rate(
          standard_banner_on_banner_star6_conditional_rate);
    } else if (arg_map.find("--limited") != arg_map.end()) {
      probability_wrapper.set_on_banner_star6_conditional_rate(
          limited_banner_on_banner_star6_conditional_rate);
    }
    // Set the value of -t|--total-pull-time
    if (iter_total_pull_time != arg_map.cend()) {
      assert(iter_total_pull_time->second.size() == 1);
      total_pull_time = total_pull_time_temp;
    }
    if (iter_total_pull_time_long_name != arg_map.cend()) {
      assert(iter_total_pull_time_long_name->second.size() == 1);
      total_pull_time = total_pull_time_temp;
    }
    // Set the value of -p|--pity
    if (iter_pity != arg_map.cend()) {
      assert(iter_pity->second.size() == 1);
      assert(pity_starting_point <= max_pity_starting_point);
      pity_starting_point = static_cast<unsigned int>(pity_starting_temp);
    }
    if (iter_pity_long_name != arg_map.cend()) {
      assert(iter_pity_long_name->second.size() == 1);
      assert(pity_starting_point <= max_pity_starting_point);
      pity_starting_point = static_cast<unsigned int>(pity_starting_temp);
    }
    // Set the value of -n|--num-rate-up
    if (iter_num_rate_up != arg_map.cend()) {
      assert(iter_num_rate_up->second.size() == 1);
      probability_wrapper.set_banner_operator_num(num_rate_up_temp);
    }
    if (iter_num_rate_up_long_name != arg_map.cend()) {
      assert(iter_num_rate_up_long_name->second.size() == 1);
      probability_wrapper.set_banner_operator_num(num_rate_up_temp);
    }
    // Set the value of -c|--current-pull
    if (iter_current_pull != arg_map.end()) {
      assert(iter_current_pull->second.size() == 1);
      current_pull = current_pull_temp;
    }
    if (iter_current_pull_long_name != arg_map.end()) {
      assert(iter_current_pull_long_name->second.size() == 1);
      current_pull = current_pull_temp;
    }
  }

  return !error_flag.check_err();
}

// Display the simulation settings before starting the simulation
void display_simulation_settings(const ProbabilityWrapper& probability_wrapper,
                                 const unsigned long long int total_pull_time,
                                 const unsigned int pity_starting_point,
                                 const unsigned long long int current_pull) {
  std::cout << "The simulation settings are:\n";
  std::cout << "\tTotal Pulling Times: " << total_pull_time << "\n";
  std::cout << "\tPity System Starting Point: " << pity_starting_point << "\n";
  std::cout << "\tCurrent Pull Times: " << current_pull << "\n";

  if (probability_wrapper.get_on_banner_star6_conditional_rate() ==
      limited_banner_on_banner_star6_conditional_rate) {
    std::cout << "\tBanner Type: Limited Banner, the conditional rate is "
              << limited_banner_on_banner_star6_conditional_rate * 100
              << " %\n";
  } else if (probability_wrapper.get_on_banner_star6_conditional_rate() ==
             standard_banner_on_banner_star6_conditional_rate) {
    std::cout << "\tBanner Type: Standard Banner, the conditional rate is "
              << standard_banner_on_banner_star6_conditional_rate * 100
              << " %\n";
  }
  std::cout << "\tRate-Up Operator(s): "
            << probability_wrapper.get_banner_operator_num() << " operator(s)\n"
            << std::endl;
}

// Display the simulation results
void display_simulation_results(
    const std::vector<unsigned long long int>& result,
    std::unordered_map<unsigned long long int, unsigned long long int>&
        rare_event,
    unsigned long long int star6_count,
    unsigned long long int target_star6_count, unsigned int seed,
    const struct timespec& start, const struct timespec& end) {
  std::cout << "...finished\n" << std::endl;

  // Simulation summary
  std::cout << "SIMULATION SUMMARY" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "Time spent: " << calc_time(start, end) << "s" << std::endl;
  std::cout << "Random seed for this simulation: " << seed << std::endl;
  std::cout << "Star 6 times: " << star6_count << std::endl;
  std::cout << "Target star 6 times: " << target_star6_count << std::endl;

  std::cout << std::endl;

  // Displaying raw data
  std::cout << "RAW DATA" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "First " << raw_data_showing_limit << " raw data:" << std::endl;
  std::cout << "\t";
  for (unsigned int i = 0; i < 10; ++i) {
    std::cout << i + 1 << ":\t";
  }
  std::cout << std::endl;
  for (size_t i = 0; i < raw_data_showing_limit; ++i) {
    if (i % 10 == 0) {
      std::cout << i / 10 + 1 << ":\t";
    }
    std::cout << result[i] << '\t';
    if (i % 10 == 9) {
      std::cout << std::endl;
    }
  }
  // If raw_data_showing_limit is not a multiple of 10, print an extra new line
  std::cout << std::endl;
  if (raw_data_showing_limit % 10 != 0) {
    std::cout << std::endl;
  }

  std::cout << std::endl;

  // Displaying rare events
  std::cout << "RARE EVENTS" << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << "Rare events happend " << rare_event.size() << " times in total"
            << std::endl;
  if (rare_event.size() != 0 && rare_event_showing_limit > 0) {
    std::cout << "Some rare events are (only showing "
              << rare_event_showing_limit << " of them here):" << std::endl;

    size_t counter = 0;
    for (const auto& p : rare_event) {
      if (counter < rare_event_showing_limit) {
        std::cout
            << "\tEvent \"Pulling " << p.first
            << " times to get the target star6 at the last pull\" happend "
            << p.second << " times" << std::endl;
        counter++;
      }
    }
    std::cout << std::endl;
    std::cout << "Note: Since the rare events are very sensitive to the error of the actual distribution\n"
                 "      of generated random numbers (i.e., we want a perfect uniform distribution, but\n"
                 "      there would be error under limited times of random number generating), the\n"
                 "      estimated probabilities for these rare events will not be accurate.\n"
                 "      Hence will not show the estimated probabilities of those rare events."
              << std::endl;
  }

  std::cout << std::endl;

  // Displaying the estimated probability
  // that you succeed *on* N-th pull
  std::cout << "ESTIMATED PROBABILITY" << std::endl;
  std::cout << "-------------------------" << std::endl;
  for (unsigned int i = 1;
       i < std::min(estimated_prob_showing_limit, result.size());
       ++i) {  // skip the unused index 0

    std::cout << "Pr(S_" << i
              << ") = " << (100.0 * result[i]) / target_star6_count << " %"
              << std::endl;
  }

  std::cout << std::endl;

  // Displaying the cumulated probability
  // Here "cumulated" means that you succeed *within* N pulls
  double cumulated_probability = 0.0;
  std::cout << "CUMULATED PROBABILITY" << std::endl;
  std::cout << "-------------------------" << std::endl;
  for (unsigned int i = 1;
       i < std::min(estimated_prob_showing_limit, result.size()); ++i) {
    cumulated_probability += result[i];
    std::cout << "Pr(W_" << i
              << ") = " << (100.0 * cumulated_probability) / target_star6_count
              << " %" << std::endl;
  }
}

#endif  // UTILS_H