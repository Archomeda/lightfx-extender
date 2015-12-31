#pragma once

// Standard includes
#include <string>

// Project includes
#include "../../LFXE/Managers/Manager.h"


namespace lightfx_tests {
    namespace managers {

        class ManagerMock : public lightfx::managers::Manager<ManagerMock> {

        public:
            ManagerMock() {}
            using Manager::~Manager;

            using Manager::shared_from_this;
            using Manager::GetLightFXExtender;
            using Manager::SetLightFXExtender;

        };

    }
}
