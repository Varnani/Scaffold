#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Application.hpp>
#include <Profiler.hpp>
#include <Input.hpp>

#include <ProfilerLayer.hpp>
#include <InputInfoLayer.hpp>
#include <DemoLayer.hpp>

#include <iostream>
#include <type_traits>

static Scaffold::Application* s_instance = nullptr;

static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Scaffold::Application::Application(const Manifest manifest)
{
    m_initialized = false;
    m_manifest = manifest;

    // -- GLFW
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW." << std::endl;
        return;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_glfwWindow = glfwCreateWindow(manifest.initialWidth, manifest.initialHeight, manifest.title.c_str(), NULL, NULL);
    if (m_glfwWindow == nullptr)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        return;
    }

    glfwMakeContextCurrent(m_glfwWindow);
    glfwSwapInterval(m_manifest.swapInterval);

    // -- GLAD
    if (gladLoadGL(glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to init GLAD." << std::endl;
        return;
    }

    // -- Modules
    m_profiler = std::make_shared<Profiler>();
    m_input = std::make_shared<Input>(m_glfwWindow);

    // -- IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // -- Create Built-in Layers
    if (manifest.useProfilerLayer)
    {
        CreateLayer<ProfilerLayer>("Profiler UI");
    }

    if (manifest.useInputInfoLayer)
    {
        CreateLayer<InputInfoLayer>("InputInfo UI");
    }

    if (manifest.useDemoLayer)
    {
        CreateLayer<DemoLayer>("ImGui Demo");
    }

    // -- Ready
    s_instance = this;
    m_initialized = true;
};

void Scaffold::Application::Run()
{
    if (!m_initialized)
    {
        std::cout << "Application is not initialized." << std::endl;
        return;
    }

    Profiler& profiler = GetProfiler();
    Input& input = GetInput();

    int width, height;

    while (!glfwWindowShouldClose(m_glfwWindow))
    {
        float deltaTime = profiler.GetRootMarker().durationAsSeconds;

        profiler.BeginFrame();

        profiler.BeginMarker("Poll Events");
        {
            input.UpdateKeyStates();

            profiler.BeginMarker("GLFW Events");
            {
                if (m_manifest.eventWaitTimeout < 0.0f) glfwPollEvents();
                else glfwWaitEventsTimeout(m_manifest.eventWaitTimeout);
            }
            profiler.EndMarker();
        }
        profiler.EndMarker();

        profiler.BeginMarker("Adjust Viewport");
        {
            glfwGetFramebufferSize(m_glfwWindow, &width, &height);
            glViewport(0, 0, width, height);
        }
        profiler.EndMarker();

        profiler.BeginMarker("AppLayer::OnUpdate");
        {
            for (size_t i = 0; i < m_activeLayers.size(); i++)
            {
                AppLayer* layer = m_activeLayers[i].get();

                profiler.BeginMarker(layer->name);
                layer->OnUpdate(deltaTime);
                profiler.EndMarker();
            }
        }
        profiler.EndMarker();

        profiler.BeginMarker("Prepare ImGui Frame");
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (m_manifest.dockspaceOverViewport)
            {
                ImGui::DockSpaceOverViewport();
            }
        }
        profiler.EndMarker();

        profiler.BeginMarker("AppLayer::OnRenderUI");
        {
            for (size_t i = 0; i < m_activeLayers.size(); i++)
            {
                AppLayer* layer = m_activeLayers[i].get();

                profiler.BeginMarker(layer->name);
                layer->OnRenderUI(deltaTime);
                profiler.EndMarker();
            }
        }
        profiler.EndMarker();

        profiler.BeginMarker("Clear Buffer");
        {
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        profiler.EndMarker();

        profiler.BeginMarker("ImGui Render");
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        profiler.EndMarker();

        profiler.BeginMarker("Swap & Present");
        {
            glfwSwapBuffers(m_glfwWindow);
        }
        profiler.EndMarker();

        profiler.EndFrame();
    }
}

Scaffold::Input& Scaffold::Application::GetInput()
{
    return *s_instance->m_input;
}

Scaffold::Profiler& Scaffold::Application::GetProfiler()
{
    return *s_instance->m_profiler;
}

Scaffold::Application& Scaffold::Application::GetInstance()
{
    return *s_instance;
}

GLFWwindow* Scaffold::Application::GetWindowHandle()
{
    return m_glfwWindow;
}

Scaffold::Application::~Application()
{
    if (!m_initialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}
