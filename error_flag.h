#ifndef ERROR_FLAG_H
#define ERROR_FLAG_H

class ErrorFlag {
 public:
  // The default values for these flags are false
  bool err_redundant_identical_ctrl_arg_flag;
  bool err_redundant_total_pull_time_ctrl_arg_flag;
  bool err_redundant_pity_ctrl_arg_flag;
  bool err_conflict_ctrl_arg_flag;
  bool err_invalid_value_for_ctrl_arg_total_pull_time_flag;
  bool err_missing_value_for_ctrl_arg_total_pull_time_flag;
  bool err_invalid_value_for_ctrl_arg_pity_flag;
  bool err_missing_value_for_ctrl_arg_pity_flag;
  bool err_unexpected_value_for_ctrl_arg_limited;
  bool err_unexpected_value_for_ctrl_arg_regular;
  bool err_unexpected_arguments_at_the_beginning;

  ErrorFlag()
      : err_redundant_identical_ctrl_arg_flag(false),
        err_redundant_total_pull_time_ctrl_arg_flag(false),
        err_redundant_pity_ctrl_arg_flag(false),
        err_conflict_ctrl_arg_flag(false),
        err_invalid_value_for_ctrl_arg_total_pull_time_flag(false),
        err_missing_value_for_ctrl_arg_total_pull_time_flag(false),
        err_invalid_value_for_ctrl_arg_pity_flag(false),
        err_missing_value_for_ctrl_arg_pity_flag(false),
        err_unexpected_value_for_ctrl_arg_limited(false),
        err_unexpected_value_for_ctrl_arg_regular(false),
        err_unexpected_arguments_at_the_beginning(false) {}

  // Return true if any kinds of error occurs
  bool check_err() const {
    return err_redundant_identical_ctrl_arg_flag ||
           err_redundant_identical_ctrl_arg_flag ||
           err_redundant_total_pull_time_ctrl_arg_flag ||
           err_redundant_pity_ctrl_arg_flag || err_conflict_ctrl_arg_flag ||
           err_invalid_value_for_ctrl_arg_total_pull_time_flag ||
           err_missing_value_for_ctrl_arg_total_pull_time_flag ||
           err_invalid_value_for_ctrl_arg_pity_flag ||
           err_missing_value_for_ctrl_arg_pity_flag ||
           err_unexpected_value_for_ctrl_arg_limited ||
           err_unexpected_value_for_ctrl_arg_regular ||
           err_unexpected_arguments_at_the_beginning;
  }
};

#endif  // ERROR_FLAG_H