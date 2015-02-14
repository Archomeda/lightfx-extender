#pragma once

// Standard includes
#include <memory>

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {

    class LightFXExtender;

    namespace managers {

        template<typename T>
        class LFXE_API ChildOfManager {

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

#pragma warning(pop)
