#pragma once

// Standard includes
#include <string>
#include <map>
#include <memory>

// Project includes
#include "Manager.h"

// API exports
#include "../Common/ApiExports.h"


namespace lightfx {

    class LightFXExtender;

    namespace managers {

        template<typename CT, typename T>
        class ManagerWithChildren : public Manager < T > {

        public:
            ManagerWithChildren() {}
            virtual ~ManagerWithChildren() {}

            using Manager::GetLightFXExtender;
            using Manager::SetLightFXExtender;


            void AddChild(const std::wstring& childKey, std::shared_ptr<CT> child) {
                this->children[childKey] = child;
                child->SetManager(shared_from_this());
            }

            std::shared_ptr<CT> GetChild(const std::wstring& childKey) {
                return this->children[childKey];
            }

        private:
            std::map<std::wstring, std::shared_ptr<CT>> children = {};

        };

    }
}
