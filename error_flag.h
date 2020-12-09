#ifndef ERROR_FLAG_H
#define ERROR_FLAG_H

#include <string>
#include <vector>

class ErrorFlag {
 public:
  // The default values for these flags are false
  bool err_redundant_identical_ctrl_arg;

  bool err_redundant_total_pull_time_ctrl_arg;
  bool err_invalid_value_for_total_pull_time_ctrl_arg;
  bool err_missing_value_for_total_pull_time_ctrl_arg;

  bool err_redundant_pity_ctrl_arg;
  bool err_invalid_value_for_pity_ctrl_arg;
  bool err_missing_value_for_pity_ctrl_arg;

  bool err_redundant_num_rate_up_ctrl_arg;
  bool err_invalid_value_for_num_rate_up_ctrl_arg;
  bool err_missing_value_for_num_rate_up_ctrl_arg;

  bool err_conflict_ctrl_arg_flag;
  bool err_unexpected_value_for_ctrl_arg_limited;
  bool err_unexpected_value_for_ctrl_arg_regular;
  bool err_unexpected_arguments_at_the_beginning;
  bool err_help_ctrl_arg_with_other_args;
  bool err_invalid_ctrl_args;

  std::vector<std::string> invalid_ctrl_args_list;

  ErrorFlag()
      : err_redundant_identical_ctrl_arg(false),
        err_redundant_total_pull_time_ctrl_arg(false),
        err_invalid_value_for_total_pull_time_ctrl_arg(false),
        err_missing_value_for_total_pull_time_ctrl_arg(false),
        err_redundant_pity_ctrl_arg(false),
        err_invalid_value_for_pity_ctrl_arg(false),
        err_missing_value_for_pity_ctrl_arg(false),
        err_redundant_num_rate_up_ctrl_arg(false),
        err_invalid_value_for_num_rate_up_ctrl_arg(false),
        err_missing_value_for_num_rate_up_ctrl_arg(false),
        err_conflict_ctrl_arg_flag(false),
        err_unexpected_value_for_ctrl_arg_limited(false),
        err_unexpected_value_for_ctrl_arg_regular(false),
        err_unexpected_arguments_at_the_beginning(false),
        err_help_ctrl_arg_with_other_args(false),
        err_invalid_ctrl_args(false) {}

  // Return true if any kinds of error occurs
  bool check_err() const {
    return err_redundant_identical_ctrl_arg ||
           err_redundant_total_pull_time_ctrl_arg ||
           err_invalid_value_for_total_pull_time_ctrl_arg ||
           err_missing_value_for_total_pull_time_ctrl_arg ||
           err_redundant_pity_ctrl_arg ||
           err_invalid_value_for_pity_ctrl_arg ||
           err_missing_value_for_pity_ctrl_arg ||
           err_redundant_num_rate_up_ctrl_arg ||
           err_invalid_value_for_num_rate_up_ctrl_arg ||
           err_missing_value_for_num_rate_up_ctrl_arg ||
           err_conflict_ctrl_arg_flag ||
           err_unexpected_value_for_ctrl_arg_limited ||
           err_unexpected_value_for_ctrl_arg_regular ||
           err_unexpected_arguments_at_the_beginning ||
           err_help_ctrl_arg_with_other_args ||
           err_invalid_ctrl_args;
  }
};

#endif  // ERROR_FLAG_H