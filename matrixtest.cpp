#include "LMatrix.hpp"
#include <iostream>

int main (void) 
{
    using namespace std;
    int w, h;
    cout<<"input h and w"<<endl;
    cin>>h;
    cin>>w;
    LMatrix <unsigned char> Matrix(h,w);
    for (int i=0; i<h; i++)
	for (int j=0; j<w; j++)
	    Matrix(i,j) = i*j+i;
    Matrix(1,2) = 984;
    // for (int i=0; i<h; i++) {
    // 	for (int j=0; j<w; j++)
    // 	    cout<<(int)Matrix(i,j)<<" ";
    // 	cout<<endl;
    // }
    Matrix.Neighbours(0,1);
    return 0;

}
