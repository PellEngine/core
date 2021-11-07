#include "io_locator.h"

namespace pellengine {

IOLocator* IOLocator::_instance = nullptr;
IOLocator* IOLocator::instance() {
  if(_instance == nullptr) {
    _instance = new IOLocator();
  }

  return _instance;
}

std::shared_ptr<AssetReader> IOLocator::getAssetReader() {
  assert(instance()->assetReader_.get() != nullptr);
  return instance()->assetReader_;
}

void IOLocator::provide(std::shared_ptr<AssetReader> assetReader) {
  instance()->assetReader_ = assetReader;
}

}