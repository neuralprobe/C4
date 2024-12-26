enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_alpaca_api test_alpaca_api.cpp)
target_link_libraries(test_alpaca_api PRIVATE GTest::GTest GTest::Main AlpacaApi)

add_test(NAME AlpacaTests COMMAND test_alpaca_api)
