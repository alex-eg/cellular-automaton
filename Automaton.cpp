#include "Life.hpp"
#include <iostream>

void Automaton::Update(void)
{
    for (int i=0; i<height; i++)
	for (int j=0; j<width; j++)
	    (*back)(i,j) = Transition[State[(*front)(i,j)].code](Neighbours(i,j));
    LMatrix <statecode> *temp;
    temp = front;
    front = back;
    back = temp;
}

std::map <statecode, int> Automaton::Neighbours(int x, int y)
{
    std::map <statecode, int> ret;

    int cx[3] = {x-1, x, x+1};
    int cy[3] = {y-1, y, y+1};

    if (x == 0) cx[0] = height -1;
    else if (x == height - 1) cx[2] = 0;

    if (y == 0) cy[0] = width -1;
    else if (y == width - 1) cy[2] = 0;

    for (int i=0; i<3; i++)
	for (int j=0; j<3; j++)
	    if (!((cx[i] == x) && (cy[j] == y))) {
		    statecode curr_state = (*front)(cx[i],cy[j]);
		    if (ret.find(curr_state) == ret.end()) ret[curr_state] = 1;
		    else ret[curr_state]++;
		}
    return ret;
}

void Automaton::Draw(int x, int y, statecode val)
{
    (*front)(x,y) = val;
}

void Automaton::Randomize()
{
    for (int i=0; i<height; i++)
	for (int j=0; j<width; j++)
	    (*front)(i,j) = rand() % 2;
}

void Automaton::Clear()
{
    for (int i=0; i<height; i++)
	for (int j=0; j<width; j++)
	    (*front)(i,j) = 0;
}

void Automaton::AddState(AutomatonState s, AutomatonTransition t)
{
    State[s.code] = s;
    Transition[s.code] = t;
}
