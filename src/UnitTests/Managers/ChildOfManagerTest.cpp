#include "CppUnitTest.h"
#include "ChildOfManagerMock.h"
#include "ManagerWithChildrenMock.h"
#include "../../LFXE/Managers/ChildOfManager.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::managers;

namespace lightfx_tests {
    namespace managers {

        TEST_CLASS(ChildOfManagerTest) {
public:

    TEST_METHOD(SetManager) {
        auto child = make_shared<ChildOfManagerMock>();
        auto manager = make_shared<ManagerWithChildrenMock>();
        child->SetManager(manager->shared_from_this());

        Assert::IsTrue(manager == child->GetManager());
    }

        };
    }
}
