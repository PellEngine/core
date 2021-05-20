#ifndef _PELLENGINE_GRAPHICS_COMMAND_BUFFER_H_
#define _PELLENGINE_GRAPHICS_COMMAND_BUFFER_H_

#include <pellengine/vulkan/vulkan_wrapper.h>
#include <pellengine/graphics/window.h>
#include <pellengine/graphics/graphics_pipeline.h>
#include <optional>
#include <memory>

namespace pellengine {

struct PipelineConfiguration {
  std::optional<GraphicsPipeline*> graphicsPipeline;
  VkPipelineBindPoint bindPoint;

  static const PipelineConfiguration generateGraphicsPipelineConfiguration(std::shared_ptr<GraphicsPipeline> pipeline) {
    PipelineConfiguration configuration{};
    configuration.graphicsPipeline = pipeline.get();
    configuration.bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    return configuration;
  }
};

class CommandBuffer {
 public:
  CommandBuffer(std::shared_ptr<Window> window, PipelineConfiguration pipelineConfiguration);
  virtual ~CommandBuffer();
  
  CommandBuffer(const CommandBuffer&) = delete;
  CommandBuffer& operator=(const CommandBuffer&) = delete;

  void initialize();
  void terminate();
  void recordAll();
  void record(uint32_t imageIndex);

  std::vector<VkCommandBuffer>& getCommandBuffers() {
    return commandBuffers;
  }

 private:
  bool initialized = false;

 protected:
  std::shared_ptr<Window> window;
  PipelineConfiguration pipelineConfiguration;
  std::vector<VkCommandBuffer> commandBuffers;

  virtual void draw(VkCommandBuffer& buffer) = 0;
};

}

#endif