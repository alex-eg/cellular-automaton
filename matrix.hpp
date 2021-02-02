#pragma once

template <typename T> class LMatrix {
private:
    unsigned int width, height;
public:
    T *field;

    LMatrix &operator = (const LMatrix &right) {
        if (this == &right)
            return *this;
        delete [] field;
        width = right.width;
        height = right.height;
        field = new T [height*width];
        for (unsigned int i=0; i<height; i++)
            for (unsigned int j=0; j<width; j++)
                field[i*height+j] = right.field[i*height+j];
        return *this;
    }

    LMatrix() {
        field = nullptr;
    }

    LMatrix(unsigned int h, unsigned int w) : width(w), height(h) {
        field = new T [height*width];
    }

    LMatrix(const LMatrix &right) {
        delete [] field;
        width = right.width;
        height = right.height;
        field = new T [height*width];
        for (int i=0; i<height; i++)
            for (int j=0; j<width; j++)
                field(i,j) = right.field(i,j);
    }

    ~LMatrix() {
        delete [] field;
    }

    T &operator () (int i, int j) {
        return this->operator()(static_cast<unsigned int>(i), static_cast<unsigned int>(j));
    }

    T &operator () (unsigned int i, unsigned int j) {
        return field[i*height+j];
    }
};
