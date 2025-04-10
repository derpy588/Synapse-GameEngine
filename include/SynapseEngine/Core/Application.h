#pragma once

#include "SynapseEngine/Synapse.h"

namespace Synapse {

    class Application {
        
        public:
            Application();
            ~Application();

            void Run();

            Engine& getEngine();

        private:
            Engine engine;

    };

}