#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    void createTable();
    void insertData(const std::string& symbol, const std::string& time, const nlohmann::json& data);
    std::vector<nlohmann::json> getData(const std::string& symbol, const std::string& start, const std::string& end);

private:
    sqlite3* db_;
    void exec(const char* sql);
};

#endif // DATABASE_MANAGER_H
