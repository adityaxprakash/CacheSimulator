#include "cache.h"

class LRUCache : public Cache
{
private:
    vector<list<unsigned int>> lru;
    vector<unordered_map<unsigned int, list<unsigned int>::iterator>> cache;

public:
    LRUCache(unsigned int sets, unsigned int blocks, unsigned int block_size, bool write_allocate, bool write_through);
    pair<int,bool> read(unsigned int address);
    pair<int,bool> write(unsigned int address);
};