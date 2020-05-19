class Controller {
public:
	void create_new_document();
	void open_document(const std::string& name, SaveDocumentFormat format);
	void save_document(const std::string& name, SaveDocumentFormat format);

	void add_shape(ShapeType type);
	void remove_shape(ShapePtr shape);
};