#include "gtest/gtest.h"
#include "log.hpp"

bool func() {
    ENSURE_OR_RETURN(true == false,false);
}

TEST(Logger, BasicUsage)
{
    LOG_DBG("{} {} test","test",23);
    LOG_INF("{} {} test","test",23);
    LOG_WRN("{} {} test","test",23);
    LOG_ERR("{} {} test","test",23);
    EXPECT_EQ(func(),false);
    ENSURE_OR_RETURN(true == false,);
}