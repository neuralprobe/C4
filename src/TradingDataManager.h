#ifndef TRADING_DATA_MANAGER_H
#define TRADING_DATA_MANAGER_H

#include "AlpacaApi.h"
#include "DatabaseManager.h"
#include <map>
#include <string>

class TradingDataManager {
public:
    TradingDataManager(AlpacaApi& api, DatabaseManager& db);
    nlohmann::json getTimeData(const std::string& symbol, const std::string& start, const std::string& end, const std::string& timeframe);

private:
    AlpacaApi& api_;
    DatabaseManager& db_;
    std::map<std::string, nlohmann::json> cache_;

    nlohmann::json filterCachedData(const nlohmann::json& data, const std::string& start, const std::string& end);
};

#endif // TRADING_DATA_MANAGER_H
