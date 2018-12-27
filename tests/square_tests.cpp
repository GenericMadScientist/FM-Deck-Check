#include "gtest/gtest.h"

#include "square.h"

TEST(SquareTest, HandlesZeroInput)
{
    ASSERT_EQ(square(0), 0);
}
