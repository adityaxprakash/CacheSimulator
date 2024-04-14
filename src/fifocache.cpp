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
        return {HIT_PENALTY, true};
    }

    int wb_cycles = 0;
    if (q.size() == (this->blocks))
    {
        unsigned int evicted = q.front();
        q.pop();
        line.erase(evicted);
        if (dirty[set][evicted] && !this->write_through)
            wb_cycles = MISS_PENALTY * (block_size >> 2);
        dirty[set].erase(evicted);
    }
    q.push(tag);
    line[tag] = true;
    dirty[set][tag] = false;

    return {(MISS_PENALTY * (block_size >> 2) + HIT_PENALTY + wb_cycles), false};
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
            return {(HIT_PENALTY + MISS_PENALTY), true};

        // write back
        return {HIT_PENALTY, true};
    }

    if (!this->write_allocate)
        return {MISS_PENALTY, false};

    int wb_cycles = 0;
    if (q.size() == (this->blocks))
    {
        unsigned int evicted = q.front();
        q.pop();
        line.erase(evicted);
        if (dirty[set][evicted] && !this->write_through)
            wb_cycles = MISS_PENALTY * (block_size >> 2);
        dirty[set].erase(evicted);
    }

    q.push(tag);
    line[tag] = true;
    dirty[set][tag] = true;

    if (this->write_through)
        return {(MISS_PENALTY * (block_size >> 2) + HIT_PENALTY + MISS_PENALTY), false};

    // write back
    return {(HIT_PENALTY + MISS_PENALTY * (block_size >> 2) + wb_cycles), false};
}