#include "lrucache.h"

LRUCache::LRUCache(unsigned int sets, unsigned int blocks, unsigned int block_size, bool write_allocate, bool write_through) : Cache(sets, blocks, block_size, write_allocate, write_through)
{
    cache.resize(this->sets);
    lru.resize(this->sets);
}

pair<int, bool> LRUCache::read(unsigned int address)
{
    unsigned int set = (address >> this->block_size_log) & (this->sets - 1);
    unsigned int tag = address >> (this->block_size_log + this->sets_log);

    unordered_map<unsigned int, list<unsigned int>::iterator> &line = this->cache[set];
    list<unsigned int> &l = this->lru[set];

    auto pos = line.find(tag);
    if (pos != line.end())
    {
        l.erase(pos->second);
        l.push_front(tag);
        return {CACHE_CYCLE, true};
    }

    int wb_cycles = 0;
    if (l.size() == (this->blocks))
    {
        unsigned int evicted = l.back();
        l.pop_back();
        line.erase(evicted);
        if (dirty[set][evicted] && !this->write_through)
            wb_cycles = MEM_CYCLE * (block_size >> 2);
        dirty[set].erase(evicted);
    }

    l.push_front(tag);
    line[tag] = l.begin();
    dirty[set][tag] = false;

    return {(MEM_CYCLE * (block_size >> 2) + CACHE_CYCLE + wb_cycles), false};
}

pair<int, bool> LRUCache::write(unsigned int address)
{
    unsigned int set = (address >> this->block_size_log) & (this->sets - 1);
    unsigned int tag = address >> (this->block_size_log + this->sets_log);

    unordered_map<unsigned int, list<unsigned int>::iterator> &line = this->cache[set];
    list<unsigned int> &l = this->lru[set];

    auto pos = line.find(tag);
    if (pos != line.end())
    {
        l.erase(pos->second);
        l.push_front(tag);
        dirty[set][tag] = true;

        if (this->write_through)
            return {(CACHE_CYCLE + MEM_CYCLE), true};

        // write back
        return {CACHE_CYCLE, true};
    }

    if (!this->write_allocate)
        return {MEM_CYCLE, false};

    int wb_cycles = 0;
    if (l.size() == (this->blocks))
    {
        unsigned int evicted = l.back();
        l.pop_back();
        line.erase(evicted);
        if (dirty[set][evicted] && !this->write_through)
            wb_cycles = MEM_CYCLE * (block_size >> 2);
        dirty[set].erase(evicted);
    }

    l.push_front(tag);
    line[tag] = l.begin();
    dirty[set][tag] = true;

    if (this->write_through)
        return {(MEM_CYCLE * (block_size >> 2) + MEM_CYCLE), false};

    // write back
    return {(CACHE_CYCLE + MEM_CYCLE * (block_size >> 2) + wb_cycles), false};
}