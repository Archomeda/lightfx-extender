#pragma once

// Standard includes
#include <string>

// Project includes
#include "ChildOfManagerMock.h"
#include "../../src/Managers/ManagerWithChildren.h"


namespace lightfx_tests {
    namespace managers {

        class ChildOfManagerMock;

        class ManagerWithChildrenMock : public lightfx::managers::ManagerWithChildren<ChildOfManagerMock, ManagerWithChildrenMock> {

        };

    }
}
