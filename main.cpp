#include "life.hpp"

int main(int argc, char **argv)
{
    using namespace ca;

    if (argc == 1) {
    LApp Life(150, 150, "23/3");
    return Life.Execute();
    } else if (argc == 4) {
    u32 x = static_cast<u32>(atoi(argv[1]));
    u32 y = static_cast<u32>(atoi(argv[2]));
    LApp Life(x, y, argv[3]);
    return Life.Execute();
    } else {
    LApp Life(150, 150, argv[1]);
    return Life.Execute();
    }
}
