# 패키지 검색
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

# 라이브러리 생성
add_library(AlpacaApi AlpacaApi.cpp)
target_link_libraries(AlpacaApi PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# 실행 파일 생성
set(SOURCES
        main.cpp
        TimeParser.cpp
        DatabaseManager.cpp
        TradingDataManager.cpp
)

add_executable(TradingMain ${SOURCES})
target_include_directories(TradingMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(TradingMain PRIVATE SQLite::SQLite3 CURL::libcurl nlohmann_json::nlohmann_json AlpacaApi)

# 빌드 출력 디렉토리 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)