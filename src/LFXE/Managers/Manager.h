#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {

    class LightFXExtender;

    namespace managers {

        template<typename T>
        class Manager : public std::enable_shared_from_this < T > {

        public:
            Manager() {}
            virtual ~Manager() {}

            std::shared_ptr<LightFXExtender> GetLightFXExtender() {
                return this->lightFXExtender.lock();
            }

            void SetLightFXExtender(std::shared_ptr<LightFXExtender> lightFXExtender) {
                this->lightFXExtender = lightFXExtender;
            }

        private:
            std::weak_ptr<LightFXExtender> lightFXExtender;

        };

    }
}
