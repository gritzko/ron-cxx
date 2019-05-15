#include "gtest/gtest.h"
#include "../../ron/ron.hpp"
#define DEBUG 1

using namespace ron;

::testing::AssertionResult IsOK(Status ok) {
    if (ok)
        return ::testing::AssertionSuccess();
    else
        return ::testing::AssertionFailure() << ok.str();
}
