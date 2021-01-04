#!/usr/bin/python3.6
import subprocess
import os

path = os.path.dirname(os.path.realpath(__file__))
result_keywords = "Can continue = "
valgrind_flag = "--leak-check=full --show-leak-kinds=all"
valgrind_keywords = "All heap blocks were freed -- no leaks are possible"

# test cases: a list of ["./executable args", "expect_values"]
test_case = [
  # test -t|--total-pull-time
  ["./cmd_parse_unitest -t 18446744073709551615", "1"]
, ["./cmd_parse_unitest -t 18446744073709551616", "1"]
, ["./cmd_parse_unitest -t 18446744073709551614", "1"]
, ["./cmd_parse_unitest -t 9223372036854775807", "1"]
, ["./cmd_parse_unitest -t 9223372036854775808", "1"]
, ["./cmd_parse_unitest -t 9223372036854775806", "1"]
, ["./cmd_parse_unitest -t -18446744073709551616", "0"]
, ["./cmd_parse_unitest -t -18446744073709551615", "0"]
, ["./cmd_parse_unitest -t -18446744073709551617", "0"]
, ["./cmd_parse_unitest -t -9223372036854775807", "0"]
, ["./cmd_parse_unitest -t -9223372036854775808", "0"]
, ["./cmd_parse_unitest -t -9223372036854775806", "0"]

, ["./cmd_parse_unitest --total-pull-time 18446744073709551615", "1"]
, ["./cmd_parse_unitest --total-pull-time 18446744073709551616", "1"]
, ["./cmd_parse_unitest --total-pull-time 18446744073709551614", "1"]
, ["./cmd_parse_unitest --total-pull-time 9223372036854775807", "1"]
, ["./cmd_parse_unitest --total-pull-time 9223372036854775808", "1"]
, ["./cmd_parse_unitest --total-pull-time 9223372036854775806", "1"]
, ["./cmd_parse_unitest --total-pull-time -18446744073709551616", "0"]
, ["./cmd_parse_unitest --total-pull-time -18446744073709551615", "0"]
, ["./cmd_parse_unitest --total-pull-time -18446744073709551617", "0"]
, ["./cmd_parse_unitest --total-pull-time -9223372036854775807", "0"]
, ["./cmd_parse_unitest --total-pull-time -9223372036854775808", "0"]
, ["./cmd_parse_unitest --total-pull-time -9223372036854775806", "0"]

, ["./cmd_parse_unitest --total-pull-time 18446744073709551615", "1"]
, ["./cmd_parse_unitest --total-pull-time 18446744073709551616", "1"]
, ["./cmd_parse_unitest --total-pull-time 18446744073709551614", "1"]
, ["./cmd_parse_unitest --total-pull-time 9223372036854775807", "1"]
, ["./cmd_parse_unitest --total-pull-time 9223372036854775808", "1"]
, ["./cmd_parse_unitest --total-pull-time 9223372036854775806", "1"]
, ["./cmd_parse_unitest --total-pull-time -18446744073709551616", "0"]
, ["./cmd_parse_unitest --total-pull-time -18446744073709551615", "0"]
, ["./cmd_parse_unitest --total-pull-time -18446744073709551617", "0"]
, ["./cmd_parse_unitest --total-pull-time -9223372036854775807", "0"]
, ["./cmd_parse_unitest --total-pull-time -9223372036854775808", "0"]
, ["./cmd_parse_unitest --total-pull-time -9223372036854775806", "0"]

# test -p|--pity
, ["./cmd_parse_unitest -p 4294967295", "1"]
, ["./cmd_parse_unitest -p 4294967296", "1"]
, ["./cmd_parse_unitest -p 4294967294", "1"]
, ["./cmd_parse_unitest -p 2147483647", "1"]
, ["./cmd_parse_unitest -p 2147483648", "1"]
, ["./cmd_parse_unitest -p 2147483646", "1"]
, ["./cmd_parse_unitest -p -4294967295", "0"]
, ["./cmd_parse_unitest -p -4294967296", "0"]
, ["./cmd_parse_unitest -p -4294967294", "0"]
, ["./cmd_parse_unitest -p -2147483647", "0"]
, ["./cmd_parse_unitest -p -2147483648", "0"]
, ["./cmd_parse_unitest -p -2147483646", "0"]

, ["./cmd_parse_unitest --pity 4294967295", "1"]
, ["./cmd_parse_unitest --pity 4294967296", "1"]
, ["./cmd_parse_unitest --pity 4294967294", "1"]
, ["./cmd_parse_unitest --pity 2147483647", "1"]
, ["./cmd_parse_unitest --pity 2147483648", "1"]
, ["./cmd_parse_unitest --pity 2147483646", "1"]
, ["./cmd_parse_unitest --pity -4294967295", "0"]
, ["./cmd_parse_unitest --pity -4294967296", "0"]
, ["./cmd_parse_unitest --pity -4294967294", "0"]
, ["./cmd_parse_unitest --pity -2147483647", "0"]
, ["./cmd_parse_unitest --pity -2147483648", "0"]
, ["./cmd_parse_unitest --pity -2147483646", "0"]

]

if __name__ == "__main__":

    # Run all test cases
    failed_case_index = []
    for i in range(len(test_case)):
        print("Test Case {case_num}".format(case_num=i))
        # print(test_case[case])
        # shell=True here for easier passing the cmd arguments in subprocess.run()
        proc = subprocess.run(
            [test_case[i][0]], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        output = proc.stdout.decode()
        index = output.find(result_keywords)
        result = output[index + len(result_keywords)]
        print("Case {case_num}: result = {result}, expect {expect}".format(case_num=i, result=result,
                                                                           expect=test_case[i][1]), end=", ")
        if result != test_case[i][1]:
            print("Case failed!")
            failed_case_index.append(i)
        else:
            print("Pass")

    if failed_case_index:
        print("Some test cases failed, will not run the valgrind")
        print("Failed test cases are:")
        for i in failed_case_index:
            print(test_case[i][0])

        print("Please fix the bug(s)")

    # If no test cases failed, then check the memory leaks
    else:
        print()
        print("All test cases passed, now starting to check memory leak.")
        print()
        for i in range(len(test_case)):
            print("Check leak for case {case_num}".format(case_num=i))
            valgrind_cmd_str = "valgrind" + " " + valgrind_flag + " " + test_case[i][0]
            # The valgrind will output the program output to stdout, and the message 
            # from valgrind itself to stderr, so here the stderr=subprocess.STDOUT to
            # combine these two streams together
            proc = subprocess.run(
                [valgrind_cmd_str], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
            output = proc.stdout.decode()
            index = output.find(valgrind_keywords)
            if index == -1:
                print("***** Case {case_num} has memory leak! *****".format(case_num=i))
                print(output)
                print(
                    "The remaining test cases memory leak will not be checked, please fix the current memory leaking.")
                
                break
