#pragma once


namespace mr {

template<class T>
struct IMapper {
	virtual T call(std::size_t chunk_num, std::size_t chunks_count) = 0;
};

}
