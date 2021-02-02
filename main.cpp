#include "life.hpp"

int main(int argc, char **argv) {
    using namespace ca;

    if (argc == 1) {
        LApp life(150, 150, "23/3");
        return life.execute();
    } else if (argc == 4) {
        u32 x = static_cast<u32>(atoi(argv[1]));
        u32 y = static_cast<u32>(atoi(argv[2]));
        LApp life(x, y, argv[3]);
        return life.execute();
    } else {
        LApp life(150, 150, argv[1]);
        return life.execute();
    }
}
