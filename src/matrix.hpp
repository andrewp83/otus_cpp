#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <numeric>


template<class T, T def>
class Matrix {
    
    using Index = std::pair<size_t, size_t>;
    using ContainerType = std::map<Index, T>;
    
    class ValueProxy {
    public:
        ValueProxy(Matrix<T, def>* m, Index index) : index(index), parent(m)  {}

        ValueProxy& operator=(const T& value) {
            if (value == def) {
                parent->values.erase(index);
            } else {
                parent->values[index] = value;
            }
            return *this;
        }

        operator T() const {
            auto it = parent->values.find(index);
            if (it != parent->values.end()) {
                return it->second;
            }
            return def;
        }

    private:
        Index index;
        Matrix<T, def>* parent {nullptr};
    };
    
    class Indexator {
    public:
        Indexator(Matrix<T, def>* m, size_t row) : row(row), parent(m) {}
        
        ValueProxy operator[](size_t col) {
            Index index = std::make_pair(row, col);
            return ValueProxy(parent, index);
        }
        
    private:
        size_t row {0};
        Matrix<T, def>* parent {nullptr};
    };
    
public:
    
    size_t size() const {
        return values.size();
    }
        
    class Iterator {
    public:
        using value_type = std::tuple<size_t, size_t, T>;
        using reference = value_type&;
        using pointer = value_type*;
        
        Iterator(typename ContainerType::const_iterator base_it) : _base_it(base_it) {}
        
        value_type operator*() const {
            return std::make_tuple(_base_it->first.first, _base_it->first.second, _base_it->second);
        }
        
        Iterator& operator++() {
            _base_it++;
            return *this;
        }
        
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._base_it == rhs._base_it;
        }
        
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }
        
    private:
        typename ContainerType::const_iterator _base_it;
    };
    
    Iterator begin() {
        return Iterator(values.begin());
    }
       
    Iterator end() {
        return Iterator(values.end());
    }
        
    Indexator operator[](size_t row) {
        return Indexator(this, row);
    }
    
private:
    ContainerType values;
};
