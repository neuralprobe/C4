#include "AlpacaApi.h"
#include "DatabaseManager.h"
#include "TradingDataManager.h"
#include <iostream>

int main() {
    AlpacaApi api;
    DatabaseManager db("trading_data.db");
    TradingDataManager manager(api, db);

    std::string start = "2023-12-18T09:30:00-05:00";
    std::string end = "2023-12-18T09:40:00-05:00";

    nlohmann::json data = manager.getTimeData("AAPL", start, end, "1Min");
    std::cout << data.dump(4) << std::endl;

    start = "2023-12-18T09:30:00-05:00";
    end = "2023-12-18T09:40:00-05:00";

    nlohmann::json data2 = manager.getTimeData("AAPL", start, end, "1Min");
    std::cout << data2.dump(4) << std::endl;

    return 0;
}
