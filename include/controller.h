#include "common_types.h"

#include <string>

#include "shapes.h"

class Model;

class Controller {
public:
    Controller(Model* model) : model(model) {}
    
	void create_new_document();
	void open_document(const std::string& name);
	void save_document(const std::string& name, SaveDocumentFormat format);

	void create_shape(ShapeType type, const Point& pt);
	void select_shape(const Point& pt);
	void remove_shape();

	void select_document(size_t index);

private:
	Model* model;
};
