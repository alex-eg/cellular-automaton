#include "Set.hpp"

set::set()
{
    size = 0;
}

void set::add(unsigned char val)
{
    bool flag = true;
    for (int i=0; i<size; i++)
	if (cont[i] == val) flag = false;
    if (flag) {
	cont[size] = val;
	size++;
    }
}

bool set::in(unsigned char val)
{
    bool flag = false;
    for (int i=0; i<size; i++)
	if (cont[i] == val) flag = true;
    return flag;
}

bool set::del(unsigned char val)
{
    for (int i=0; i<size; i++)
	if (cont[i] == val) {
	    size--;
	    return true;
	}
    return false;
}
    
