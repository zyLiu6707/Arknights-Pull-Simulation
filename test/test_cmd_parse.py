#!/usr/bin/python3.6
import subprocess
import os

path = os.path.dirname(os.path.realpath(__file__))
result_keywords = "Can continue = "
valgrind_flag = "--leak-check=full --show-leak-kinds=all"
valgrind_keywords = "All heap blocks were freed -- no leaks are possible"

# test cases: a list of ["./executable args", "expect_values"]
test_case = [["./cmd_parse_unitest", "1"]
    , ["./cmd_parse_unitest --help", "0"]

    # Test cases for -t, --total-pull-time
    , ["./cmd_parse_unitest -t 2", "1"]
    , ["./cmd_parse_unitest -t 2 3", "0"]
    , ["./cmd_parse_unitest -t 2.0", "0"]
    , ["./cmd_parse_unitest -t 0", "0"]
    , ["./cmd_parse_unitest -t -1", "0"]
    , ["./cmd_parse_unitest -t -1.0", "0"]
    , ["./cmd_parse_unitest --t kaltsit_is_my_waifu", "0"]
    , ["./cmd_parse_unitest -t", "0"]
    , ["./cmd_parse_unitest -tt", "0"]
    , ["./cmd_parse_unitest --total-pull-time 2", "1"]
    , ["./cmd_parse_unitest --total-pull-time 2 3", "0"]
    , ["./cmd_parse_unitest --total-pull-time 2.0", "0"]
    , ["./cmd_parse_unitest --total-pull-time 0", "0"]
    , ["./cmd_parse_unitest --total-pull-time -1", "0"]
    , ["./cmd_parse_unitest --total-pull-time -1.0", "0"]
    , ["./cmd_parse_unitest --total-pull-time kaltsit_is_my_waifu", "0"]
    , ["./cmd_parse_unitest --total-pull-time", "0"]
    , ["./cmd_parse_unitest --total-pull-tim", "0"]
    , ["./cmd_parse_unitest -t --total-pull-time", "0"]
    , ["./cmd_parse_unitest -t 3 --total-pull-time", "0"]
    , ["./cmd_parse_unitest -t --total-pull-time 4", "0"]
    , ["./cmd_parse_unitest -t 3 --total-pull-time 4", "0"]

    # Test cases for -p, --pity
    , ["./cmd_parse_unitest -p 2", "1"]
    , ["./cmd_parse_unitest -p 2 3", "0"]
    , ["./cmd_parse_unitest -p 2.0", "0"]
    , ["./cmd_parse_unitest -p 0", "1"]
    , ["./cmd_parse_unitest -p -1", "0"]
    , ["./cmd_parse_unitest -p -1.0", "0"]
    , ["./cmd_parse_unitest -p kaltsit_is_my_waifu", "0"]
    , ["./cmd_parse_unitest -p", "0"]
    , ["./cmd_parse_unitest -pp", "0"]
    , ["./cmd_parse_unitest --pity", "0"]
    , ["./cmd_parse_unitest --pity 2", "1"]
    , ["./cmd_parse_unitest --pity 2 3", "0"]
    , ["./cmd_parse_unitest --pity 2.0", "0"]
    , ["./cmd_parse_unitest --pity 0", "1"]
    , ["./cmd_parse_unitest --pity -1", "0"]
    , ["./cmd_parse_unitest --pity -1.0", "0"]
    , ["./cmd_parse_unitest --pity kaltsit_is_my_waifu", "0"]
    , ["./cmd_parse_unitest --pity", "0"]
    , ["./cmd_parse_unitest --pit", "0"]
    , ["./cmd_parse_unitest -p --pity", "0"]
    , ["./cmd_parse_unitest -p 3 --pity", "0"]
    , ["./cmd_parse_unitest -p --pity 4", "0"]
    , ["./cmd_parse_unitest -p 3 --pity 4", "0"]
    
    # Test cases for -n, --num-rate-up
    , ["./cmd_parse_unitest -n 1", "1"]
    , ["./cmd_parse_unitest -n 2", "1"]
    , ["./cmd_parse_unitest -n 3", "0"]
    , ["./cmd_parse_unitest -n 2 3", "0"]
    , ["./cmd_parse_unitest -n 2.0", "0"]
    , ["./cmd_parse_unitest -n 0", "0"]
    , ["./cmd_parse_unitest -n -1", "0"]
    , ["./cmd_parse_unitest -n -1.0", "0"]
    , ["./cmd_parse_unitest -n kaltsit_is_my_waifu", "0"]
    , ["./cmd_parse_unitest -n", "0"]
    , ["./cmd_parse_unitest -nn", "0"]
    , ["./cmd_parse_unitest --num-rate-up", "0"]
    , ["./cmd_parse_unitest --num-rate-up 1", "1"]
    , ["./cmd_parse_unitest --num-rate-up 2", "1"]
    , ["./cmd_parse_unitest --num-rate-up 3", "0"]
    , ["./cmd_parse_unitest --num-rate-up 2 1", "0"]
    , ["./cmd_parse_unitest --num-rate-up 2.0", "0"]
    , ["./cmd_parse_unitest --num-rate-up 0", "0"]
    , ["./cmd_parse_unitest --num-rate-up -1", "0"]
    , ["./cmd_parse_unitest --num-rate-up -1.0", "0"]
    , ["./cmd_parse_unitest --num-rate-up kaltsit_is_my_waifu", "0"]
    , ["./cmd_parse_unitest --num-rate-up", "0"]
    , ["./cmd_parse_unitest --num-rate-up", "0"]
    , ["./cmd_parse_unitest -n --num-rate-up", "0"]
    , ["./cmd_parse_unitest -n 2 --num-rate-up", "0"]
    , ["./cmd_parse_unitest -n --num-rate-up 4", "0"]
    , ["./cmd_parse_unitest -n 1 --num-rate-up 4", "0"]

    , ["./cmd_parse_unitest --regular", "1"]
    , ["./cmd_parse_unitest --regular --regular", "0"]
    , ["./cmd_parse_unitest --regular 2", "0"]
    , ["./cmd_parse_unitest --limited", "1"]
    , ["./cmd_parse_unitest --limited --limited", "0"]
    , ["./cmd_parse_unitest --limited 4", "0"]
    , ["./cmd_parse_unitest -regula", "0"]
    , ["./cmd_parse_unitest -limite", "0"]
    , ["./cmd_parse_unitest --regular -t 20", "1"]
    , ["./cmd_parse_unitest --limited -p 20", "1"]
    , ["./cmd_parse_unitest --regular -t 20 --limited", "0"]
    , ["./cmd_parse_unitest -t -p --regular --limited -p", "0"]
    , ["./cmd_parse_unitest -t -p --regular --limited --pity", "0"]
    , ["./cmd_parse_unitest -t -p --regular --pity 0.0", "0"]
    , ["./cmd_parse_unitest 123", "0"]
    , ["./cmd_parse_unitest 123 -p", "0"]
    , ["./cmd_parse_unitest 123 -p 50", "0"]
    , ["./cmd_parse_unitest -help", "0"]
    , ["./cmd_parse_unitest --", "0"]
    , ["./cmd_parse_unitest --help 2", "0"]
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
