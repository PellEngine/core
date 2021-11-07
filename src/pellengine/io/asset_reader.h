#ifndef _PELLENGINE_IO_ASSET_READER_H_
#define _PELLENGINE_IO_ASSET_READER_H_

#include <vector>
#include <string>

namespace pellengine {

class AssetReader {
 public:
  AssetReader() {}
  virtual ~AssetReader() {}
  virtual size_t getFileLength(std::string fileName) = 0;
  virtual void getFileBuffer(std::string fileName, std::vector<char>& data) = 0;
};

}

#endif