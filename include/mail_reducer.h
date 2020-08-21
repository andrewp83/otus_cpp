#pragma once

#include "mail_mapper.h"
#include "reducer.h"

class MailReducer : public mr::IReducer<MailContainer> {
public:
	virtual void call(const MailContainer& chunk) override;
    
     std::mutex out_m;
};
