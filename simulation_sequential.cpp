#include <iostream>
#include <unordered_map>

#include "probability_wrapper.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  unsigned int seed = get_random_seed();
  std::mt19937_64 mt(seed);
  unsigned int dist_left_border = 0;
  unsigned int dist_right_border = 999;
  std::uniform_int_distribution<unsigned int> dist(dist_left_border,
                                                   dist_right_border);

  // TODO: accept command line args as rate value
  // process_command_line_input();

  // On default, will calculate the probability of a limited banner (e.g.,
  // Nian, W and Rosmontis banner), which uses the following probability
  // parameter: 
  // Pr(get a star6 operator): 2% 
  // Pr(get a on-banner star 6 operator | get a satr 6 operator): 70%
  ProbabilityWrapper probability_wrapper(0.02, 0.7, 0.02, 2);
  
  // TODO: make it can accept command line arguments, and give prompt of possibly long time consumption!
  // On default, run one billion times
  // unsigned int total_pull_time = 1000000000;
  unsigned int total_pull_time = 20000000;

  // Count the times of getting a star 6 operator in total_pull_times pulling
  unsigned int star6_count = 0;
  // Count the times of getting the target star 6 operator in total_pull_times pulling
  unsigned int target_star6_count = 0;
  // Count the times of countinuously getting a non-star-6 operator
  unsigned int pity_count = 0;
  // Count the times of pulling. Will be reset to 0 when get the target star 6 operator
  unsigned int current_pull_count = 0;

  // The thresholds will be used to decide whether we got a star6/target star6
  // operator in a pull
  const unsigned int init_target_star6_threshold = probability_wrapper.calc_init_target_star6_threshold(dist_left_border, dist_right_border);
  const unsigned int init_star6_threshold = probability_wrapper.calc_init_star6_threshold(dist_left_border, dist_right_border);
  unsigned int target_star6_threshold = init_target_star6_threshold;
  unsigned int star6_threshold = init_star6_threshold;

  // The amount of change each time for the thresholds when pity system comes into effect
  unsigned int delta_star6_threshold = probability_wrapper.calc_star6_threshold_change_step(dist_left_border, dist_right_border);
  unsigned int delta_target_star6_threshold = probability_wrapper.calc_target_star6_threshold_change_step(dist_left_border, dist_right_border);

  // Using a vector as a simple hash map to record the results.
  // Indexes as the times of pulling, values as the times as this event happens
  // Will record at most 1000 times continuously pulls (, which will cost you a
  // lot...)
  // If you are curiouse about the probability of continuously pulling more times
  // and get the target star 6 operator at the last time pull (prepare enough money, haha),
  // then you can increase this number, but this will suffer more from the errors of
  // generated random numbers
  std::vector<unsigned int> result(1000);

  // Record the event that did not get a star 6 operator
  // untill pulling more than 1000 times
  std::unordered_map<unsigned int, unsigned int> rare_event;

  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  // Start simulation
  for (unsigned int i = 0; i < total_pull_time; ++i) {
    unsigned int rand_num = dist(mt);
    current_pull_count++;  // Leave the index 0 of result vector unused
    // Get a star-6 operator
    if (rand_num < star6_threshold) {
      star6_count++;
      // This star-6 operator is also your target operator
      if (rand_num < target_star6_threshold) {
        target_star6_count++;
        if (current_pull_count < result.size()) {
          result[current_pull_count]++;
        } else {
          rare_event[current_pull_count]++;
        }
        current_pull_count = 0;
      }
      pity_count = 0;
      star6_threshold = init_star6_threshold;
      target_star6_threshold = init_target_star6_threshold;
    } else {
      pity_count++;
      if (pity_count > 50) {
        star6_threshold += delta_star6_threshold;
        target_star6_threshold += delta_target_star6_threshold;
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);

  // TODO: abstract this job as a function...
  // Print the result
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
  std::cout << "The rare events are:" << std::endl;
  // TODO: less likely - print limited amount of rare events when this kind of events happens many times in total under **ENOUGH** times of pull simulation
  for (const auto& p : rare_event) {
    std::cout << "\tEvent \"Pulling " << p.first
              << " times to get the target star6 at the last pull\" happend "
              << p.second << " times" << std::endl;
  }
  std::cout << std::endl;
  // TODO: make it controlled by the command line argument and give the warning if user wish to still show the estimated probability
  std::cout << "Note: Since the rare events are very sensitive to the error of the actual distribution\n"
               "      of generated random numbers (i.e., we want a perfect uniform distribution, but\n"
               "      there would be error under limited times of random number generation), the\n"
               "      estimated probabilities for these rare events will not be accurate.\n" 
               "      Hence will not show the estimated probability."
               << std::endl;
  
  std::cout << "-------------------------" << std::endl;

  std::cout << "The estimated probabilities are (skip all non-happend events): " << std::endl;
  for (unsigned int i = 1; i < result.size(); ++i) {  // skip the unused result[0]
    if (result[i] != 0) {
      std::cout << "Pr(Pulling " << i << " times to succeed) = "
                << (100.0 * result[i]) / target_star6_count << "%" << std::endl;
    }
  }

  return 0;
}