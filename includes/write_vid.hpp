#ifndef WV_H
#define WV_H

#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

constexpr auto DOWNLOAD_FILE_PATH = "./downloaded_videos/";

int write_v(const char *filepath, const char *binary, std::size_t binary_size) {
  if (!filepath) {
    std::cerr << "Filepath doesn't exist!" << std::endl;
    return 1;
  }

  struct stat info;
  if (stat(DOWNLOAD_FILE_PATH, &info) != 0 || (!(info.st_mode & S_IFDIR))) {
    if (mkdir(DOWNLOAD_FILE_PATH, 0777) != 0) {
      std::cerr << "Failed to create directory: " << DOWNLOAD_FILE_PATH
                << std::endl;
      return 1;
    }
  }

  // 拼接文件路径
  std::string fullPath = std::string(DOWNLOAD_FILE_PATH) + filepath;
std::ofstream writeFile(fullPath, std::ios::binary);
  if ( !writeFile) {
    std::cerr << "Couldn't open file for writing: " << fullPath << std::endl;
    return 1;
  }

  // 写入二进制数据
  if (writeFile.write(binary, binary_size); !writeFile) {
    std::cerr << "Failed in writing data to file" << std::endl;
    return 1;
  }

  writeFile.close();

  std::cout << "Successfully wrote to " << fullPath << std::endl;

  return 0;
}

#endif
