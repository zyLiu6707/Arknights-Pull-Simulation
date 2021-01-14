#include "../utils.h"

int main(int argc, char* argv[]) {
  std::cout << "\n*************** Start Testing ***************\n";

  ProbabilityWrapper dbg_pw(-1.0, -1.0, -1.0, 0);
  unsigned long long int dbg_total_pull_time = 0;
  unsigned int dbg_pity_starting_point = 50;
  unsigned long long int dbg_current_pull = 0;
  std::cout << "argc = " << argc << std::endl;
  std::cout << "Initially:\n"
               "\ttotal pull time          = 0\n"
               "\tpity starting point      = 0\n"
               "\tcurrent pull times       = 0\n"
               "\tconditional probability  = -1.0\n"
               "\ton banner operator num   = 0"
            << std::endl;

  std::cout << "\n----- Starting Processing the Arguments -----\n";

  bool dbg_can_continue = process_cmd_input_and_set_corres_var(argc, argv, dbg_pw, dbg_total_pull_time,
                                       dbg_pity_starting_point, dbg_current_pull);
  std::cout << "\n----- After Processing -----\n";
  std::cout << "\ttotal pull time          = " << dbg_total_pull_time << std::endl;
  std::cout << "\tpity starting point      = " << dbg_pity_starting_point << std::endl;
  std::cout << "\tcurrent pull times       = " << dbg_current_pull << std::endl;
  std::cout << "\tconditional probability  = "
            << dbg_pw.get_on_banner_star6_conditional_rate() << std::endl;
  std::cout << "\ton banner operator num   = " << dbg_pw.get_banner_operator_num()
            << std::endl;
  std::cout << "\nCan continue = " << dbg_can_continue << std::endl;
  std::cout << "*************** End Testing ***************\n"
            << std::endl;

  return 0;
}