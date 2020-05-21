#include <list>
#include <map>
#include <memory>

using byte_t = unsigned char;

struct MemoryChunk {
	MemoryChunk() = default;
	MemoryChunk(byte_t* ptr, size_t size) : ptr(ptr), size(size) {}

	byte_t* ptr {nullptr};
	size_t size {0};
};

class MemoryBlock {
public:
	MemoryBlock(size_t size);
    
    MemoryBlock(const MemoryBlock&) = delete;
    MemoryBlock(MemoryBlock&&) = delete;
    
	~MemoryBlock();

	void* memory_allocate(size_t size);
	void memory_free(void* ptr);

	byte_t* get_ptr() const { return ptr; }
	size_t get_size() const { return size; }

	std::string dump();

	bool is_empty() const;

protected:
	byte_t* ptr {nullptr};
	size_t size {0};

	std::map<byte_t*, size_t> allocated_chunks;
	std::list<MemoryChunk> free_chunks;
};

using MemoryBlockPtr = std::shared_ptr<MemoryBlock>;

class MemoryMgr {
public:
    static MemoryMgr& get_instance(size_t block_size);
    
	void* memory_alloc(size_t size);
	void  memory_free(void* ptr/*, size_t size*/);

	void garbage_collect();

	std::string dump();

protected:
	MemoryMgr(size_t block_size);
    MemoryMgr() = default;

protected:
	std::list<MemoryBlockPtr> blocks;

	size_t block_size {0};
};
