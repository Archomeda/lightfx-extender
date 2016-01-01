#include "../Devices/DeviceMock.h"
#include "CppUnitTest.h"

// Project includes
#include "../../LFXE/Managers/TrayManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::managers;
using namespace lightfx_tests::devices;

namespace lightfx_tests {
    namespace managers {

        TEST_CLASS(TrayManagerTest) {
public:

    TEST_METHOD(HasNoUpdateNotification) {
        auto manager = make_shared<TrayManager>();
        Assert::IsFalse(manager->HasUpdateNotification());
    }

    TEST_METHOD(SetUpdateNotification) {
        auto manager = make_shared<TrayManager>();
        manager->SetUpdateNotification(L"0.0", L"http://some.nice/download?url=here");
        Assert::IsTrue(manager->HasUpdateNotification());
    }

        };
    }
}
