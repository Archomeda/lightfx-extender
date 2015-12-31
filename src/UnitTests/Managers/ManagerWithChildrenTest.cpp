#include "CppUnitTest.h"
#include "ManagerWithChildrenMock.h"
#include "ChildOfManagerMock.h"
#include "../../LFXE/Managers/ManagerWithChildren.h"
#include "../../LFXE/LightFXExtender.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::managers;

namespace lightfx_tests {
    namespace managers {

        TEST_CLASS(ManagerWithChildrenTest) {
public:

    TEST_METHOD(AddChild) {
        wstring childKey = L"SomeChildKey";
        auto child = make_shared<ChildOfManagerMock>();
        auto manager = make_shared<ManagerWithChildrenMock>();
        manager->AddChild(childKey, child);

        Assert::IsTrue(child == manager->GetChild(childKey));
        Assert::IsTrue(manager == child->GetManager());
    }

        };
    }
}
