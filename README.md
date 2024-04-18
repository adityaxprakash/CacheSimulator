# Cache Simulator
In this project, we have built a cache simulator which can be used to study the effectiveness of various cache configurations.

## How to Use?
Make sure you have a Linux machine (preferably Ubuntu) with `gcc` installed on it. Download the code on your local system and run any one of the following: 
```bash
make
make cacheSim   # to compile and link the source files and generate an executable
```

Next you can run the executable with:
```bash
./cacheSim <cache_sets> <blocks_per_set> <block_size> <write-allocate or no-write-allocate> <write-through or write-back> <lru or fifo> < <some trace file>
```
Make sure `cache_sets` `blocks_per_set` and `block_size` are positive powers of 2 with `block_size` being atleast 4.
This will print the output containing information about _hits_ and _misses_ on the terminal.

## Cleaning Files:
```bash
make clean   # removes all object files as well as executables
```

## Experiments:
The outcome of the experiments conducted using this have been compiled into a report which can be found [here](/COL216_Best_Cache_Report.pdf).
