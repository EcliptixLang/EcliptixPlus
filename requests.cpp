#include <requests.hpp>

Requests::Requests() : curl(nullptr), headers(nullptr) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize libcurl");
    }
    headers = nullptr;
}

Requests::~Requests() {
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void Requests::setUrl(const std::string& url) {
    this->url = url;
}

void Requests::addHeader(const std::string& header) {
    headers = curl_slist_append(headers, header.c_str());
}

size_t Requests::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), realSize);
    return realSize;
}

void Requests::initCurl() {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
}

std::string Requests::get() {
    if (url.empty()) {
        throw std::invalid_argument("URL is not set");
    }

    std::string response;
    initCurl();

    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        throw std::runtime_error("GET request failed: " + std::string(curl_easy_strerror(res)));
    }

    return response;
}

std::string Requests::post(const std::string& data) {
    if (url.empty()) {
        throw std::invalid_argument("URL is not set");
    }

    std::string response;
    initCurl();

    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    curl_easy_setopt(curl, CURLOPT_POST, 1L); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);    

    CURLcode res = curl_easy_perform(curl); 
    if (res != CURLE_OK) {      
        throw std::runtime_error("POST request failed: " + std::string(curl_easy_strerror(res)));
    }

    return response;
}

std::string Requests::del() {
    if (url.empty()) {
        throw std::invalid_argument("URL is not set");
    }

    std::string response;
    initCurl();  

    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);  
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);  

    CURLcode res = curl_easy_perform(curl);  
    if (res != CURLE_OK) {
        throw std::runtime_error("DELETE request failed: " + std::string(curl_easy_strerror(res)));
    }

    return response;
}