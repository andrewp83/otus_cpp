#pragma once

namespace my_db {
	
class Command;

using CommandPtr = std::shared_ptr<Command>;

class Command {
public:
	static CommandPtr create
};


}