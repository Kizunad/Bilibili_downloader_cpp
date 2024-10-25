#ifndef URL_H
#define URL_H
#include <bilibili_video.h>
#include <iomanip>
#include <md5.h>
#include <sstream>
#include <string>

constexpr const char *entropy =
    "rbMCKn@KuamXWlPMoJGsKcbiJKUfkPF_8dABscJntvqhRSETg";

struct url_t {
  std::string api_url;
  std::string api_cid_url;
  std::string api_video_url;

private:
public:
  std::string get_api_url(const char *bv) {
    return std::string("https://api.bilibili.com/x/web-interface/view?bvid=") +
           bv;
  }

  std::string get_cid_url(const char *cid, const char *quality) {
    std::string yportne;
    std::string appkey;
    std::string sec;

    // Reverse entropy and then split it
    for (int i = 0; i < std::strlen(entropy); ++i) {
      yportne += entropy[std::strlen(entropy) - 1 - i];
    }

    // Extract appkey and sec
    for (int i = 0; i < yportne.size() / 2; ++i) {
      appkey += char(yportne[i] + 2);
      sec += char(yportne[yportne.size() / 2 + i] + 2);
    }

    // Prepare URL params
    std::ostringstream oss;
    oss << "appkey=" << appkey << "&cid=" << std::string(cid)
        << "&otype=json&qn=" << std::string(quality)
        << "&quality=" << std::string(quality) << "&type=";

    std::string checksum = MD5(oss.str() + sec).toStr();
    return "https://interface.bilibili.com/v2/playurl?" + oss.str() +
           "&sign=" + checksum;
  }
};

#endif
