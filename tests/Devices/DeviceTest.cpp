#include "CppUnitTest.h"

// Project includes
#include "DeviceMock.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::devices;

namespace lightfx_tests {
    namespace devices {

        TEST_CLASS(DeviceTest) {
public:

    TEST_METHOD(Enable) {
        auto device = make_shared<DeviceMock>();
        Assert::IsFalse(device->IsEnabled());
        device->Enable();
        Assert::IsTrue(device->IsEnabled());
    }

    TEST_METHOD(Disable) {
        auto device = make_shared<DeviceMock>();
        device->Enable();
        Assert::IsTrue(device->IsEnabled());
        device->Disable();
        Assert::IsFalse(device->IsEnabled());
    }

    TEST_METHOD(Initialize) {
        auto device = make_shared<DeviceMock>();
        Assert::IsFalse(device->IsInitialized());
        device->Initialize();
        Assert::IsTrue(device->IsInitialized());
    }

    TEST_METHOD(Release) {
        auto device = make_shared<DeviceMock>();
        device->Initialize();
        Assert::IsTrue(device->IsInitialized());
        device->Release();
        Assert::IsFalse(device->IsInitialized());
    }

    TEST_METHOD(QueueLightAction) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        LightAction lightAction(1, LightColor(1, 2, 3, 4));
        device->QueueLightAction(lightAction);
        Assert::IsTrue(lightAction.GetStartColor(0) == device->GetQueuedLightAction().GetStartColor(0));
    }

    TEST_METHOD(QueueLightActionAndUpdate) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        LightAction lightAction(1, LightColor(1, 2, 3, 4));
        device->QueueLightAction(lightAction);
        device->Update();
        Assert::IsTrue(lightAction.GetStartColor(0) == device->GetCurrentLightAction().GetStartColor(0));
    }

        };

    }
}
