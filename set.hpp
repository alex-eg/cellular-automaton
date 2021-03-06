#pragma once
#include "base.hpp"

namespace ca {
template <class T> class Set {
protected:
    T cont[8];
    u8 size = 0;
public:
    Set() = default;

    void add(T val);
    bool in(T val);
    bool del(T val);
    Set(const Set &right) = default;
    Set &operator=(const Set &right) {
        if (this == &right) {
            return *this;
}
        size = right.size;
        for (i32 i=0; i<size; i++) {
            cont[i] = right.cont[i];
}
        return *this;
    }
};

template <class T> void Set<T>::add(T val)
{
    bool flag = true;
    for (i32 i=0; i<size; i++) {
        if (cont[i] == val) { flag = false;
}
}
    if (flag) {
        cont[size] = val;
        size++;
    }
}

template <class T> bool Set<T>::in(T val)
{
    bool flag = false;
    for (i32 i=0; i<size; i++) {
        if (cont[i] == val) { flag = true;
}
}
    return flag;
}

template <class T> bool Set<T>::del(T val)
{
    for (i32 i=0; i<size; i++) {
        if (cont[i] == val) {
            for (i32 j = i; j < size; j++) { cont[j] = cont[j + 1];
}
            size--;
            return true;
        }
}
    return false;
}
} // namespace ca
