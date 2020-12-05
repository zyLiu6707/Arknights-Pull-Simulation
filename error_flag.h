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

  ErrorFlag(bool _err_redundant_identical_ctrl_arg_flag,
            bool _err_redundant_total_pull_time_ctrl_arg_flag,
            bool _err_redundant_pity_ctrl_arg_flag,
            bool _err_conflict_ctrl_arg_flag,
            bool _err_invalid_value_for_ctrl_arg_total_pull_time_flag,
            bool _err_missing_value_for_ctrl_arg_total_pull_time_flag,
            bool _err_invalid_value_for_ctrl_arg_pity_flag,
            bool _err_missing_value_for_ctrl_arg_pity_flag) : 
        err_redundant_identical_ctrl_arg_flag(err_redundant_identical_ctrl_arg_flag),
        err_redundant_total_pull_time_ctrl_arg_flag(_err_redundant_total_pull_time_ctrl_arg_flag),
        err_redundant_pity_ctrl_arg_flag(_err_redundant_pity_ctrl_arg_flag),
        err_conflict_ctrl_arg_flag(_err_conflict_ctrl_arg_flag),
        err_invalid_value_for_ctrl_arg_total_pull_time_flag(_err_invalid_value_for_ctrl_arg_total_pull_time_flag),
        err_missing_value_for_ctrl_arg_total_pull_time_flag(_err_missing_value_for_ctrl_arg_total_pull_time_flag),
        err_invalid_value_for_ctrl_arg_pity_flag(_err_invalid_value_for_ctrl_arg_pity_flag),
        err_missing_value_for_ctrl_arg_pity_flag(_err_missing_value_for_ctrl_arg_pity_flag) {}
};

#endif  // ERROR_FLAG_H