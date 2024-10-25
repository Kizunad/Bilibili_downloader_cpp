#include "client.h"
#include <curl/curl.h>
#include <iostream>
#include <string>

#define DEBUG
size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

std::string Client::get() {
  CURL *curl;
  CURLcode res;
  std::string buffer = "";
  struct curl_slist *headers = NULL;
  // Initilization
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (!curl) {
    std::cerr << "curl_easy_init failed" << std::endl;
    goto free_resources;
  }

  if (this->url.empty()) {
    std::cerr << "Where is the URL??" << std::endl;
    goto free_resources;
  }

  for (const auto &arg : this->args) {
    if (arg == "headers") {
      for (const auto &str : this->hdrs) {
#ifdef DEBUG
        std::cout << str << std::endl;
#endif // DEBUG
        headers = curl_slist_append(headers, str.c_str());
      }
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
  }

  curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

  if (res = curl_easy_perform(curl); res != CURLE_OK) {
    std::cerr << "curl_easy_perform failed: " << curl_easy_strerror(res)
              << std::endl;
    goto free_resources;
  }

// Free resources
free_resources:
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
#ifdef DEBUG
  std::cout << buffer << std::endl;
#endif // DEBUG
  return buffer;
}
