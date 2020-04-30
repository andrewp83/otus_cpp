#include <list>

struct MemoryChunk {
	void* ptr {nullptr};
	size_t size {0};
};

class MemoryBlock {
public:
	MemoryBlock(size_t size);
	~MemoryBlock();

	void* memory_allocate(size_t size);
	void memory_free(void* ptr, size_t size);

	void* get_ptr() const { return ptr; }
	size_t get_size() const { return size; }

	std::string dump() const;

	bool is_empty() const;

protected:
	void* ptr {nullptr};
	size_t size {0};

	std::list<MemoryChunk> free_chunks;
};


class MemoryMgr {
public:
	MemoryMgr(size_t block_size);

	void* memory_alloc(size_t size);
	void  memory_free(void* ptr, size_t size);

	void garbage_collect();

protected:
	

protected:
	std::list<MemoryBlock> blocks;

	size_t BLOCK_SIZE {0};
};