#ifndef _PELLENGINE_IO_IO_LOCATOR_H_
#define _PELLENGINE_IO_IO_LOCATOR_H_

#include <memory>
#include "asset_reader.h"

namespace pellengine {

class IOLocator {
 public:
  IOLocator(const IOLocator&) = delete;
  IOLocator& operator=(const IOLocator&) = delete;
  static IOLocator* instance();

  static std::shared_ptr<AssetReader> getAssetReader();
  static void provide(std::shared_ptr<AssetReader> assetReader);

 private:
  IOLocator() {}
  static IOLocator* _instance;

  std::shared_ptr<AssetReader> assetReader_;
};

}

#endif