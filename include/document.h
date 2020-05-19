#pragma once

class Document {
public:
	void set_name(const std::string& name);
	const std::string& get_name() const;

private:
	std::vector<ShapePtr> shapes;
	std::string name;
};

using DocumentPtr = std::shared_ptr<Document>;
