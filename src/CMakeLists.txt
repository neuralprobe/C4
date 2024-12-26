add_library(AlpacaApi AlpacaApi.cpp)

find_package(CURL REQUIRED)
target_link_libraries(AlpacaApi PRIVATE CURL::libcurl)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(AlpacaApi PRIVATE nlohmann_json::nlohmann_json)

