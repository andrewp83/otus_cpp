#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <numeric>
#include <array>

template<typename T, size_t N, size_t... I>
auto array_to_tuple_impl(const std::array<T, N>& array, std::index_sequence<I...>) {
    return std::make_tuple(array[I]...);
}

template<typename T, size_t N, typename Indices = std::make_index_sequence<N>>
auto array_to_tuple(const std::array<T, N>& array) {
    return array_to_tuple_impl(array, Indices{});
}

template<class T, T def, size_t N>
class Matrix {
    
    using Index = std::array<size_t, N>;
    using ContainerType = std::map<Index, T>;
    
    class ValueProxy {
    public:
        ValueProxy(Matrix<T, def, N>* m, Index index) : index(index), parent(m)  {}

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
        Matrix<T, def, N>* parent {nullptr};
    };
    
    template<size_t D>
    class Indexator {
    public:
        Indexator(Matrix<T, def, N>* m, std::array<size_t, N-D+1> rows) : rows(rows), parent(m) {
            
        }
        
        Indexator<D-1> operator[](size_t col) {
            //Index index = std::make_pair(row, col);
            std::array<size_t, N-D+2> index;
            std::copy(rows.begin(), rows.end(), index.begin());
            *(index.rbegin()) = col;
            return Indexator<D-1>(parent, index);
        }
        
    private:
        std::array<size_t, N-D+1> rows;
        Matrix<T, def, N>* parent {nullptr};
    };
    
    template<>
    class Indexator<2> {
    public:
        Indexator(Matrix<T, def, N>* m, std::array<size_t, N-1> rows) : rows(rows), parent(m) {}
        
        ValueProxy operator[](size_t col) {
            Index index;
            std::copy(rows.begin(), rows.end(), index.begin());
            *(index.rbegin()) = col;
            return ValueProxy(parent, index);
        }
        
    private:
        std::array<size_t, N-1> rows;
        Matrix<T, def, N>* parent {nullptr};
    };
    
public:
    
    size_t size() const {
        return values.size();
    }
    
    class Iterator {
    public:
        Iterator(typename ContainerType::const_iterator base_it) : _base_it(base_it) {}
        
        auto operator*() const {
            auto index_tuple = array_to_tuple(_base_it->first);
            return std::tuple_cat(index_tuple, std::make_tuple(_base_it->second));
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
    
    Iterator begin() const {
        return Iterator(values.begin());
    }
       
    Iterator end() const {
        return Iterator(values.end());
    }
        
    Indexator<N> operator[](size_t row) {
        std::array<size_t, 1U> part_idx;
        part_idx[0] = row;
        return Indexator<N>(this, part_idx);
    }
    
private:
    ContainerType values;
};
