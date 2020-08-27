#pragma once

#include "mail_mapper.h"
#include "reducer.h"

class MailReducer : public mr::IReducer<MailString, MailContainer> {
public:
	virtual void call(const MailContainer& chunk, std::size_t chunk_num) override;
    
protected:
    void save_to_file(int chars_num, std::size_t suffix);
    
protected:
    std::mutex con_m;
};
