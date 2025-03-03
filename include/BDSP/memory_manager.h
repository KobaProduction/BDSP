#ifndef BDSP_MEMORY_MANAGER_H
#define BDSP_MEMORY_MANAGER_H

#include <stddef.h>

namespace BDSP::core {

class IMemoryManager {
public:
    virtual void *allocate_memory(size_t size) = 0;
    virtual void free_memory(void *ptr) = 0;
    virtual ~IMemoryManager() = default;
};

class DefaultMemoryManager final: public IMemoryManager {
public:
    void *allocate_memory(size_t size) override;
    void free_memory(void *ptr) override;
};

class MemoryManager final {
private:
    MemoryManager() = default;
    ~MemoryManager() = default;
    MemoryManager(MemoryManager const&) = default;
    MemoryManager& operator= (MemoryManager const&) = default;
public:
    static IMemoryManager *get_current_manager();

    static void set_new_manager(IMemoryManager *new_manager);
};

} // namespace BDSP::core

#endif // BDSP_MEMORY_MANAGER_H
