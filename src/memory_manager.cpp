#include "BDSP/memory_manager.h"

#include <stdlib.h>

using namespace BDSP::core;

static IMemoryManager *current_manager;
static DefaultMemoryManager default_memory_manager;

void *DefaultMemoryManager::allocate_memory(size_t size) {
    return malloc(size);
}

void DefaultMemoryManager::free_memory(void *ptr) {
    free(ptr);
}

IMemoryManager *MemoryManager::get_current_manager() {
    if (not current_manager) {
        return &default_memory_manager;
    }
    return current_manager;
}

void MemoryManager::set_new_manager(IMemoryManager *new_manager) {
    current_manager = new_manager;
}
