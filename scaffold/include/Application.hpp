#pragma once

#include <glad/gl.h>
#include <glfw3.h>

#include <vector>
#include <memory>

#include "Manifest.hpp"
#include "AppLayer.hpp"

namespace Scaffold
{
  class Application
  {
  public:
    Application(const Manifest manifest);

    void Run();
    GLFWwindow* GetWindowHandle();

    ~Application();

  public:
    template <typename T>
    T& CreateLayer(std::string name)
    {
      static_assert(std::is_base_of<AppLayer, T>::value,
        "You can only create AppLayer objects!");

      AppLayer* layer = m_activeLayers.emplace_back(std::make_shared<T>()).get();
      layer->name = name;

      T* child = dynamic_cast<T*>(layer);
      return *child;
    }

  private:
    bool m_initialized;
    Manifest m_manifest;
    GLFWwindow* m_glfwWindow;

    std::vector<std::shared_ptr<AppLayer>> m_activeLayers;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
  };
} // namespace Scaffold
