#ifndef PROBABILITY_WRAPPER_H
#define PROBABILITY_WRAPPER_H

class ProbabilityWrapper {
 private:
  // the probability of getting a star 6 operator in one pull
  // without pity system comming into effect
  double base_star6_rate;

  // the conditional probability of up star 6 operator among all
  // star 6 operators,
  // i.e. Pr(get a on-banner star 6 operator | get a satr 6 operator)
  double on_banner_star6_conditional_rate;

  // the change amount of the probability of getting a star6 operator
  // after the pity system starting to take into effect
  double delta_base_star6_rate;

  // the number of on-banner operators
  unsigned int banner_operator_num;

 public:
  ProbabilityWrapper(double _base_star6_rate,
                     double _on_banner_star6_conditional_rate,
                     double _delta_base_star6_rate,
                     unsigned int _banner_operator_num)
      : base_star6_rate(_base_star6_rate),
        on_banner_star6_conditional_rate(_on_banner_star6_conditional_rate),
        delta_base_star6_rate(_delta_base_star6_rate),
        banner_operator_num(_banner_operator_num){};

  // Getters
  const double get_base_star6_rate() const;

  const double get_on_banner_star6_conditional_rate() const;

  const double get_delta_star6_base_rate() const;

  const unsigned int get_banner_operator_num() const;

  // Setters
  void set_on_banner_star6_conditional_rate(
      double _on_banner_star6_conditoinal_rate);

  void set_banner_operator_num(unsigned int _banner_operator_num);

  // calculate the initial threshold of getting a any star 6 operator
  // based on the range of uniform int distribution
  unsigned long long int calc_init_star6_threshold(
      const unsigned int dist_left_border,
      const unsigned int dist_right_border);

  // calculate the initial threshold of getting a specific star 6 operator
  // based on the range of uniform int distribution
  unsigned long long int calc_init_target_star6_threshold(
      const unsigned int dist_left_border,
      const unsigned int dist_right_border);

  // calculate the change step of the threshold of getting a any star 6 operator
  // after the pity system comming into effect
  unsigned int calc_star6_threshold_change_step(
      const unsigned int dist_left_border,
      const unsigned int dist_right_border);

  // calculate the change step of the threshold of getting the target star 6
  // operator after the pity system comming into effect
  unsigned int calc_target_star6_threshold_change_step(
      const unsigned int dist_left_border,
      const unsigned int dist_right_border);
};

#endif  // PROBABILITY_WRAPPER_H