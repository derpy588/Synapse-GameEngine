#include "SynapseEngine/Core/Application.h"

namespace Synapse {
    Application::Application() {
        engine.Initilize();
    }

    Application::~Application() {
        engine.Shutdown();
    }

    void Application::Run() {

    }

    Engine& Application::getEngine() {
        return engine;
    }
}

