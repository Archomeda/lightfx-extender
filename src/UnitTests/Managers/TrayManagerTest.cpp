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

    TEST_METHOD(HasNoUpdateAvailableNotification) {
        auto manager = make_shared<TrayManager>();
        Assert::IsFalse(manager->HasUpdateAvailableNotification());
    }

    TEST_METHOD(SetUpdateAvailableNotification) {
        auto manager = make_shared<TrayManager>();
        manager->SetUpdateAvailableNotification(L"0.0", L"http://some.nice/download?url=here");
        Assert::IsTrue(manager->HasUpdateAvailableNotification());
    }

        };
    }
}
