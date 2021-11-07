#ifdef ANDROID
#ifndef _PELLENGINE_IO_ASSET_READER_ANDROID_H_
#define _PELLENGINE_IO_ASSET_READER_ANDROID_H_

#include "asset_reader.h"
#include <string>
#include <android/native_activity.h>

namespace pellengine {

class AssetReaderAndroid : public AssetReader {
 public:
  AssetReaderAndroid(AAssetManager* androidAssetManager);
  ~AssetReaderAndroid();
  
  virtual size_t getFileLength(std::string fileName) override;
  virtual void getFileBuffer(std::string fileName, std::vector<char>& data) override;

 private:
  AAssetManager* androidAssetManager;
};

}

#endif
#endif