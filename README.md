# City Growth Simulator

**A SimCity-style urban growth simulation written in C++.** Residential, industrial and commercial zones grow over time, share workers and goods, and spread pollution across a grid-based city.

![C++11](https://img.shields.io/badge/C%2B%2B-11-00599C?logo=cplusplus&logoColor=white)
![Dependencies: STL only](https://img.shields.io/badge/dependencies-STL%20only-555555)

## About

I built this simulation on my own as a course project. You give it a city map as a CSV file and a config file that sets how long to run. At each time step it updates every zone according to growth rules that depend on neighboring cells, access to power, and a shared supply of workers and goods. It then prints the city's final state with population and pollution totals, and lets you **zoom in** on any rectangle of the map to see that area's own totals.

**Skills shown:** C++ · STL (`vector`, `sort`, `stringstream`) · file I/O & parsing · simulation modeling · modular design · command-line programs

## Technical Highlights

- **Grid-based simulation.** The city is stored as three 2D `std::vector` grids: the zone map read from the CSV, the population of each cell, and the pollution level of each cell. The simulation advances in discrete time steps, updating the zone types in a fixed order.
- **Simultaneous updates.** Residential changes go into a temporary copy of the grid and are applied all at once at the end of the step ([`residential.cpp`](src/residential.cpp)). That way every cell decides based on the same snapshot, no matter what order cells are processed in.
- **Shared economy.** Zone types depend on each other through shared resources. New residents add workers, industrial cells use workers and produce goods, and commercial cells use one worker and one good each time they grow.
- **Commercial priority.** When workers and goods run short, commercial cells compete for them. They're stored as `Cell` structs and ordered by current population, then by number of populated neighbors, using `std::sort` with custom comparison functions ([`commercial.cpp`](src/commercial.cpp)).
- **Pollution spread.** Each growing industrial cell spreads pollution outward in rings, and the level drops by 1 for each cell of distance from the source ([`pollution.cpp`](src/pollution.cpp)).
- **Input parsing.** Config settings are read with `stringstream`, and the CSV map is split with delimiter-based `getline` ([`main.cpp`](src/main.cpp), [`simulation.cpp`](src/simulation.cpp)).
- **Modular structure.** Each zone type has its own `.h`/`.cpp` file pair, and everything builds with a Makefile that turns compiler warnings into errors (`-Wmost -Werror`), so it compiles with zero warnings.

## How It Works

### Map legend

| Symbol | Meaning |
| --- | --- |
| `R` | Residential zone |
| `I` | Industrial zone |
| `C` | Commercial zone |
| `-` | Road |
| `T` | Power line |
| `#` | Power line over a road |
| `P` | Power plant |

### Each time step

1. **Residential.** An empty cell gets its first resident once it's next to a power plant or a populated cell. Growing beyond that requires more and more populated neighbors, up to all 8. Each new resident adds one **available worker**.
2. **Industrial.** A cell can grow when at least 2 workers are available and it's next to a power line or populated neighbors. Each growth step uses up 2 workers, and cells can reach a population of 3. Every step, each industrial cell produces **goods** equal to its population. Each time an industrial cell grows, it spreads pollution to the cells around it.
3. **Commercial.** Each growth step uses up 1 worker and 1 good. An empty cell needs a power line or a populated neighbor to grow, and a cell at population 1 needs at least 2 populated neighbors to reach 2.

The map and the remaining workers and goods are printed every `Refresh Rate` steps. When the time limit is reached, the program prints:
- the final map
- the population of each zone type
- the total pollution, plus the pollution in residential areas

It then opens the interactive zoom-in.

## Getting Started

**Requirements:** a C++11 compiler and `make`. No other libraries are needed.

```bash
git clone https://github.com/Hannahwu0408/sim-city-project.git
cd sim-city-project
make
./main
```

When asked for a configuration file, enter `examples/config1.txt`. Run the program from the repository root, because paths in the config file are relative to it.

The Makefile uses `clang++`. If you only have `g++` (common on Linux), build with this instead:

```bash
g++ -std=c++11 src/*.cpp -o main
```

To run without typing anything, pipe in the config file and one zoom-in query:

```bash
printf 'examples/config1.txt\n0 3 0 3\nN\n' | ./main
```

## Input Format

**Config file** ([`examples/config1.txt`](examples/config1.txt)). The settings must appear in this order:

```
Region Layout:examples/region1.csv
Time Limit:10
Refresh Rate:5
```

**Region map** ([`examples/region1.csv`](examples/region1.csv)). Every row must have the same number of cells:

```
-,T,T,T,T,T,T,P
-,I,I,-,C,C,-,T
-,I,I,-,C,C,-,T
-,-,-,#,-,-,-,T
-,R,R,-,R,R,-,T
-,R,R,-,R,R,T,T
-,R,R,P,R,R,-,-
-,-,-,-,-,-,-,-
```

## Example Output

Here's a 10-step run on the sample map, with the middle steps cut. In the final state, each zone cell shows its population. By step 6, every available worker is in use and the city stays stable from then on.

```
Please enter the file name for the configuration file: examples/config1.txt
examples/region1.csv
- T T T T T T P
- I I - C C - T
- I I - C C - T
- - - # - - - T
- R R - R R - T
- R R - R R T T
- R R P R R - -
- - - - - - - -
...
FINAL REGION STATE  - - - -
- T T T T T T P
- 3 3 - 1 1 - T
- 3 3 - 1 1 - T
- - - # - - - T
- 2 2 - 2 2 - T
- 3 3 - 3 3 T T
- 2 2 P 2 2 - -
- - - - - - - -
Total Population : 44
 > Total Residential Zone Population: 28
 > Total Industrial Zone Population: 12
 > Total Commercial Zone Population: 4
Total Pollution State: 72
 > Regional Pollution State: 28

ZOOM-IN FEATURE - - - -
Your map is (0, 0) to (7, 7) big.
 > Enter the starting x-coordinate of the region you'd like to zoom-in on : 0
 > Enter the ending x-coordinate of the region you'd like to zoom-in on : 3
 > Enter the starting y-coordinate of the region you'd like to zoom-in on : 0
 > Enter the ending y-coordinate of the region you'd like to zoom-in on : 3
Checking coordinates (0, 0) to (3, 3)...
 > Total Population of region (0, 0) to (3, 3) : 12
 > Total Pollution of region (0, 0) to (3, 3) : 36
Would you like to Zoom-In again? (Y/N) : N
```

In zoom-in coordinates, `x` is the row and `y` is the column. Coordinates outside the map are rejected, and you're asked to enter them again.

## Project Structure

```
sim-city-project/
├── src/
│   ├── main.cpp              # Entry point: reads input, runs the time steps, prints results, zoom-in
│   ├── simulation.h/.cpp     # CSV map reader and shared simulation state
│   ├── residential.h/.cpp    # Residential growth and worker supply
│   ├── industrial.h/.cpp     # Industrial growth and goods production
│   ├── commercial.h/.cpp     # Commercial growth and priority ordering
│   └── pollution.h/.cpp      # Pollution spread from industrial cells
├── examples/
│   ├── config1.txt           # Sample config file
│   └── region1.csv           # Sample 8×8 city map
└── Makefile
```

## Possible Improvements

- **Replace global variables with one object.** Moving the shared state (`available_workers`, `vecRows`, …) into a `Simulation` object that owns the grids would make the simulation reusable and easier to test.
- **A shared interface for zone types.** If every zone type implemented a common `Zone` interface, adding a new zone type wouldn't require changing the main loop.
- **Unit tests** for each growth rule and for the input parser.

## Author

**Hannah Wu**, CS + ML @ University of Pennsylvania · [GitHub](https://github.com/Hannahwu0408)
