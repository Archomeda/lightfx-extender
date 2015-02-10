#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// Project includes
#include "../LightFXExtender.h"

// API exports
#include "../Common/ApiExports.h"


#pragma warning(push)
#pragma warning(disable : 4251)

namespace lightfx {

    class LightFXExtender;

    namespace managers {

        template<typename CT, typename T>
        class LFXE_API Manager : public std::enable_shared_from_this < T > {

        public:
            Manager() {};

            std::shared_ptr<LightFXExtender> GetLightFXExtender() {
                return this->lightFXExtender;
            }

            void SetLightFXExtender(std::shared_ptr<LightFXExtender> lightFXExtender) {
                this->lightFXExtender = lightFXExtender;
            }


            void AddChild(const std::wstring& childKey, std::shared_ptr<CT> child) {
                this->children[childKey] = child;
                child->SetConfigManager(shared_from_this());
            }

            std::shared_ptr<CT> GetChild(const std::wstring& childKey) {
                return this->children[childKey].lock();
            }

        private:
            std::shared_ptr<LightFXExtender> lightFXExtender;
            std::map<std::wstring, std::weak_ptr<CT>> children = {};


        };

    }
}

#pragma warning(pop)
