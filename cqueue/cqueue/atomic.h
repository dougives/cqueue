#pragma once
#include <stdbool.h>
#include <stdint.h>

extern bool atomic_lock(volatile uint64_t* lock);
extern void atomic_unlock(volatile uint64_t* lock);
//extern bool atomic_set(volatile uint64_t* addr, uint64_t value);
extern bool atomic_set32(volatile uint32_t* addr, uint32_t value);
extern bool atomic_set64(volatile uint64_t* addr, uint64_t value);
extern void atomic_increment(volatile uint64_t* addr);