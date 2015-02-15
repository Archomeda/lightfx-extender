#include "../../src/Managers/TrayManager.h"
#include "../Devices/DeviceMock.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::managers;
using namespace lightfx_tests::devices;

namespace lightfx_tests {
    namespace managers {

        TEST_CLASS(TrayManagerTest) {
public:

    TEST_METHOD(AttachDevice) {
        auto manager = make_shared<TrayManager>();
        auto device = make_shared<DeviceMock>();

        manager->AttachDevice(device);
        Assert::IsTrue(device == manager->GetDevice(device->GetDeviceName()));
    }

    TEST_METHOD(RemoveDevice) {
        auto manager = make_shared<TrayManager>();
        auto device = make_shared<DeviceMock>();

        manager->AttachDevice(device);
        Assert::IsTrue(device == manager->GetDevice(device->GetDeviceName()), L"Device not added");
        manager->RemoveDevice(device->GetDeviceName());
        Assert::IsTrue(nullptr == manager->GetDevice(device->GetDeviceName()), L"Device not removed");
    }

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
