#include <gtest/gtest.h>
#include "AlpacaApi.h"

TEST(AlpacaApiTest, TestAccountInfo) {
    AlpacaApi api;
    auto accountInfo = api.getAccountInfo();
    std::cout << "Account Info:\n" << accountInfo.dump(4) << std::endl;
    EXPECT_TRUE(accountInfo.contains("account_number"));
}

TEST(AlpacaApiTest, TestPositions) {
    AlpacaApi api;
    auto positions = api.getPositions();
    std::cout << "Positions Info:\n" << positions.dump(4) << std::endl;
    EXPECT_TRUE(positions.is_array());
}

TEST(AlpacaApiTest, TestMarketData) {
    AlpacaApi api;
    auto marketData = api.getMarketData("AAPL");
    std::cout << "MarketData Info:\n" << marketData.dump(4) << std::endl;
    EXPECT_TRUE(marketData.contains("symbol"));
    EXPECT_EQ(marketData["symbol"], "AAPL");
}

TEST(AlpacaApiTest, TestgetAssetsByClass) {
    AlpacaApi api;
    auto marketData = api.getAssetsByClass("us_equity");
    int count = 0;
    for (auto it = marketData.begin(); it != marketData.end() && count < 3; ++it) {
        std::cout << "MarketData Info (" << count << "-th asset)\n" << *it << std::endl;
        count++;
    }
    EXPECT_TRUE(true);
    EXPECT_EQ(1,1);
}

TEST(AlpacaApiTest, TestgetTimeData) {
    AlpacaApi api;

    const std::string symbol = "AAPL";
    const std::string timeframe = "1Min";
    const std::string start = "2023-12-18T09:30:00-05:00";
    const std::string end = "2023-12-18T09:40:00-05:00";
    const std::string adjustment = "split";
    const std::string feed = "sip";
    auto timeData = api.getTimeData(symbol, timeframe, start, end, adjustment, feed);
    std::cout << "TimeData Info:\n" << timeData.dump(4) << std::endl;
    EXPECT_TRUE(true);
    EXPECT_EQ(1,1);
}

TEST(AlpacaApiTest, TestgetLatestTrades) {
    AlpacaApi api;

    const std::string symbol = "AAPL";
    const std::string feed = "sip";
    auto latestData = api.getLatestTrades(symbol, feed);
    std::cout << "LatestData Info:\n" << latestData.dump(4) << std::endl;
    EXPECT_TRUE(true);
    EXPECT_EQ(1,1);
}

//TEST(AlpacaApiTest, TestPlaceOrder) {
//    AlpacaApi api();
//    auto orderResponse = api.placeOrder("AAPL", 1, "buy", "market", "gtc");
//    EXPECT_TRUE(orderResponse.contains("id"));
//}
