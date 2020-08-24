#pragma once

#include "mail_mapper.h"
#include "reducer.h"

class MailReducer : public mr::IReducer<MailString, bool, MailContainer> {
public:
	virtual bool call(const MailContainer& chunk) override;
    
    std::mutex con_m;
};
