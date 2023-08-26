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

    void Run();

    ~Application();

  public:
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

      AppLayer* layer = m_activeLayers.emplace_back(std::make_shared<T>()).get();
      layer->name = name;

      T* child = dynamic_cast<T*>(layer);
      return *child;
    }

  private:
    bool m_initialized;

    Manifest m_manifest;

    std::shared_ptr<Profiler> m_profiler;
    std::shared_ptr<Input> m_input;

    GLFWwindow* m_glfwWindow;

    std::vector<std::shared_ptr<AppLayer>> m_activeLayers;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
  };
} // namespace Scaffold
