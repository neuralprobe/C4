#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbPath) {
    if (sqlite3_open(dbPath.c_str(), &db_)) {
        throw std::runtime_error("Failed to open database");
    }
    createTable();
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db_);
}

void DatabaseManager::createTable() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS StockData (
            Symbol TEXT,
            Time TEXT,
            Data TEXT,
            PRIMARY KEY (Symbol, Time)
        );
    )";
    exec(sql);
}

void DatabaseManager::insertData(const std::string& symbol, const std::string& time, const nlohmann::json& data) {
    std::string sql = "INSERT OR IGNORE INTO StockData (Symbol, Time, Data) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        throw std::runtime_error("Failed to prepare statement for insertData.");
    }
    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, time.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, data.dump().c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db_) << std::endl;
        throw std::runtime_error("Failed to execute statement for insertData.");
    }
    sqlite3_finalize(stmt);
}


std::vector<nlohmann::json> DatabaseManager::getData(
    const std::string& symbol, const std::string& start, const std::string& end) {
    std::vector<nlohmann::json> results;
    std::string sql = "SELECT Data FROM StockData WHERE Symbol = ? AND Time BETWEEN ? AND ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        throw std::runtime_error("Failed to prepare statement for getData.");
    }
    sqlite3_bind_text(stmt, 1, symbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, start.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, end.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            results.push_back(nlohmann::json::parse(reinterpret_cast<const char*>(text)));
        }
    }
    if (results.empty()) {
        std::cerr << "No data found for Symbol: " << symbol
                  << " between " << start << " and " << end << std::endl;
    }
    sqlite3_finalize(stmt);
    return results;
}



void DatabaseManager::exec(const char* sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "SQLite Error: ";
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}
