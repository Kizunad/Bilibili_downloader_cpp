#define DEBUG

#include <client.h>
#include <get_url.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <write_vid.hpp>

using json = nlohmann::json;

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <bvid>" << std::endl;
    return 1;
  }
  Client c;
  c.url = "https://api.bilibili.com/x/web-interface/view?bvid=" +
          std::string(argv[1]);
  c.args.push_back("headers");
  c.hdrs.push_back(
      "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 "
      "(KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");

  // 获取响应内容
  std::string response = c.get();
  int cid = 0;
  // 使用 nlohmann::json 解析
  try {
    json j = json::parse(response);

    // 检查是否有 "data" 和 "pages"
    if (j.contains("data") && j["data"].contains("pages")) {
      for (const auto &page : j["data"]["pages"]) {
        if (page.contains("cid")) {
          std::cout << "cid: " << page["cid"] << std::endl;
          cid = page["cid"];
        }
      }
    } else {
      std::cerr << "No 'data' or 'pages' field in the JSON response"
                << std::endl;
    }
  } catch (json::parse_error &e) {
    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
  }

  url_t url;
  std::string video_url = url.get_cid_url((std::to_string(cid)).c_str(), "64");
  std::cout << video_url << std::endl;

  c.hdrs.push_back("Accept: */*");
  c.hdrs.push_back("Accept-Language: en-US,en;q=0.5");
  c.hdrs.push_back("Accept-Encoding: gzip, deflate, br");
  c.hdrs.push_back("Range: bytes=0-");
  c.hdrs.push_back("Referer: " + url.get_api_url(argv[1]));
  c.hdrs.push_back("Origin: https://www.bilibili.com");
  c.hdrs.push_back("Connection: keep-alive");

  response = c.get();

  if(write_v("a.mp4", response.c_str(), response.size())){
	  return 1;
  }
  return 0;
}
