//
// Created by jhoons on 12/25/24.
//

#ifndef ALPACAAPI_H
#define ALPACAAPI_H

#include <string>
#include <nlohmann/json.hpp>

#define ALPACA_API_BASE_URL "https://paper-api.alpaca.markets/v2"
#define ALPACA_API_DATA_URL "https://data.alpaca.markets/v2"
#define ALPACA_API_KEY "PKPJ63YF491BRVT49TXO"
#define ALPACA_API_SECRET "5UMF3LUloDTAUsyRkQ6SGNjEwZzC6bvP9gEAy9VH"

class AlpacaApi {
public:
    AlpacaApi();

    nlohmann::json getAccountInfo();
    nlohmann::json getPositions();
    nlohmann::json getMarketData(const std::string& symbol);
    nlohmann::json getAssetsByClass(const std::string& assetClass);
    nlohmann::json getTimeData(const std::string& symbol, const std::string& timeframe, const std::string& start, const std::string& end, const std::string& adjustment, const std::string& feed);
    nlohmann::json getLatestTrades(const std::string& symbol, const std::string& feed);
    nlohmann::json placeOrder(const std::string& symbol, int qty, const std::string& side, const std::string& type, const std::string& time_in_force);

private:
    std::string api_key_;
    std::string api_secret_;
    const std::string api_base_url_ = ALPACA_API_BASE_URL;

    std::string httpGet(const std::string& endpoint, const std::string& api_url_);
    std::string httpPost(const std::string& endpoint, const std::string& payload, const std::string& api_url_);
};

#endif //ALPACAAPI_H
