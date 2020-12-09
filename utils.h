#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <time.h>

#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "error_flag.h"
#include "probability_wrapper.h"

// Pre-defined parameters for Arknights
const double limited_banner_on_banner_star6_conditional_rate = 0.7;
const double regular_banner_on_banner_star6_conditional_rate = 0.5;

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
// is different each time in each Monte Carlo simulation
auto get_random_seed() {
  std::random_device rd;  // uses RDRND or /dev/urandom
                          // if you want use a specific token, do not use
                          // "/dev/random" on Linux, which is much slower
  std::mt19937_64 mt64(rd());
  auto random_seed = mt64();
  return random_seed;
}

// Display the help message
void display_help_message() {
  std::cout << "Usage: [--help] [-t|--total-pull-time <value>] [--regular|--limited] [-p|--pity <value>]\n"
               "--help : Display the help message\n"
               "--t|--total-pull-time : Set the time of pulling in a simulation\n"
               "                        Note : this is not the experiment time\n"
               "--regular : Simulation and get the estimated probability in a regular banner\n"
               "            Cannot be specified with --limited at the same time\n"
               "--limited : Simulation and get the estimated probability in a limited banner\n"
               "            Cannot be specified with --regular at the same time\n"
               "--pity : Set the starting point where the pity system comes into effect,\n"
               "         i.e., you will get a higher probability on the specified pull's next pull"
               // TODO: add the -n|--on-banner-num arguments introduction!
               << std::endl;

  return;
}

void display_error_detail(const ErrorFlag& error_flag) {
  if (error_flag.check_err()) {
    std::cerr << "The provided command line arguments are invalid due to the following error(s):\n";
    if (error_flag.err_help_ctrl_arg_with_other_args) {
      std::cerr << "\tIf you would like to see help message, please ony specify \"--help\"\n";
      std::cerr << "\tOther arguments are ignored. Will not run the simulation.\n";
      // Ignore other args and potential errors, aborting the program
      return;
    }
    if (error_flag.err_invalid_ctrl_args) {
      std::cerr << "\tInvalid arguments: ";
      for (const auto& invalid_arg : error_flag.invalid_ctrl_args_list) {
        std::cerr << invalid_arg << " ";
      }
      std::cerr << std::endl;
    }
    if (error_flag.err_unexpected_arguments_at_the_beginning) {
      std::cerr << "\tUnexpected argument(s) at the beginning\n";
    }
    // Redundant argument error
    if (error_flag.err_redundant_identical_ctrl_arg) {
      std::cerr << "\tSame arguments are specified more than once\n";
    }
    if (error_flag.err_redundant_total_pull_time_ctrl_arg) {
      std::cerr << "\tRedundant arguments: \"-t\" and \"--total-pull-time\" are specified at the same time\n";
    }
    if (error_flag.err_redundant_pity_ctrl_arg) {
      std::cerr << "\tRedundant arguments: \"-p\" and \"--pity\" are specified at the same time\n";
    }
    // Conflict arguments error
    if (error_flag.err_conflict_ctrl_arg_flag) {
      std::cerr << "\tConflict arguments: \"--regular\" and \"--limited\" are specified at the same time\n";
    }
    // Missing detail value
    if (error_flag.err_missing_value_for_ctrl_arg_total_pull_time) {
      std::cerr << "\tMissing value for \"-t (or --total-pull-time)\"\n";
    }
    if (error_flag.err_missing_value_for_ctrl_arg_pity) {
      std::cerr << "\tMissing value for \"-p (or --pity)\"\n";
    }
    // Invalid value
    if (error_flag.err_invalid_value_for_ctrl_arg_total_pull_time) {
      std::cerr << "\tInvalid value for \"-t (or --total-pull-time)\" - it must be a positive integer\n";
    }
    if (error_flag.err_invalid_value_for_ctrl_arg_pity) {
      std::cerr << "\tInvalid value for \"-p (or --pity)\" - it must be a non-negative integer\n";
    }
    if (error_flag.err_unexpected_value_for_ctrl_arg_regular) {
      std::cerr << "\tUnexpected value for \"--regular\"\n";
    }
    if (error_flag.err_unexpected_value_for_ctrl_arg_limited) {
      std::cerr << "\tUnexpected value for \"--limited\"\n";
    }
    std::cerr << "Please check and correct the error(s)\nYou can refer to help message, README, or visit the online repo:\n"
                 "https://github.com/zyLiu6707/Arknights-Gacha-Simulation\n" << std::endl;
    display_help_message();
  }
}

// Read the command line inputs, check the potential errors.
// If the input is valid, set the corresponding field in ProbabilityWrapper
// and other variables.
// Return a bool flag with true value if the input is valid, which will
// indicates the main simulation program whether can continue running
bool process_cmd_input_and_set_corres_var(
    int argc, char* argv[], ProbabilityWrapper& probability_wrapper,
    unsigned int& total_pull_time, unsigned int& pity_starting_point) {
  const int expected_max_arg_num = 6;
  if (argc > expected_max_arg_num) {
    std::cerr << "Too many arguments!" << std::endl;
    display_help_message();
    return false;
  }
  if (argc == 1) {
    display_help_message();
    return false;
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
        // For arguments that do not need a detailed value, i.e., --help,
        // --regular and --limited, just put an empty vector of string
        // along with this key
        arg_map[argv[i]];
        for (int j = i + 1; j < argc; ++j) {
          if (expected_control_arg.find(argv[j]) != expected_control_arg.end()) {
            break;
          }
          arg_map[argv[i]].push_back(argv[j]);
        }
      } 
      else {
        // Situation that exact same control arg appears more than once
        error_flag.err_redundant_identical_ctrl_arg = true;
        //break;
      }
    }
    else if (argv[i][0] == '-') {
      error_flag.err_invalid_ctrl_args = true;
      error_flag.invalid_ctrl_args_list.push_back(argv[i]);
    }
  }

#ifdef DEBUG  // this function will be executed when debugging
  print_arg_map_contents(arg_map);
#endif

  // If --help is specified, ignore all other arguments and print the help message
  if (arg_map.find("--help") != arg_map.end()) {
    if (argc == 2) {
      display_help_message();
      return false;
    }
    else {
      error_flag.err_help_ctrl_arg_with_other_args = true;
    }
  }

  // Argument error priority order:
  // too many arg > redundant arg > conflict arg > missing value > invalid value
  // Check whether the first argument (i.e., argv[1]) is invalid
  if (expected_control_arg.find(std::string(argv[1])) == expected_control_arg.end()) {
    error_flag.err_unexpected_arguments_at_the_beginning = true;
  }
  // Check another situation of redundant control arguments
  if (arg_map.find("-t") != arg_map.end() &&
      arg_map.find("--total-pull-time") != arg_map.end()) {
    error_flag.err_redundant_total_pull_time_ctrl_arg = true;
  }
  if (arg_map.find("-p") != arg_map.end() &&
      arg_map.find("--pity") != arg_map.end()) {
    error_flag.err_redundant_pity_ctrl_arg = true;
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
    error_flag.err_missing_value_for_ctrl_arg_total_pull_time = true;
  }
  const auto it_pity = arg_map.count("-p") == 1 ? arg_map.find("-p") : arg_map.find("--pity");
  if (it_pity != arg_map.cend() && it_pity->second.size() == 0) {
    error_flag.err_missing_value_for_ctrl_arg_pity = true;
  }

  // Check the format of specific value for control arguments that need one,
  // i.e., -t/--total-pull-time, -p/--pity
  // the specific values are required to be positive integer; If the format is
  // correct, then retrieve the argument value
  long int total_pull_time_temp = -1;
  if (it_total_pull_time != arg_map.cend()) {
    if (it_total_pull_time->second.size() > 1) {
      error_flag.err_invalid_value_for_ctrl_arg_total_pull_time = true;
    } else if (it_total_pull_time->second.size() > 0) {  // must be a non-empty vector to be able call strtol
      char* p_end = nullptr;
      total_pull_time_temp = strtol(it_total_pull_time->second[0].c_str(), &p_end, 10);
      if (*p_end != '\0' || total_pull_time_temp <= 0) {
        error_flag.err_invalid_value_for_ctrl_arg_total_pull_time = true;
      }
    }
  }
  long int pity_starting_temp = -1;
  if (it_pity != arg_map.cend()) {
    if (it_pity->second.size() > 1) {
      error_flag.err_invalid_value_for_ctrl_arg_pity = true;
    } else if (it_pity->second.size() > 0) {  // must be a non-empty vector to be able call strtol
      char* p_end = nullptr;
      pity_starting_temp = strtol(it_pity->second[0].c_str(), &p_end, 10);
      if (*p_end != '\0' || pity_starting_temp < 0) {
        error_flag.err_invalid_value_for_ctrl_arg_pity = true;
      }
    }
  }

  // Check whether there is unexpected values for the control arguments
  // --regular and --limited
  if (arg_map.count("--regular") == 1 && arg_map["--regular"].size() != 0) {
    error_flag.err_unexpected_value_for_ctrl_arg_regular = true;
  }
  if (arg_map.count("--limited") == 1 && arg_map["--limited"].size() != 0) {
    error_flag.err_unexpected_value_for_ctrl_arg_limited = true;
  }

  display_error_detail(error_flag);

  // Command line input is valid, now set the fields ProbabilityWrapper
  if (!error_flag.check_err()) {
    if (arg_map.find("--regular") != arg_map.end()) {
      probability_wrapper.set_on_banner_star6_conditional_rate(
          regular_banner_on_banner_star6_conditional_rate);
    } else if (arg_map.find("--limited") != arg_map.end()) {
      probability_wrapper.set_on_banner_star6_conditional_rate(
          limited_banner_on_banner_star6_conditional_rate);
    }
    if (it_total_pull_time != arg_map.cend()) {
      // Should ony has one element
      assert(it_total_pull_time->second.size() == 1);
      total_pull_time = total_pull_time_temp;
    }
    if (it_pity != arg_map.cend()) {
      // Should only has one element
      assert(it_pity->second.size() == 1);
      pity_starting_point = pity_starting_temp;
    }
  }

  return !error_flag.check_err();
}

// Display the simulation results
void display_simulation_results(
    const std::vector<unsigned int>& result,
    std::unordered_map<unsigned int, unsigned int>& rare_event,
    unsigned int star6_count, unsigned int target_star6_count,
    unsigned int seed, const struct timespec& start,
    const struct timespec& end) {
  std::cout << "Time spent: " << calc_time(start, end) << "s" << std::endl;
  std::cout << "Random seed for this simulation: " << seed << std::endl;
  std::cout << "Star 6 times: " << star6_count << std::endl;
  std::cout << "Target star 6 times: " << target_star6_count << std::endl;

  std::cout << "-------------------------" << std::endl;

  std::cout << "First 100 raw data:" << std::endl;
  std::cout << "\t";
  for (int i = 0; i < 10; ++i) {
    std::cout << i + 1 << ":\t";
  }
  std::cout << std::endl;
  for (unsigned int i = 0; i < 100; ++i) {
    if (i % 10 == 0) {
      std::cout << i / 10 + 1 << ":\t";
    }
    std::cout << result[i] << '\t';
    if (i % 10 == 9) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;

  std::cout << "Rare events happend " << rare_event.size() << " times in total"
            << std::endl;
  if (rare_event.size() != 0) {
    std::cout << "The rare events are:" << std::endl;
  }

  for (const auto& p : rare_event) {
    std::cout << "\tEvent \"Pulling " << p.first
              << " times to get the target star6 at the last pull\" happend "
              << p.second << " times" << std::endl;
  }
  std::cout << std::endl;
  // TODO: make it controlled by the command line argument and give the warning
  // if user wish to still show the estimated probability
  std::cout << "Note: Since the rare events are very sensitive to the error of "
               "the actual distribution\n"
               "      of generated random numbers (i.e., we want a perfect "
               "uniform distribution, but\n"
               "      there would be error under limited times of random "
               "number generation), the\n"
               "      estimated probabilities for these rare events will not "
               "be accurate.\n"
               "      Hence will not show the estimated probabilities of those rare events."
            << std::endl;

  std::cout << "-------------------------" << std::endl;

  std::cout << "The estimated probabilities are (skip all non-happend events): "
            << std::endl;
  for (unsigned int i = 1; i < result.size(); ++i) {  // skip the unused index 0
    if (result[i] != 0) {
      std::cout << "Pr(Pulling " << i << " times to succeed) = "
                << (100.0 * result[i]) / target_star6_count << " %"
                << std::endl;
    }
  }
}

#endif  // UTILS_H