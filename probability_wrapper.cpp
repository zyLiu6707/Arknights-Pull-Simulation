#include "probability_wrapper.h"

// Getters
const double ProbabilityWrapper::get_base_star6_rate() const {
  return base_star6_rate;
}

const double ProbabilityWrapper::get_on_banner_star6_conditional_rate() const {
  return on_banner_star6_conditional_rate;
}

const double ProbabilityWrapper::get_delta_star6_base_rate() const {
  return delta_base_star6_rate;
}

const unsigned int ProbabilityWrapper::get_banner_operator_num() const {
  return banner_operator_num;
}

// Setters
void ProbabilityWrapper::set_on_banner_star6_conditional_rate(double _on_banner_star6_conditoinal_rate) {
  on_banner_star6_conditional_rate = _on_banner_star6_conditoinal_rate;
}

void ProbabilityWrapper::set_banner_operator_num(unsigned int _banner_operator_num) {
  banner_operator_num = _banner_operator_num;
}

// calculate the initial threshold of getting a any star 6 operator
// based on the range of uniform int distribution
unsigned long long int ProbabilityWrapper::calc_init_star6_threshold(
    const unsigned int dist_left_border, const unsigned int dist_right_border) {
  return static_cast<unsigned long long int>(
      (dist_right_border - dist_left_border + 1) * base_star6_rate);
}

// calculate the initial threshold of getting a specific star 6 operator
// based on the range of uniform int distribution
unsigned long long int ProbabilityWrapper::calc_init_target_star6_threshold(
    const unsigned int dist_left_border, const unsigned int dist_right_border) {
  return static_cast<unsigned long long int>(
      (dist_right_border - dist_left_border + 1) * base_star6_rate *
      on_banner_star6_conditional_rate / banner_operator_num);
}

// calculate the change step of the threshold of getting a any star 6 operator
// after the pity system comming into effect
unsigned int ProbabilityWrapper::calc_star6_threshold_change_step(const unsigned int dist_left_border,
                                                                  const unsigned int dist_right_border) {
  return static_cast<unsigned int>((dist_right_border - dist_left_border + 1) * delta_base_star6_rate);
}

// calculate the change step of the threshold of getting the target star 6
// operator after the pity system comming into effect
unsigned int ProbabilityWrapper::calc_target_star6_threshold_change_step(const unsigned int dist_left_border,
                                                                         const unsigned int dist_right_border) {
  return static_cast<unsigned int>(calc_star6_threshold_change_step(dist_left_border, dist_right_border) *
         on_banner_star6_conditional_rate / banner_operator_num);
}