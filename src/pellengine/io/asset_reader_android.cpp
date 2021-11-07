#ifdef ANDROID
#include "asset_reader_android.h"

namespace pellengine {

AssetReaderAndroid::AssetReaderAndroid(AAssetManager* androidAssetManager) : AssetReader(), androidAssetManager(androidAssetManager) {}
AssetReaderAndroid::~AssetReaderAndroid() {}

size_t AssetReaderAndroid::getFileLength(std::string fileName) {
  AAsset* file = AAssetManager_open(androidAssetManager, fileName.c_str(), AASSET_MODE_BUFFER);
  size_t size = AAsset_getLength(file);
  AAsset_close(file);
  return size;
}

void AssetReaderAndroid::getFileBuffer(std::string fileName, std::vector<char>& data) {
  AAsset* file = AAssetManager_open(androidAssetManager, fileName.c_str(), AASSET_MODE_BUFFER);
  size_t fileLength = AAsset_getLength(file);
  AAsset_read(file, data.data(), fileLength);
  AAsset_close(file);
}

}

#endif