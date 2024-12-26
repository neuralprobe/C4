//
// Created by jhoons on 12/25/24.
//

#include "AlpacaApi.h"
#include <curl/curl.h>
#include <iostream>

using json = nlohmann::json;

// Callback for libcurl to handle response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

AlpacaApi::AlpacaApi()
    : api_key_(ALPACA_API_KEY), api_secret_(ALPACA_API_SECRET) {}

std::string AlpacaApi::httpGet(const std::string& endpoint, const std::string& api_url_){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = api_url_ + endpoint;
        struct curl_slist* headers = nullptr;

        headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + api_key_).c_str());
        headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + api_secret_).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("HTTP GET failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return readBuffer;
}

std::string AlpacaApi::httpPost(const std::string& endpoint, const std::string& payload, const std::string& api_url_){
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = api_url_ + endpoint;
        struct curl_slist* headers = nullptr;

        headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + api_key_).c_str());
        headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + api_secret_).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("HTTP POST failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return readBuffer;
}

json AlpacaApi::getAccountInfo(){
    std::string response = httpGet("/account", ALPACA_API_BASE_URL);
    return json::parse(response);
}

json AlpacaApi::getPositions(){
    std::string response = httpGet("/positions", ALPACA_API_BASE_URL);
    return json::parse(response);
}

json AlpacaApi::getMarketData(const std::string& symbol){
    std::string response = httpGet("/assets/" + symbol, ALPACA_API_BASE_URL);
    return json::parse(response);
}

nlohmann::json AlpacaApi::getAssetsByClass(const std::string& assetClass) {
    std::string endpoint = "/assets";
    endpoint += "?asset_class=" + assetClass;

    std::string response = httpGet(endpoint, ALPACA_API_BASE_URL);
    return nlohmann::json::parse(response);
}

nlohmann::json AlpacaApi::getTimeData(const std::string& symbol,
    const std::string& timeframe,
    const std::string& start,
    const std::string& end,
    const std::string& adjustment,
    const std::string& feed){
    std::string endpoint = "/stocks/" + symbol + "/bars";
    endpoint += "?timeframe=" + timeframe +
        "&start=" + start +
        "&end=" + end +
        "&adjustment=" + adjustment +
        "&feed=" + feed;

    std::string response = httpGet(endpoint, ALPACA_API_DATA_URL);
    return nlohmann::json::parse(response);
}

nlohmann::json AlpacaApi::getLatestTrades(const std::string& symbol, const std::string& feed) {
    std::string endpoint = "/stocks/" + symbol + "/trades/latest";
    endpoint += "?feed=" + feed;
    std::string response = httpGet(endpoint, ALPACA_API_DATA_URL);
    return nlohmann::json::parse(response);
}

json AlpacaApi::placeOrder(const std::string& symbol,
    int qty,
    const std::string& side,
    const std::string& type,
    const std::string& time_in_force){
    json orderPayload = {
        {"symbol", symbol},
        {"qty", qty},
        {"side", side},
        {"type", type},
        {"time_in_force", time_in_force}
    };
    std::string response = httpPost("/orders", orderPayload.dump(),ALPACA_API_BASE_URL);
    return json::parse(response);
}