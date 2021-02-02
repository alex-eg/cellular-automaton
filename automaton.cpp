#include "automaton.hpp"
#include <iostream>

namespace ca {
void Automaton::update(void) {
    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            state_count[(*front)(i, j)]--;
            (*back)(i, j) =
                transition[state[(*front)(i, j)].code](neighbours(i, j));
            state_count[(*back)(i, j)]++;
        }
    }
    LMatrix<statecode> *temp;
    temp = front;
    front = back;
    back = temp;
}

std::map<statecode, int> Automaton::neighbours(u32 x, u32 y) {
    std::map<statecode, int> ret;

    u32 cx[3] = { x - 1, x, x + 1 };
    u32 cy[3] = { y - 1, y, y + 1 };

    if (x == 0) {
        cx[0] = height - 1;
    } else if (x == height - 1) {
        cx[2] = 0;
    }

    if (y == 0) {
        cy[0] = width - 1;
    } else if (y == width - 1) {
        cy[2] = 0;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!((cx[i] == x) && (cy[j] == y))) {
                statecode curr_state = (*front)(cx[i], cy[j]);
                if (ret.find(curr_state) == ret.end()) {
                    ret[curr_state] = 1;
                } else {
                    ret[curr_state]++;
                }
            }
        }
    }
    return ret;
}

void Automaton::draw(int x, int y, statecode val) {
    state_count[(*front)(x, y)]--;
    (*front)(x, y) = val;
    state_count[(*front)(x, y)]++;
}

void Automaton::randomize() {
    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            state_count[(*front)(i, j)]--;
            (*front)(i, j) = rand() % 2;
            state_count[(*front)(i, j)]++;
        }
    }
}

void Automaton::clear() {
    for (u32 i = 0; i < height; i++) {
        for (u32 j = 0; j < width; j++) {
            state_count[1] = 0;
            (*front)(i, j) = 0;
            state_count[0] = width * height;
        }
    }
}

void Automaton::add_state(AutomatonState s, AutomatonTransition t) {
    state[s.code] = s;
    transition[s.code] = t;
}
} // namespace ca
