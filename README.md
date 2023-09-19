# Kirkwood Gaps Simulation
A simulation of [Kirkwood Gaps](https://en.wikipedia.org/wiki/Kirkwood_gap), peculiar gaps in the asteroid belt caused by orbital resonances with Jupiter. The user can either visualize the system in real time (requires Raylib) or plot a distance histogram after a given number of steps (requires Python 3 and Matplotlib).

## Build
```console
    $ gcc kirkwood.c -o kirkwood -lm -lraylib -fopenmp
```

## Run
```console
    $ ./kirkwood v # Visualize system in real time
    $ ./kirkwood p 10000 # Generate distance histogram after 10000 time steps
```

