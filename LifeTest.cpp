#include "Life.hpp"
#include <iostream>

int main(void)
{
    using namespace std;
    LLife L(10,10);
    L.Draw(3,3,1);
    L.Draw(3,4,1);
    L.Draw(3,5,1);
    L.Draw(2,5,1);
    L.Draw(1,4,1);
    L.Print();
    L.Update();
    L.Print();
    L.Update();
    L.Print();
    L.Update();
    L.Print();
    L.Update();
    L.Print();
    L.Update();
    // cout<<"4:4\t"<<L.Neighbours(4,4)<<endl;
    // cout<<"0:0\t"<<L.Neighbours(0,0)<<endl;
    // cout<<"8:1\t"<<L.Neighbours(8,1)<<endl;
    // cout<<"8:1\t"<<L.Neighbours(8,1)<<endl;
    return 0;
}
