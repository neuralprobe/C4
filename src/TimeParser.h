#ifndef TIME_PARSER_H
#define TIME_PARSER_H

#include <string>
#include <vector>
#include <ctime>

class TimeParser {
public:
    static std::time_t parseISO8601(const std::string& timeStr); // ISO8601 문자열을 파싱하여 뉴욕 시간대의 time_t 반환
    static std::string formatISO8601(std::time_t time); // time_t를 ISO8601 형식으로 포맷팅
    static std::string formatISO8601_NY(std::time_t time); // time_t를 ISO8601 형식으로 포맷팅 (뉴욕 시간대 적용)
    static std::vector<std::string> splitRange(std::time_t start, std::time_t end, int intervalSeconds); // 주어진 시간 범위를 interval 단위로 나눔 (뉴욕 시간대)
};

#endif // TIME_PARSER_H
