#pragma once

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string_view>

#include "application/error.h"
#include "tools/vec3.h"

bool isValid(const std::string_view& line) {
  return !line.empty() && line[0] == 'v' && std::isspace(line[1]);
}

class WavefrontReader {
 public:
  WavefrontReader(const std::string_view& file = "") { openFile(file); }

  void parseInput() {
    if (m_inputStream.is_open()) {
      std::string line;
      while (std::getline(m_inputStream, line)) {
        if (isValid(line)) {
          parse(line);
        }
      }
    }
    m_inputStream.close();
  }

  void openFile(const std::string_view& file) {
    if (!file.empty()) {
      m_inputStream.open(file.data());
    }
  }

  std::vector<Vec3f> vertexCollection() const { return m_outVector; }

 private:
  void parse(const std::string_view& line) {
    std::istringstream ss(line.data());
    std::string v, x, y, z;
    if (ss >> v >> x >> y >> z) {
      try {
        m_outVector.push_back(Vec3f(stof(x), stof(y), stof(z)));
      } catch (...) {
        APP_MSG(line.data());
        APP_ASSERT(false,
                   "Error while parsing the line above in wavefront file!");
      }
    }
  }

  std::ifstream m_inputStream;
  std::vector<Vec3f> m_outVector;
};
