#define BOOST_TEST_MODULE test_memory_mgr

#include "memory_mgr.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_memory_mgr)

BOOST_AUTO_TEST_CASE(test_memory_block_bases)
{
	MemoryBlock block(10);
	BOOST_CHECK_EQUAL(block.dump(), "0000000000");
	BOOST_CHECK(block.is_empty());

	void* ptr = block.memory_allocate(3);
	BOOST_CHECK(ptr);
	BOOST_CHECK_EQUAL(block.dump(), "1110000000");

	void* ptr2 = block.memory_allocate(4);
	BOOST_CHECK(ptr2);
	BOOST_CHECK_EQUAL(block.dump(), "1111111000");

	void* ptr3 = block.memory_allocate(4);
	BOOST_CHECK(!ptr3);

	void* ptr4 = block.memory_allocate(3);
	BOOST_CHECK(ptr4);
	BOOST_CHECK_EQUAL(block.dump(), "1111111111");


	block.memory_free(ptr2);
	BOOST_CHECK_EQUAL(block.dump(), "1110000111");

	block.memory_free(ptr2);
	BOOST_CHECK_EQUAL(block.dump(), "1110000111");

	block.memory_free(ptr);
	BOOST_CHECK_EQUAL(block.dump(), "0000000111");

	block.memory_free(ptr4);
	BOOST_CHECK_EQUAL(block.dump(), "0000000000");
	BOOST_CHECK(block.is_empty());
}

BOOST_AUTO_TEST_CASE(test_memory_block_neighbours)
{
	MemoryBlock block(10);

	void* ptr1 = block.memory_allocate(3);
	void* ptr2 = block.memory_allocate(3);
	void* ptr3 = block.memory_allocate(3);

	BOOST_CHECK(ptr1);
	BOOST_CHECK(ptr2);
	BOOST_CHECK(ptr3);
	BOOST_CHECK_EQUAL(block.dump(), "1111111110");

	block.memory_free(ptr1);
	BOOST_CHECK_EQUAL(block.dump(), "0001111110");

	block.memory_free(ptr3);
	BOOST_CHECK_EQUAL(block.dump(), "0001110000");

	block.memory_free(ptr2);
	BOOST_CHECK_EQUAL(block.dump(), "0000000000");
}

BOOST_AUTO_TEST_CASE(test_memory_mgr)
{
	MemoryMgr mgr(10);

	auto ptr = mgr.memory_alloc(5);
	BOOST_CHECK_EQUAL(mgr.dump(), "1111100000");
    mgr.memory_free(ptr);

	mgr.garbage_collect();

	BOOST_CHECK_EQUAL(mgr.dump(), "");
    
    ptr = mgr.memory_alloc(11);
    BOOST_CHECK_EQUAL(mgr.dump(), "11111111111000000000");
    
    ptr = mgr.memory_alloc(10);
    BOOST_CHECK_EQUAL(mgr.dump(), "11111111111000000000 1111111111");
}

}

