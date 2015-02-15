#pragma once

// Standard includes
#include <memory>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {

    class LightFXExtender;

    namespace managers {

        template<typename T>
        class ChildOfManager {

        public:
            ChildOfManager() {}
            virtual ~ChildOfManager() {}

            std::shared_ptr<T> GetManager() {
                return this->manager;
            }

            void SetManager(std::shared_ptr<T> manager) {
                this->manager = manager;
            }

        private:
            std::shared_ptr<T> manager;

        };

    }
}
