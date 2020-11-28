class ProbabilityWrapper {
 private:
  // the probability of getting a star 6 operator in one pull
  // without pity system start taking effect
  const double base_star6_rate;

  // the conditional probability of up star 6 operator among all
  // star 6 operators,
  // i.e. Pr(get a on-banner star 6 operator | get a satr 6 operator)
  const double on_banner_star6_conditional_rate;

  // the number of on-banner operators
  const unsigned int banner_operator_num;

 public:
  ProbabilityWrapper()
      : base_star6_rate(-1.0),
        on_banner_star6_conditional_rate(-1.0),
        banner_operator_num(0){};

  ProbabilityWrapper(double _base_star6_rate,
                     double _on_banner_star6_conditional_rate,
                     unsigned int _banner_operator_num)
      : base_star6_rate(_base_star6_rate),
        on_banner_star6_conditional_rate(_on_banner_star6_conditional_rate),
        banner_operator_num(_banner_operator_num){};

  double get_base_star6_rate() { return base_star6_rate; }

  double get_on_banner_star6_conditional_rate() {
    return on_banner_star6_conditional_rate;
  }

  unsigned int get_banner_operator_num() { return banner_operator_num; }
};