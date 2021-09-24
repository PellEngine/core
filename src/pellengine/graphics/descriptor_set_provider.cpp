#include "descriptor_set_provider.h"

namespace pellengine {

VkDescriptorSetLayoutBinding DescriptorSetProvider::getDescriptorSetLayoutBinding() {
  return descriptorSetLayoutBinding;
}

}