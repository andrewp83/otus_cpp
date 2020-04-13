#pragma once

#include <array>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &str, char d);

using octet_t = unsigned char;

template<std::size_t N>
class IpAddress {
public:
    static const size_t LENGTH = N;
    
	IpAddress(const std::string& str) { //"x.x.x.x" 
	    auto strOctets = split(str, '.');
	    if (strOctets.size() == N) {
	    	for (int i = 0; i < N; ++i) {
	        	octet_t oct = std::stoi(strOctets[i]);
	        	setOctet(i, oct);	    		
	    	}
	    }
	}

	// Как-нибудь бы тут сделать N аргументов
	IpAddress(octet_t o1, octet_t o2, octet_t o3, octet_t o4) {
		setOctet(0, o1);
    	setOctet(1, o2);
    	setOctet(2, o3);
    	setOctet(3, o4);
	}

	octet_t getOctet(size_t num) const { // 0..4
		if (num >= N) {
        	return 0; // или exception ?
    	}
    	return data[num];
	}

	void setOctet(size_t num, octet_t value) {
    	if (num < N) {
        	data[num] = value;
    	}		
	}

	bool operator<(const IpAddress& rhs) const {
		//return memcmp(&data[0], &rhs.data[0], sizeof(data[0]) * N) < 0; // не быстрее ли?
    	return data < rhs.data;
	}
    
    std::string toString() const {
    	std::string str;
    	for (int i = 0; i < N; ++i) {
    		if (!str.empty()) str += ".";
    		str += std::to_string(getOctet(i));
    	}
		return str;	
    }

private:
	std::array<octet_t, N> data;
};

using IpAddressV4 = IpAddress<4>;

using IpPool = std::vector<IpAddressV4>;

IpPool ip_filter_1(const IpPool& pool, octet_t value);
IpPool ip_filter_1_2(const IpPool& pool, octet_t value1, octet_t value2);
IpPool ip_filter_any(const IpPool& pool, octet_t value);

void print_ip_pool(const IpPool& pool);
