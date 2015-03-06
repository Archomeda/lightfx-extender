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
        device->Disable();
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
        LightAction lightAction = LightAction::NewInstant(1, LightColor(1, 2, 3, 4));
        device->QueueLightAction(lightAction);
        Assert::IsTrue(lightAction.GetStartColor(0) == device->GetQueuedLightAction().GetStartColor(0));
    }

    TEST_METHOD(QueueLightActionAndUpdate) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightAction lightAction = LightAction::NewInstant(1, LightColor(1, 2, 3, 4));
        device->QueueLightAction(lightAction);
        device->Update();

        bool success = false;
        for (int i = 0; i < 500; ++i) {
            if (device->GetActiveLightAction().GetStartColor().size() > 0 && lightAction.GetStartColor(0) == device->GetActiveLightAction().GetStartColor(0)) {
                success = true;
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
        if (!success) {
            Assert::Fail(L"Active light actions do not match after an extended period of time");
        }
        device->Disable();
    }

    TEST_METHOD(QueueLightActionAndUpdateNoFlush) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightAction lightAction1 = LightAction::NewMorph(1, LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightAction lightAction2 = LightAction::NewInstant(1, LightColor(3, 4, 5, 6));
        device->QueueLightAction(lightAction1);
        device->Update();

        device->QueueLightAction(lightAction2);
        device->Update(false);

        bool success = false;
        for (int i = 0; i < 500; ++i) {
            if (device->GetActiveLightAction().GetStartColor().size() > 0 && lightAction2.GetStartColor(0) != device->GetActiveLightAction().GetStartColor(0)) {
                success = true;
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(1));
        }
        if (!success) {
            Assert::Fail(L"Active light actions do not match after an extended period of time");
        }
        device->Disable();
    }

    TEST_METHOD(LastLightActionActive) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightAction lightAction = LightAction::NewMorph(1, LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        device->QueueLightAction(lightAction);
        device->Update();

        Assert::IsTrue(lightAction.GetStartColor(0) == device->GetLastLightAction().GetStartColor(0));
        device->Disable();
    }

    TEST_METHOD(LastLightActionQueued) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightAction lightAction1 = LightAction::NewMorph(1, LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightAction lightAction2 = LightAction::NewInstant(1, LightColor(3, 4, 5, 6));
        device->QueueLightAction(lightAction1);
        device->Update();

        device->QueueLightAction(lightAction2);
        device->Update(false);

        Assert::IsTrue(lightAction2.GetStartColor(0) == device->GetLastLightAction().GetStartColor(0));
        device->Disable();
    }

        };

    }
}
