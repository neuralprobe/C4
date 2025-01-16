#include "TradingDataManager.h"
#include "TimeParser.h"

TradingDataManager::TradingDataManager(AlpacaApi& api, DatabaseManager& db)
    : api_(api), db_(db) {}

#include <iostream>

nlohmann::json TradingDataManager::getTimeData(
    const std::string& symbol,
    const std::string& start,
    const std::string& end,
    const std::string& timeframe) {

    std::time_t startTime = TimeParser::parseISO8601(start);
    std::time_t endTime = TimeParser::parseISO8601(end);
    int interval = (timeframe == "1Min") ? 60 : 3600; // 분봉은 60초, 시간봉은 3600초 간격

    // 모든 분 단위 시간을 생성
    auto allTimes = TimeParser::splitRange(startTime, endTime, interval);

    // 데이터베이스에서 데이터 가져오기
    std::vector<std::string> missingIntervals;
    nlohmann::json dbData = nlohmann::json::array();

    for (const auto& time : allTimes) {
        auto data = db_.getData(symbol, time, time);
        if (!data.empty()) {
            for (const auto& item : data) {
                dbData.push_back(item);
            }
            std::cout << "[Database] Retrieved data for time: " << time << std::endl;
        } else {
            missingIntervals.push_back(time);
        }
    }

    // 누락된 구간을 start-end 페어로 생성
    std::vector<std::pair<std::string, std::string>> apiIntervals;
    if (!missingIntervals.empty()) {
        std::string currentStart = missingIntervals[0];
        for (size_t i = 1; i < missingIntervals.size(); ++i) {
            // 연속된 시간인지 확인
            if (TimeParser::parseISO8601(missingIntervals[i]) -
                TimeParser::parseISO8601(missingIntervals[i - 1]) > interval) {
                // 새로운 interval 시작
                apiIntervals.emplace_back(currentStart, missingIntervals[i - 1]);
                currentStart = missingIntervals[i];
            }
        }
        // 마지막 구간 추가
        apiIntervals.emplace_back(currentStart, missingIntervals.back());
    }

    // API 요청 및 데이터베이스 저장
    nlohmann::json apiData = nlohmann::json::array();
    for (const auto& interval : apiIntervals) {
        std::cout << "[API] Requesting data for interval: "
                  << interval.first << " to " << interval.second << std::endl;

        auto fetchedData = api_.getTimeData(
            symbol,
            timeframe,
            interval.first,
            interval.second,
            "split",
            "sip"
        );

        std::cout << "TimeData Info:\n" << fetchedData.dump(4) << std::endl;

        for (const auto& data : fetchedData) {
            apiData.push_back(data);
            db_.insertData(symbol, data["t"].get<std::string>(), data);
        }
    }

    // 결과 병합
    nlohmann::json result = dbData;
    for (const auto& item : apiData) {
        result.push_back(item);
    }

    // 캐시에 저장
    cache_[symbol] = result;

    return result;
}


nlohmann::json TradingDataManager::filterCachedData(
    const nlohmann::json& data,
    const std::string& start,
    const std::string& end) {

    nlohmann::json result = nlohmann::json::array();
    std::time_t startTime = TimeParser::parseISO8601(start);
    std::time_t endTime = TimeParser::parseISO8601(end);

    // 캐시 데이터에서 시간 범위 필터링
    for (const auto& item : data) {
        std::time_t itemTime = TimeParser::parseISO8601(item["t"].get<std::string>());
        if (itemTime >= startTime && itemTime <= endTime) {
            result.push_back(item);
        }
    }

    return result;
}
