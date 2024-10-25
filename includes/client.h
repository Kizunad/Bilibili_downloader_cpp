#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <string>

struct Client {
  std::vector<std::string> args;
  std::vector<std::string> argv;
  std::string url;
  std::vector<std::string> hdrs;

public:
  std::string get();
};

#endif
