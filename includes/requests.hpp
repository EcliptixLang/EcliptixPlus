#pragma once
#include <string>
#include <cpr/cpr.h>
#include <map>
#include <stdexcept>
#include <iostream>

struct Response {
    
};

#include <cpr/cpr.h>

int main(int argc, char** argv) {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/whoshuu/cpr/contributors"},
                      cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
                      cpr::Parameters{{"anon", "true"}, {"key", "value"}});
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    return 0;
}

class Requests {
public:
    Requests();
    ~Requests();

    void setUrl(const std::string& url);
    void addHeader(const std::string& header);
    std::string get();
    std::string post(const std::string& data);
    std::string del();

private:

    CURL* curl;
    struct curl_slist* headers;
    std::string url;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void initCurl();
};
