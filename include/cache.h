#include <bits/stdc++.h>
#pragma once
using namespace std;


class Cache
{
protected:
    const int MEM_CYCLE = 100;
    const int CACHE_CYCLE = 1;
    unsigned int sets;
    unsigned int blocks;
    unsigned int block_size;
    unsigned int block_size_log;
    unsigned int sets_log;
    bool write_allocate;
    bool write_through;
    vector<unordered_map<unsigned int, bool>> dirty;

public:
    Cache(unsigned int sets, unsigned int blocks, unsigned int block_size, bool write_allocate, bool write_through)
    {
        this->sets = sets;
        this->blocks = blocks;
        this->block_size = block_size;
        this->block_size_log = __builtin_ctz(block_size);
        this->sets_log = __builtin_ctz(sets);
        this->write_allocate = write_allocate;
        this->write_through = write_through;
        dirty.resize(sets);
    }
    virtual pair<int,bool> read(unsigned int address) = 0;
    virtual pair<int,bool> write(unsigned int address) = 0;
};