#include "LMatrix.hpp"
#include "Set.hpp"

class LLife {
private:
    int width, height;
    set born, survive;
    LMatrix <unsigned char> field1;
    LMatrix <unsigned char> field2;
    LMatrix <unsigned char> *back;
    int Neighbours(int x, int y);
public:
     LMatrix <unsigned char> *front;

    ~LLife()
    {
	front = 0;
	back = 0;
    }

    LLife()
    {
	back = 0;
	front = 0;
    }

    LLife(int w, int h) : width(w), height(h)
    {
	field1 = LMatrix <unsigned char> (width, height);
	field2 = LMatrix <unsigned char> (width, height);
	front = &field1;
	back = &field2;

	for (int i=0; i<height; i++)
	    for (int j=0; j<width; j++)
		field1(i,j) = field1(i,j) = 0;

	born.add(3);
	survive.add(2);
	survive.add(3);
    }

    LLife &operator = (const LLife &right)
    {
	if (this == &right)
	    return *this;
	width = right.width;
	height = right.height;
	
	born = right.born;
	survive = right.survive;
	
	field1 = right.field1;
	field2 = right.field2;
	
	front = &field1;
	back = &field2;
	return *this;
    }
 
    void Randomize();
    void Clear();
    void Update();
    void Draw(int x, int y, unsigned char val);
    void Print();
};
