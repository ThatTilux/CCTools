#include "gtest/gtest.h"
#include "Logger.hh"

namespace CCTools {

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    Logger::deactivate();
    return RUN_ALL_TESTS();
}

} // namespace CCTools
