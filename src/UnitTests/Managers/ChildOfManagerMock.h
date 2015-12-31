#pragma once

// Standard includes
#include <string>

// Project includes
#include "ManagerWithChildrenMock.h"
#include "../../LFXE/Managers/ChildOfManager.h"


namespace lightfx_tests {
    namespace managers {        

        class ManagerWithChildrenMock;

        class ChildOfManagerMock : public lightfx::managers::ChildOfManager<ManagerWithChildrenMock> {

        };

    }
}
