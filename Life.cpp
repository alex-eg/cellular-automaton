#include "Life.hpp"
#include <iostream>

int LLife::Neighbours(int x, int y)
{
    int ret = 0;
    int cx[3] = {x-1, x, x+1};
    int cy[3] = {y-1, y, y+1};

    if (x == 0) cx[0] = height -1;
    else if (x == height - 1) cx[2] = 0;

    if (y == 0) cy[0] = width -1;
    else if (y == width - 1) cy[2] = 0;

    for (int i=0; i<3; i++)
	for (int j=0; j<3; j++)
	    if (!((cx[i] == x) && (cy[j] == y)) && (*front)(cx[i], cy[j]) != 0) ret++;
    return ret;
}

void LLife::Print()
{
    for (int i=0; i<height; i++) {
	for (int j=0; j<width; j++)
	    std::cout<<(int)(*front)(i,j);
	std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void LLife::Update()
{
    for (int i=0; i<height; i++)
	for (int j=0; j<width; j++)
	    if ((*front)(i,j) == 1)
		if (survive.in(Neighbours(i,j))) (*back)(i,j) = 1;
		else (*back)(i,j) = 0;
	    else
		if (born.in(Neighbours(i,j))) (*back)(i,j) = 1;
		else (*back)(i,j) = 0;
    LMatrix <unsigned char> *temp;
    temp = front;
    front = back;
    back = temp;
}  

void LLife::Draw(int x, int y, unsigned char val)
{
    (*front)(x,y) = val;
}

void LLife::Randomize()
{
    for (int i=0; i<height; i++)
	for (int j=0; j<width; j++)
	    (*front)(i,j) = rand() % 2;
}

void LLife::Clear()
{
    for (int i=0; i<height; i++)
	for (int j=0; j<width; j++)
	    (*front)(i,j) = 0;
}
