#ifndef _PELLENGINE_IO_ASSET_READER_H_
#define _PELLENGINE_IO_ASSET_READER_H_

#include <vector>
#include <string>

#ifdef ANDROID
  #include <android/native_activity.h>
#endif

namespace pellengine {

class AssetReader {
 public:
  AssetReader(const AssetReader&) = delete;
  AssetReader &operator=(const AssetReader&) = delete;
  
  static size_t getFileLength(std::string filename);
  static void getFileBuffer(std::string filename, std::vector<char>& data);

  static AssetReader* getInstance();

  #ifdef ANDROID
    AAssetManager* androidAssetManager;
  #endif
  
 protected:
  AssetReader();
  static AssetReader* singleton_;
};

}

#endif