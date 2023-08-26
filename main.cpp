#include <Application.hpp>
#include <DemoLayer.hpp>

int main(int, char**)
{
    Scaffold::Manifest manifest = {};

    manifest.title = "Scaffold";
    manifest.initialWidth = 1280;
    manifest.initialHeight = 720;
    manifest.dockspaceOverViewport = true;
    manifest.useProfilerLayer = true;

    Scaffold::Application app(manifest);
    app.Run();

    return 0;
}