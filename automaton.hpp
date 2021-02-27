#pragma once

#include "matrix.hpp"
#include "set.hpp"
#include <string>
#include <map>

namespace ca {
using statecode = u8;

class AutomatonState {
private:
    f64 color[3];
    std::string name;
    int lifespan;
public:
    statecode code;
    AutomatonState() {
        name = "Unnamed state";
        color[0] = 0.0;
        color[1] = 0.0;
        color[2] = 0.0;
        code = 0;
        lifespan = 0;
    }

    AutomatonState(statecode c, f64 r, f64 g, f64 b, std::string n, int l = 0) {
        code = c;
        color[0] = r;
        color[1] = g;
        color[2] = b;
        name = n;
        lifespan = l;
    }

    f64 *get_color(void) {
        return color;
    }

    AutomatonState(const AutomatonState &other) = default;
    AutomatonState &operator=(const AutomatonState &right) {
        if (this == &right) {
            return *this;
}
        name = right.name;
        color[0] = right.color[0];
        color[1] = right.color[1];
        color[2] = right.color[2];
        code = right.code;
        lifespan = right.lifespan;
        return *this;
    }
};

class AutomatonTransition {
private:
    std::map<statecode, Set<int>> requirements;
    statecode met_code, unmet_code;
public:
    std::string name;
    AutomatonTransition(statecode m, statecode um, std::map <statecode, Set<int>> req, std::string n) {
        met_code = m;
        unmet_code = um;
        requirements = req;
        name = n;
    }

    AutomatonTransition() {
        name = "Unnamed transition";
    }

    AutomatonTransition(const AutomatonTransition &right) = default;
    AutomatonTransition &operator = (const AutomatonTransition &right) {
        if (this == &right) {
            return *this;
        }
        name = right.name;
        requirements = right.requirements;
        met_code = right.met_code;
        unmet_code = right.unmet_code;
        return *this;
    }

    statecode &operator()(std::map<statecode, int> neighbours) {
        bool r_met = true;
        std::map<statecode, Set<int>>::iterator it = requirements.begin();
        for (; it != requirements.end(); it++) {
            statecode req_code =(*it).first;
            int neighbour_count = (*neighbours.find(req_code)).second;
            if ( !(*it).second.in(neighbour_count) ) { r_met = false;
            }
        }

        if (r_met) {
            return met_code;
        } else {
            return unmet_code;
        }
    }
};

class Automaton {
private:
    u32 width, height;
    LMatrix<statecode> field1;
    LMatrix<statecode> field2;
    LMatrix<statecode> *back;
    std::map<statecode, int> neighbours(u32 x, u32 y);
public:
    std::map<statecode, AutomatonTransition> transition;
    std::map<statecode, AutomatonState> state;
    std::map<statecode, u32> state_count;
    LMatrix<statecode> *front;

    Automaton() {
        front = nullptr;
        back = nullptr;
    }

    statecode &operator()(u32 i, u32 j) {
        return (*front)(i,j);
    }

    Automaton(u32 w, u32 h) : width(w), height(h) {
        field1 = LMatrix<statecode>(width, height);
        field2 = LMatrix<statecode>(width, height);
        front = &field1;
        back = &field2;

        for (u32 i=0; i<height; ++i) {
            for (u32 j=0; j<width; ++j) {
                field1(i,j) = field2(i,j) = 0;
            }
        }
        state_count[0] = width*height;
    }

    Automaton &operator=(const Automaton &right) {
        if (this == &right) {
            return *this;
}
        width = right.width;
        height = right.height;

        field1 = right.field1;
        field2 = right.field2;

        transition = right.transition;
        state = right.state;
        state_count = right.state_count;
        front = &field1;
        back = &field2;
        return *this;
    }

    void add_state(AutomatonState s, AutomatonTransition t);
    void clear();
    void update();
    void draw(int x, int y, statecode val);
    void randomize();
};
} // namespace ca
