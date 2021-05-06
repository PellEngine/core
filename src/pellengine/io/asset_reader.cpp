#include "asset_reader.h"

namespace pellengine {

AssetReader* AssetReader::singleton_ = nullptr;

AssetReader* AssetReader::getInstance() {
  if(singleton_ == nullptr) {
    singleton_ = new AssetReader();
  }

  return singleton_;
}

AssetReader::AssetReader() {}

size_t AssetReader::getFileLength(std::string filename) {
  #ifdef ANDROID

  AAsset* file = AAssetManager_open(getInstance()->androidAssetManager, filename.c_str(), AASSET_MODE_BUFFER);
  size_t size = AAsset_getLength(file);
  AAsset_close(file);
  return size;

  #endif
}

void AssetReader::getFileBuffer(std::string filename, std::vector<char>& data) {
  #ifdef ANDROID

  AAsset* file = AAssetManager_open(getInstance()->androidAssetManager, filename.c_str(), AASSET_MODE_BUFFER);
  size_t fileLength = AAsset_getLength(file);
  AAsset_read(file, data.data(), fileLength);
  AAsset_close(file);
  return;

  #endif
}
 
}