#include "gtest/gtest.h"
#include "../fs.hpp"
#define DEBUG 1

namespace ron {

template<class Frame>
Status Compare(const Frame& a, const Frame& b) {
    Status ok = CompareFrames(a, b);
    if (!ok) {
        cerr<<"FRAME A:\n"<<a.data()<<".\n";
        cerr<<"FRAME B:\n"<<b.data()<<".\n";
    }
    return ok;
}

::testing::AssertionResult IsOK(Status ok) {
    if (ok)
        return ::testing::AssertionSuccess();
    else
        return ::testing::AssertionFailure() << ok.str();
}


}