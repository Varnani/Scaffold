#pragma once

#include <glad/gl.h>
#include <glfw3.h>

#include <vector>
#include <memory>

#include <Manifest.hpp>
#include <AppLayer.hpp>
#include <Input.hpp>
#include <Profiler.hpp>

namespace Scaffold
{
  class Application
  {
  public:
    Application(const Manifest manifest);
    ~Application();

  public:
    void Run();
    GLFWwindow* GetWindowHandle();

  public:
    static Input& GetInput();
    static Profiler& GetProfiler();
    static Application& GetInstance();

  public:
    template <typename T>
    T& CreateLayer(std::string name)
    {
      static_assert(std::is_base_of<AppLayer, T>::value,
        "You can only create AppLayer objects!");

      std::shared_ptr<AppLayer> layer = std::make_shared<T>();
      layer.get()->name = name;

      m_activeLayers.push_back(layer);

      T* layerPtr = dynamic_cast<T*>(layer.get());
      return *layerPtr;
    }

  private:
    bool m_initialized;

    Manifest m_manifest;

    std::unique_ptr<Profiler> m_profiler;
    std::unique_ptr<Input> m_input;

    GLFWwindow* m_glfwWindow;

    std::vector<std::shared_ptr<AppLayer>> m_activeLayers;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
  };
} // namespace Scaffold
