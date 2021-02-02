#pragma once
#include "base.hpp"

namespace ca {

template <typename T> class LMatrix {
private:
    u32 width, height;
public:
    T *field;

    LMatrix &operator = (const LMatrix &right) {
        if (this == &right) {
            return *this;
}
        delete [] field;
        width = right.width;
        height = right.height;
        field = new T [height*width];
        for (u32 i=0; i<height; i++) {
            for (u32 j=0; j<width; j++) {
                field[i*height+j] = right.field[i*height+j];
}
}
        return *this;
    }

    LMatrix() {
        field = nullptr;
    }

    LMatrix(u32 h, u32 w) : width(w), height(h) {
        field = new T [height*width];
    }

    LMatrix(const LMatrix &right) {
        delete [] field;
        width = right.width;
        height = right.height;
        field = new T [height*width];
        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                field(i,j) = right.field(i,j);
}
}
    }

    ~LMatrix() {
        delete [] field;
    }

    T &operator () (int i, int j) {
        return this->operator()(static_cast<u32>(i), static_cast<u32>(j));
    }

    T &operator()(u32 i, u32 j) {
        return field[i * height + j];
    }
};
} // namespace ca
