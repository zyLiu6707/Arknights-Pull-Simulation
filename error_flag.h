#ifndef ERROR_FLAG_H
#define ERROR_FLAG_H

class ErrorFlag {
 public:
  bool err_redundant_identical_ctrl_arg_flag = false;
  bool err_redundant_total_pull_time_ctrl_arg_flag = false;
  bool err_redundant_pity_ctrl_arg_flag = false;
  bool err_conflict_ctrl_arg_flag = false;
  bool err_invalid_value_for_ctrl_arg_total_pull_time_flag = false;
  bool err_missing_value_for_ctrl_arg_total_pull_time_flag = false;
  bool err_invalid_value_for_ctrl_arg_pity_flag = false;
  bool err_missing_value_for_ctrl_arg_pity_flag = false;
};

#endif  // ERROR_FLAG_H