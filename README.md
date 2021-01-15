# Arknights-Pull-Simulation
------
### Please be sure to refer to [the full documentation](https://rpubs.com/zyLiu6707/arknights-pull-simulation) ([中文版文档](https://rpubs.com/zyLiu6707/711321)) on RPubs, where LaTeX is naturally supported, for the detail about how to use this repo, the discussion about the mathematical model behind this mobile game and the explanation of simulation results.
------

Estimate the theoretical probability of getting a **specific 6★** operator **on <!-- $i^{th}$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=i%5E%7Bth%7D"> pull/within <!-- $i$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=i"> pulls** in different types of banners in Arknights via Monte Carlo method.

The starting point of the pity system is configurable. I provided this flexibility for you if you are curious about the probability in a different mathematical model. With different settings for this value, you will see that the pity system has a different influence on the theoretical probability.

### Build the Code

After `git clone`, `cd` into the directory and run `make` in the repo's directory to build from the source code. Then an executable file named `simulation_sequential` will be generated.

Run `make clean` to remove all `*.o`s and the executable files.

### Command Line Arguments

You need to specify some command line arguments to run the simulation:

```shell
./simulation_sequential [--help] [-t|--total-pull-time <value>] [--standard|--limited] 
                        [-p|--pity <value>] [-n|--num-rate-up <value>] [-c|--current-pull <value>]
```

The arguments in square brackets are optional, and their orders does not matter. You can use either the short name arguments (`-t`, `-p`, `-n` and `-c`) or the long name arguments (`--total-pull-time`, `--pity`, `--num-rate-up` and `--current-pull`) as you wish. 

Each arguments' meaning is explained in the following table:

| Argument                     | Explanation                                                  |
| :--------------------------- | :----------------------------------------------------------- |
| `--help`                     | Show the help message - the introduction of each argument and how to run the program <br/>Should not be specified with other arguments at the same time |
| `-t`<br/>`--total-pull-time` | Set how many times of pulling will be executed during the simulation<br/>Control the scale of the simulation<br/>**Valid value: an integer between [1, 18446744073709551615] (inclusive)** |
| `--standard`                 | Estimate the theoretical probability in a standard banner    |
| `--limited`                  | Estimate the theoretical probability in a limited banner     |
| `-p`<br/>`--pity`            | Set the starting point where the pity system comes into effect<br/>The pity system will start to increase the probability of getting a 6★ operator in the pull after the `N-th` pull (`N` is the number you specified)<br/>**Valid value: an integer from [0, 4294967295] (inclusive)** |
| `-n`<br/>`--num-rate-up`     | Set whether to simulate a single-rate-up banner or a double-rate-up banner<br/>**Valid value: either 1 or 2** |
| `-c`<br/>`--currentl-pull`   | Set how many times have you pulled but without getting a 6★ operator<br/>**Valid value: an integer from [0, `<-p \| --pity value>` + 49) (inclusive, exclusive)** |

On default, if no arguments are provided, the program will simulate 100,000,000 times of pulling in a double-rate-up limited banner, with zero value for `-c` or `--current-pull` and pity starting point equals to 50 (equivalent to run with `--limited -t 100000000 -n 2 -p 50`), which is same as in a limited banner in Arknights, unless you specify the corresponding arguments to override the default behavior of the program.

Simulation results are under `res/`. High resolution line charts and other images are under `img/`. Some test cases and testing python scripts that I used during my development are under `test/`, which you don't need to worry about.

### Some Further Explanation

* The bigger the value for total pull time is, the more accurate the result will be, but the program will consume more time to finish the simulation. The single thread simulation with `-t 10000000000` (simulating 10 billion times of pulling) took about 110 seconds to finish on the platform that I used:

  * OS: Ubuntu 18.04
  * CPU: Intel® Xeon® Gold 6140 CPU @ 2.30GHz
  * Memory: 8GB

* You can override the program's default simulation configuration. For example, if you only specify `-t 200000000` (or `--total-pull-time 200000000`) and still omit other arguments, the default value of total pull times (which is 100,000,000) will be overridden, and the theoretical probability of a double-rate-up limited banner will be estimated under 200,000,000 times simulated pulling. If you also specify `--standard` and `-n 1`, then the program will simulate a single-rate-up standard banner under 200,000,000 times simulated pulling.

* The program will try its best to find out any types of syntax errors that may occur in the command line arguments and give you the error reason (show in the picture below) — just like a simple compiler. However, do not fully rely on this error-check feature: always read the docs carefully ;-)

  <img src="img/Arguments Error Checking.png" style="zoom:33%;" />

### Quick Examples

Here are several quick examples to start with:

* See the help message

  `./simulation_sequential --help`

* Simulate a double-rate-up limited banner

  `./simulation_sequential --limited -t <a-positive-big-integer>`

  or just

  `./simulation_sequential -t <a-positive-big-integer>`

* Simulate a double-rate-up standard banner

  `./simulation_sequential --standard -t <a-positive-big-integer>` 

* Simulate a single-rate-up standard banner

  `./simulation_sequential --standard -n 1 -t <a-positive-big-integer>`

* Simulate a single-rate-up banner which you will start to roll, but you have pulled for 42 times without getting a 6★ operator in another standard banner.

  `./simulation_sequential --standard -n 1 -c 42 -t <a-positive-big-integer>`

* Simulate a single-rate-up limited banner (not appeared in Arknights yet!)

  `./simulation_sequential --limited -n 1 -t <a-positive-big-integer>`

  or just

  `./simulation_sequential -n 1 -t <a-positive-big-integer>`

  The recommended value for `<a-positive-big-integer>` is 100000000 or above.

  It is recommended that you redirect the output to a file, for example

  ```shell
  ./simulation_sequential --limited -t 200000000 >> simu_limited_banner.res
  ```

  since the output of the simulation program will be relatively long.
