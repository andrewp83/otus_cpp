
template<class T>
struct my_list_link_node {
    my_list_link_node(const T& data) : data(data) {}
    
    T data;
    my_list_link_node* prev {nullptr};
    my_list_link_node* next {nullptr};
};

template <class T>
class my_list_iterator {

    my_list_link_node<T>* _ptr {nullptr};

    explicit my_list_iterator(my_list_link_node<T>* p) : _ptr(p) {}

    template<class, class> friend class my_list;
    
public:
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;

    my_list_iterator() : _ptr(nullptr) {}

    my_list_iterator(const my_list_iterator& _p)
        : _ptr(_p._ptr) {
    }

    ~my_list_iterator() = default;

    my_list_iterator& operator=(const my_list_iterator& _p) {
        if (this != &_p) {
            _ptr = _p._ptr;
        }
        return *this;
    }

    reference operator*() const {
        return _ptr->data;
    }
    
    pointer operator->() const {
        return &(_ptr->data);
    }

    my_list_iterator& operator++() {
        _ptr = _ptr->next;
        return *this;
    }
    
    friend bool operator==(const my_list_iterator& lhs, const my_list_iterator& rhs) {
        return lhs._ptr == rhs._ptr;
    }
    
    friend bool operator!=(const my_list_iterator& lhs, const my_list_iterator& rhs) {
        return !(lhs == rhs);
    }
};


template<class T, class Alloc = std::allocator<T>>
class my_list {
public:
    using value_type = T;
    using iterator = my_list_iterator<value_type>;
    
    void push_back(const value_type& x) {
                
        typename Alloc::template rebind<my_list_link_node<T>>::other link_allocator;
        
        my_list_link_node<T>* new_node = link_allocator.allocate(1);
        allocator.construct(new_node, x);
        
        new_node->prev = last_node;
        if (last_node) last_node->next = new_node;
        last_node = new_node;
        if (!first_node) first_node = new_node;
    }
        
    bool empty() const {
        return !first_node;
    }
    
    iterator begin() {
        return iterator(first_node);
    }
    
    iterator end() {
        return iterator(nullptr);
    }
    
protected:
    Alloc allocator;
    
    my_list_link_node<T>* first_node {nullptr};
    my_list_link_node<T>* last_node {nullptr};
};
