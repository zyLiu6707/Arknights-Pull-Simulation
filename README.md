# Arknights-Pull-Simulation

Estimate the theoretical probability of getting a **specific 6★** operator **on <!-- $i^{th}$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=i%5E%7Bth%7D"> pull/within <!-- $i$ --> <img style="transform: translateY(0.1em); background: white;" src="https://render.githubusercontent.com/render/math?math=i"> pulls** in different types of banners in Arknights via Monte Carlo method.

The starting point of the pity system is configurable. I provided this flexibility for you if you are curious about the probability in a different mathematical model. With different settings for this value, you will see that the pity system has a different influence on the theoretical probability.

After `git clone`, `cd` into the directory and run `make` in the repo's directory to build from the source code. Then an executable file named `simulation_sequential` will be generated. This is our simulation program (single thread version).

Run `make clean` to remove all `*.o`s and the executable files.

Simulation results are under `res/`. High resolution line charts and other images are under `img/`. Some test cases and testing python scripts that I used during my development are under `test/`, which you don't need to worry about.

#### Please be sure to refer to [the full documentation](https://rpubs.com/zyLiu6707/arknights-pull-simulation) on RPubs, where LaTeX is naturally supported, for the detail about how to use this repo, the discussion about the mathematical model behind this mobile game and the explanation of simulation results.
