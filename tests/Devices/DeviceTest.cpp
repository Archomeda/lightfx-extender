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

    TEST_METHOD(QueueTimeline) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        LightColorTimeline timeline = LightColorTimeline::NewInstant(LightColor(1, 2, 3, 4));
        device->QueueTimeline(LightColorsTimeline(1, timeline));
        Assert::IsTrue(timeline.GetColorAtTime(0) == device->GetQueuedTimeline().GetColorAtTime(0, 0));
    }

    TEST_METHOD(QueueTimelineAndUpdate) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightColorTimeline timeline = LightColorTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        device->QueueTimeline(LightColorsTimeline(1, timeline));
        device->Update();

        bool success = false;
        for (int i = 0; i < 500; ++i) {
            if (device->GetActiveTimeline().GetColorAtTime(0).size() > 0 && timeline.GetColorAtTime(0) == device->GetActiveTimeline().GetColorAtTime(0, 0)) {
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

    TEST_METHOD(QueueTimelineAndUpdateNoFlush) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightColorTimeline timeline1 = LightColorTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightColorTimeline timeline2 = LightColorTimeline::NewInstant(LightColor(3, 4, 5, 6));
        device->QueueTimeline(LightColorsTimeline(1, timeline1));
        device->Update();

        device->QueueTimeline(LightColorsTimeline(1, timeline2));
        device->Update(false);

        bool success = false;
        for (int i = 0; i < 500; ++i) {
            if (device->GetActiveTimeline().GetColorAtTime(0).size() > 0 && timeline2.GetColorAtTime(0) != device->GetActiveTimeline().GetColorAtTime(0, 0)) {
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

    TEST_METHOD(GetRecentTimelineActive) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightColorTimeline timeline = LightColorTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        device->QueueTimeline(LightColorsTimeline(1, timeline));
        device->Update();

        Assert::IsTrue(timeline.GetColorAtTime(0) == device->GetRecentTimeline().GetColorAtTime(0, 0));
        device->Disable();
    }

    TEST_METHOD(GetRecentTimelineQueued) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightColorTimeline timeline1 = LightColorTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightColorTimeline timeline2 = LightColorTimeline::NewInstant(LightColor(1, 2, 3, 4));
        device->QueueTimeline(LightColorsTimeline(1, timeline1));
        device->Update();

        device->QueueTimeline(LightColorsTimeline(1, timeline2));
        device->Update(false);

        Assert::IsTrue(timeline2.GetColorAtTime(0) == device->GetRecentTimeline().GetColorAtTime(0, 0));
        device->Disable();
    }

        };

    }
}
