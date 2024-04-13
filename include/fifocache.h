#include "cache.h"

class FIFOCache : public Cache
{
private:
    vector<queue<unsigned int>> fifo;
    vector<unordered_map<unsigned int, bool>> cache;

public:
    FIFOCache(unsigned int sets, unsigned int blocks, unsigned int block_size, bool write_allocate, bool write_through);
    pair<int,bool> read(unsigned int address);
    pair<int,bool> write(unsigned int address);
};