#include "memory_mgr.h"

#include <algorithm>
#include <numeric>
#include <string>
#include <iostream>

#pragma mark - MemoryBlock

MemoryBlock::MemoryBlock(size_t size) : size(size) {
	ptr = (byte_t*)malloc(size);
	free_chunks.emplace_back(ptr, size);
}

MemoryBlock::~MemoryBlock() {
	free(ptr);
}

void* MemoryBlock::memory_allocate(size_t size) {

	auto it = std::find_if(free_chunks.begin(), free_chunks.end(), [size](const MemoryChunk& chunk){
		return chunk.size >= size;
	});
	if (it == free_chunks.end()) {
		return nullptr;
	}
	
	byte_t* ptr = it->ptr;
	
	if (it->size > size) {
		it->ptr += size;
		it->size -= size;
	} else {
		free_chunks.erase(it);
	}

	allocated_chunks[ptr] = size;

	return ptr;
}

void MemoryBlock::memory_free(void* ptr) {

	byte_t* b_ptr = (byte_t*)ptr;

	auto it = allocated_chunks.find(b_ptr);
	if (it == std::end(allocated_chunks)) {
		// что делать непонятно, исключение ?
		return;
	}

	size_t size = it->second;

	auto it_left_neighbour = free_chunks.end();
	auto it_right_neighbour = free_chunks.end();
	for (auto it = free_chunks.begin(); it != free_chunks.end(); ++it) {
		const MemoryChunk& cur_chunk = *it;
		if (cur_chunk.ptr + cur_chunk.size == b_ptr) {
			it_left_neighbour = it;
		}
		if (b_ptr + size == cur_chunk.ptr) {
			it_right_neighbour = it;
		}
	}

	allocated_chunks.erase(it);

	MemoryChunk new_free_chunk(b_ptr, size);

	if (it_left_neighbour != free_chunks.end()) {
		new_free_chunk.ptr = it_left_neighbour->ptr;
		new_free_chunk.size += it_left_neighbour->size;
		free_chunks.erase(it_left_neighbour);
	}

	if (it_right_neighbour != free_chunks.end()) {
		new_free_chunk.size += it_right_neighbour->size;
		free_chunks.erase(it_right_neighbour);
	}

	free_chunks.push_back(new_free_chunk);
}

bool MemoryBlock::is_empty() const {
	size_t free_size = std::accumulate(free_chunks.begin(), free_chunks.end(), 0, [](size_t total, const MemoryChunk& ch) {
		return total + ch.size;
	});
	//std::cout << free_size << std::endl;
	return free_size == size;
}

std::string MemoryBlock::dump() {
	std::string res(size, '1');
	for (const MemoryChunk& free_ch : free_chunks) {
		size_t i_start = free_ch.ptr - ptr;
		res.replace(i_start, free_ch.size, free_ch.size, '0');
	}
	return res;
}

#pragma mark - MemoryMgr

MemoryMgr& MemoryMgr::get_instance(size_t block_size) {
    static std::map<size_t, MemoryMgr*> managers_cache;
    
    auto it = managers_cache.find(block_size);
    if (it != std::end(managers_cache)) {
        return *(it->second);
    }
    
    MemoryMgr* instance = new MemoryMgr(block_size);
    managers_cache[block_size] = instance;
    return *instance;
}

MemoryMgr::MemoryMgr(size_t block_size) : block_size(block_size) {
}
	
void* MemoryMgr::memory_alloc(size_t size) {
	void* ptr = nullptr;

	for (auto& block : blocks) {
		if ((ptr = block->memory_allocate(size))) {
			break;
		}
	}

	if (!ptr) {
		// Свободного блока нет, выделение памяти
		auto blocks_need = size / block_size;
		if (size % block_size > 0) blocks_need++;
		auto total_bytes = blocks_need * block_size;
		MemoryBlockPtr new_block = std::make_shared<MemoryBlock>(total_bytes);
		blocks.push_back(new_block);

		ptr = new_block->memory_allocate(size);
	}

	return ptr;
}

void MemoryMgr::memory_free(void* ptr/*, size_t size*/) {

	// Найти блок, в котором находится выделенная память 
	
	auto it_block = std::find_if(blocks.begin(), blocks.end(), [=](const MemoryBlockPtr& b){
		return b->get_ptr() <= ptr && ptr < b->get_ptr() + b->get_size();
	});

	if (it_block == blocks.end()) {
		return;
	}

	(*it_block)->memory_free(ptr);
}

void MemoryMgr::garbage_collect() {
	auto it = blocks.begin();
	while (it != blocks.end()) {
		if ((*it)->is_empty()) {
			it = blocks.erase(it);
		} else {
			it++;
		}
	}
}

std::string MemoryMgr::dump() {
	std::string res;
	for (MemoryBlockPtr& b : blocks) {
		if (!res.empty()) res += " ";
		res += b->dump();
	}
	return res;
}
