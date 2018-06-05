#include "FrameTimer.hpp"
#include "Util.hpp"
int framesToMS(int frames){
    return 1000/frames;
}

static int fps;
static int dms;
static std::chrono::time_point<std::__1::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1LL, 1000000000LL>>> start;

void FrameTimer::initialize(int fps) {
    ::fps=fps;
    dms=framesToMS(fps);
    start = std::chrono::high_resolution_clock::now();
}

bool FrameTimer::newFrame() {
    std::chrono::time_point<std::__1::chrono::steady_clock, std::chrono::duration<long long, std::ratio<1LL, 1000000000LL>>> stop =std::chrono::high_resolution_clock::now();
    auto time = stop - start;
    start = std::chrono::high_resolution_clock::now();
    int d = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    //if (d!=0)Util::print(d);      //this right here produces the magic
    if (d >= dms) {
        return true;
    } else {
        return false;
    }
}

/*
 auto start = chrono::high_resolution_clock::now();
*/
