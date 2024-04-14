#include "lrucache.h"
#include "fifocache.h"

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        cout << "Wrong number of arguments\n";
        return 0;
    }
    unsigned int sets = atoi(argv[1]);
    unsigned int blocks = atoi(argv[2]);
    unsigned int block_size = atoi(argv[3]);
    string write_allocate = argv[4];
    string write_through = argv[5];
    string policy = argv[6];

    if (write_allocate != "write-allocate" && write_allocate != "no-write-allocate")
    {
        cout << "Invalid write allocate policy\n";
        return 0;
    }
    if (write_through != "write-through" && write_through != "write-back")
    {
        cout << "Invalid write through policy\n";
        return 0;
    }

    bool wa = write_allocate == "write-allocate";
    bool wt = write_through == "write-through";

    int loads = 0, stores = 0, load_hits = 0, store_hits = 0, load_misses = 0, store_misses = 0;
    long long total_cycles = 0;
    if (policy == "lru")
    {
        LRUCache cache(sets, blocks, block_size, wa, wt);
        string type;
        unsigned int address;
        string ignore;
        while (cin >> type >> hex >> address >> ignore)
        {
            if (type == "s")
            {
                stores++;
                pair<int, bool> result = cache.write(address);
                total_cycles += result.first;
                if (result.second)
                    store_hits++;
                else
                    store_misses++;
            }
            else
            {
                loads++;
                pair<int, bool> result = cache.read(address);
                total_cycles += result.first;
                if (result.second)
                    load_hits++;
                else
                    load_misses++;
            }
        }
    }
    else if (policy == "fifo")
    {
        FIFOCache cache(sets, blocks, block_size, wa, wt);
        string type;
        unsigned int address;
        string ignore;
        while (cin >> type >> hex >> address >> ignore)
        {
            if (type == "s")
            {
                stores++;
                pair<int, bool> result = cache.write(address);
                total_cycles += result.first;
                if (result.second)
                    store_hits++;
                else
                    store_misses++;
            }
            else
            {
                loads++;
                pair<int, bool> result = cache.read(address);
                total_cycles += result.first;
                if (result.second)
                    load_hits++;
                else
                    load_misses++;
            }
        }
    }
    else
    {
        cout << "Invalid policy\n";
        return 0;
    }
    cout << "Total loads: " << loads << "\n";
    cout << "Total stores: " << stores << "\n";
    cout << "Load hits: " << load_hits << "\n";
    cout << "Load misses: " << load_misses << "\n";
    cout << "Store hits: " << store_hits << "\n";
    cout << "Store misses: " << store_misses << "\n";
    cout << "Total cycles: " << total_cycles << "\n";

    return 0;
}