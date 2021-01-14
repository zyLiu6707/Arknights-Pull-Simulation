#include "utils.h"

int main(int argc, char* argv[]) {
  // On default, will calculate the probability of a limited banner (e.g.,
  // Nian, W and Rosmontis banner), which uses the following parameter:
  // 1. Pr(get a star6 operator): 2%
  // 2. Pr(get a on-banner star 6 operator | get a satr 6 operator): 70%
  // 3. Double rate up
  ProbabilityWrapper probability_wrapper(0.02, 0.7, 0.02, 2);

  // In Arknights, the pity system will come into effect after 50 pulls
  // if you did not get any star6 operator
  unsigned int pity_starting_point = 50;

  unsigned long long int total_pull_time = 100000000;
  unsigned long long int current_pull = 0;

  bool can_continue = process_cmd_input_and_set_corres_var(
      argc, argv, probability_wrapper, total_pull_time, pity_starting_point, current_pull);
  if (!can_continue) {
    // Exit the program here rather than exiting when argument format error is
    // found in order to avoid memory leak
    return 0;
  }

  display_simulation_settings(probability_wrapper, total_pull_time,
                              pity_starting_point, current_pull);

  auto seed = get_random_seed();
  std::mt19937_64 mt(seed);
  unsigned int dist_left_border = 0;
  unsigned int dist_right_border = 999;
  // Uniform distribution on [0, 999]
  std::uniform_int_distribution<unsigned int> dist(dist_left_border,
                                                   dist_right_border);

  // Count the times of getting a star 6 operator in total_pull_times pulling
  unsigned long long int star6_count = 0;
  // Count the times of getting the target star 6 operator in total_pull_times
  // pulling
  unsigned long long int target_star6_count = 0;
  // Count the times of countinuously getting a non-star-6 operator
  unsigned long long int pity_count = 0;
  // Count the times of pulling in a trial. Will be reset to 0 when get the
  // target star 6 operator. Theoretically, no matter how many bits used to
  // store the value of current_pull_count, there exists a non-zero probability
  // that it will overflow - but the probability will converge to zero when num
  // of bits grows to positive infinity
  unsigned long long int current_pull_count = 0;

  // The thresholds will be used to decide whether we got a star6/target star6
  // operator in a pull
  const unsigned long long int init_target_star6_threshold =
      probability_wrapper.calc_init_target_star6_threshold(dist_left_border,
                                                           dist_right_border);
  const unsigned long long int init_star6_threshold =
      probability_wrapper.calc_init_star6_threshold(dist_left_border,
                                                    dist_right_border);

  unsigned long long int target_star6_threshold = init_target_star6_threshold;
  unsigned long long int star6_threshold = init_star6_threshold;

  // The amount that the thresholds change after each failed pull when pity
  // system comes into effect
  unsigned int delta_star6_threshold =
      probability_wrapper.calc_star6_threshold_change_step(dist_left_border,
                                                           dist_right_border);
  unsigned int delta_target_star6_threshold =
      probability_wrapper.calc_target_star6_threshold_change_step(
          dist_left_border, dist_right_border);

  std::vector<unsigned long long int> result(1000);

  // Record the event that did not get a star 6 operator
  // untill pulling more than 1000 times
  std::unordered_map<unsigned long long int, unsigned long long int> rare_event;

  // To avoid the rare_event map recording too many items and consuming too
  // large memory.
  const size_t max_rare_event_map_size = 100000;  // you need to simulate
                                                  // approximately 10^13 times
                                                  // of pulling to reach this
                                                  // limit.

  std::cout << "Now will start the simulation...\n" << std::endl;

  struct timespec start;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  // Start simulation
  for (unsigned long long int i = 0; i < total_pull_time; ++i) {
    unsigned int rand_num = dist(mt);
    current_pull_count++;  // leave the index 0 of result vector unused
    // Get a star-6 operator
    if (rand_num < star6_threshold) {
      star6_count++;
      pity_count = 0;
      // This star-6 operator is also your target operator
      if (rand_num < target_star6_threshold) {
        target_star6_count++;
        if (current_pull_count < result.size()) {
          result[current_pull_count]++;
        }
        else if (rare_event.size() < max_rare_event_map_size) {
          rare_event[current_pull_count]++;
        }
        current_pull_count = 0;
        // Finish currrent trial, reset the pity counter and start next trial
        pity_count = current_pull;
      }
      star6_threshold = init_star6_threshold;
      target_star6_threshold = init_target_star6_threshold;
    }
    else {
      pity_count++;
      if (pity_count > pity_starting_point) {
        star6_threshold += delta_star6_threshold;
        target_star6_threshold += delta_target_star6_threshold;
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  // Print the result
  display_simulation_results(result, rare_event, star6_count,
                             target_star6_count, seed, start, end);

  return 0;
}