#include <iostream>
#include <unordered_map>

template<class T, T def>
class Series {
public:
    size_t size() const {
        return values.size();
    }
    
    class ValueProxy {
    public:
        ValueProxy(Series<T, def>* s, size_t index) : index(index), parent(s)  {}
        
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
        size_t index {0};
        Series<T, def>* parent {nullptr};
    };
    
    ValueProxy operator[](size_t i) {
        return ValueProxy(this, i);
    }
    
private:
    std::unordered_map<size_t, T> values;

};

template<class T, T def>
class Matrix {
public:
    using RowType = Series<T, def>;
        
    RowType& operator[](size_t i) {
        return values[i];
    }
        
private:
    std::unordered_map<size_t, RowType> values;
};
