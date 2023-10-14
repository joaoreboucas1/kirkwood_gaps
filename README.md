# Kirkwood Gaps Simulation
A simulation of [Kirkwood Gaps](https://en.wikipedia.org/wiki/Kirkwood_gap), peculiar gaps in the asteroid belt caused by orbital resonances with Jupiter. The user can either visualize the system in real time (requires Raylib) or plot a distance histogram after a given number of steps (requires Python 3 and Matplotlib).

This code is inspired by the book **Introduction to Computer Simulation Methods**, by Harvey Gould, Jan Tobochnik and Wolfgang Christian.

## Build
```console
    $ gcc kirkwood.c -o kirkwood -lm -lraylib
```

The user can also build with the `-fopenmp` flag for speeding up the simulation (doesn't affect real-time visualization):
```console
    $ gcc kirkwood.c -o kirkwood -lm -lraylib -fopenmp
```
To use multithreading, set the `OMP_NUM_THREADS` environment variable before running the code:
```console
    $ export OMP_NUM_THREADS=4 # Set the number of threads you want to use
    $ ./kirkwood p 63000
```

## Run
```console
    $ ./kirkwood vis # Visualize system in real time
    $ ./kirkwood plot 10000 # Generate distance histogram after 10000 time steps
```