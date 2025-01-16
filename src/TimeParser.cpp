#include "TimeParser.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <stdexcept>

std::time_t TimeParser::parseISO8601(const std::string& timeStr) {
    std::tm tm = {};
    std::regex isoRegex(R"((\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2})([+-]\d{2}):?(\d{2}))");
    std::smatch match;

    if (!std::regex_match(timeStr, match, isoRegex)) {
        throw std::runtime_error("Failed to parse time string: " + timeStr);
    }

    // 기본 시간 부분 파싱
    std::istringstream ss(match[1].str());
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time component: " + match[1].str());
    }

    // UTC 기준 시간 생성
    std::time_t utcTime = std::mktime(&tm);

    // 시간대 오프셋 처리
    int offsetHours = std::stoi(match[2].str());
    int offsetMinutes = std::stoi(match[3].str());
    int totalOffsetSeconds = (offsetHours * 3600) + (offsetMinutes * 60);

    if (offsetHours < 0) {
        totalOffsetSeconds = -totalOffsetSeconds;
    }

    return utcTime - totalOffsetSeconds; // UTC 시간 반환
}

std::string TimeParser::formatISO8601(std::time_t time) {
    constexpr int NEW_YORK_OFFSET_SECONDS = -3 * 3600; // 뉴욕 시간대 (-5시간)

    // UTC 기준으로 시간 변환 후 뉴욕 시간 오프셋 적용
    time += NEW_YORK_OFFSET_SECONDS;

    // UTC 기준으로 gmtime 사용
    std::tm* tm = std::gmtime(&time);
    if (!tm) {
        throw std::runtime_error("Failed to convert time_t to UTC time.");
    }

    std::ostringstream ss;
    ss << std::put_time(tm, "%Y-%m-%dT%H:%M:%S-05:00"); // 고정된 -05:00 시간대 표시
    return ss.str();
}

std::vector<std::string> TimeParser::splitRange(std::time_t start, std::time_t end, int intervalSeconds) {
    std::vector<std::string> result;
    for (std::time_t t = start; t <= end; t += intervalSeconds) {
        result.push_back(formatISO8601(t));
    }
    return result;
}
