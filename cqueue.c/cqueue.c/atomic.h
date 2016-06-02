#pragma once
#include <stdbool.h>
#include <stdint.h>

extern bool atomic_lock(uint64_t* lock);
extern void atomic_unlock(uint64_t* lock);
extern bool atomic_set(uint64_t* addr, uint64_t value);