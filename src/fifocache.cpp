#include "fifocache.h"

FIFOCache::FIFOCache(unsigned int sets, unsigned int blocks, unsigned int block_size, bool write_allocate, bool write_through) : Cache(sets, blocks, block_size, write_allocate, write_through)
{
    this->fifo.resize(this->sets);
    this->cache.resize(this->sets);
}

pair<int, bool> FIFOCache::read(unsigned int address)
{
    unsigned int set = (address >> this->block_size_log) & (this->sets - 1);
    unsigned int tag = address >> (this->block_size_log + this->sets_log);

    queue<unsigned int> &q = this->fifo[set];
    unordered_map<unsigned int, bool> &line = this->cache[set];

    if (line.find(tag) != line.end())
    {
        return {CACHE_CYCLE, true};
    }

    int wb_cycles = 0;
    if (q.size() == (this->blocks))
    {
        unsigned int evicted = q.front();
        q.pop();
        line.erase(evicted);
        if (dirty[set][evicted] && !this->write_through)
            wb_cycles = MEM_CYCLE * (block_size >> 2);
        dirty[set].erase(evicted);
    }
    q.push(tag);
    line[tag] = true;
    dirty[set][tag] = false;

    return {(MEM_CYCLE * (block_size >> 2) + CACHE_CYCLE + wb_cycles), false};
}

pair<int, bool> FIFOCache::write(unsigned int address)
{
    unsigned int set = (address >> this->block_size_log) & (this->sets - 1);
    unsigned int tag = address >> (this->block_size_log + this->sets_log);

    queue<unsigned int> &q = this->fifo[set];
    unordered_map<unsigned int, bool> &line = this->cache[set];

    if (line.find(tag) != line.end())
    {
        dirty[set][tag] = true;

        if (this->write_through)
            return {(CACHE_CYCLE + MEM_CYCLE), true};

        // write back
        return {CACHE_CYCLE, true};
    }

    if (!this->write_allocate)
        return {MEM_CYCLE, false};

    int wb_cycles = 0;
    if (q.size() == (this->blocks))
    {
        unsigned int evicted = q.front();
        q.pop();
        line.erase(evicted);
        if (dirty[set][evicted] && !this->write_through)
            wb_cycles = MEM_CYCLE * (block_size >> 2);
        dirty[set].erase(evicted);
    }

    q.push(tag);
    line[tag] = true;
    dirty[set][tag] = true;

    if (this->write_through)
        return {(MEM_CYCLE * (block_size >> 2) + MEM_CYCLE), false};

    // write back
    return {(CACHE_CYCLE + MEM_CYCLE * (block_size >> 2) + wb_cycles), false};
}