#include "CppUnitTest.h"

// Project includes
#include "DeviceMock.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::devices;
using namespace lightfx::timelines;

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
        LightTimeline timeline = LightTimeline::NewInstant(LightColor(1, 2, 3, 4));
        device->QueueTimeline(Timeline(1, timeline));
        Assert::IsTrue(timeline.GetColorAtTime(0) == device->GetQueuedTimeline().GetColorAtTime(0, 0));
    }

    TEST_METHOD(QueueTimelineAndUpdate) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightTimeline timeline = LightTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        device->QueueTimeline(Timeline(1, timeline));
        device->CommitQueuedTimeline();
        bool ret = device->Update(chrono::milliseconds(0));

        Assert::IsFalse(ret, L"Return value should be false");
        Assert::IsTrue(timeline.GetColorAtTime(0) == device->GetActiveTimeline().GetColorAtTime(0, 0), L"The active timeline should match with timeline");
        device->Disable();
    }

    TEST_METHOD(QueueTimelineAndUpdateFlush) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightTimeline timeline1 = LightTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightTimeline timeline2 = LightTimeline::NewInstant(LightColor(3, 4, 5, 6));
        device->QueueTimeline(Timeline(1, timeline1));
        device->CommitQueuedTimeline();
        bool ret = device->Update(chrono::milliseconds(0));

        Assert::IsFalse(ret, L"Return value should be false");
        Assert::IsTrue(timeline1.GetColorAtTime(0) == device->GetActiveTimeline().GetColorAtTime(0, 0), L"The active timeline should match with timeline1");

        device->QueueTimeline(Timeline(1, timeline2));
        device->CommitQueuedTimeline();
        ret = device->Update(chrono::milliseconds(0));

        Assert::IsTrue(ret, L"Return value should now be true");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == device->GetActiveTimeline().GetColorAtTime(0, 0), L"The active timeline should now match with timeline2");
        device->Disable();
    }

    TEST_METHOD(QueueTimelineAndUpdateNoFlush) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightTimeline timeline1 = LightTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightTimeline timeline2 = LightTimeline::NewInstant(LightColor(3, 4, 5, 6));
        device->QueueTimeline(Timeline(1, timeline1));
        device->CommitQueuedTimeline();
        bool ret = device->Update(chrono::milliseconds(0));

        Assert::IsFalse(ret, L"Return value should be false");
        Assert::IsTrue(timeline1.GetColorAtTime(0) == device->GetActiveTimeline().GetColorAtTime(0, 0), L"The active timeline should match with timeline1");
        
        device->QueueTimeline(Timeline(1, timeline2));
        device->CommitQueuedTimeline(false);
        ret = device->Update(chrono::milliseconds(0));

        Assert::IsFalse(ret, L"Return value should still be false");
        Assert::IsTrue(timeline1.GetColorAtTime(0) == device->GetActiveTimeline().GetColorAtTime(0, 0), L"The active timeline should still match with timeline1");
        device->Disable();
    }

    TEST_METHOD(GetRecentTimelineActive) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightTimeline timeline = LightTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        device->QueueTimeline(Timeline(1, timeline));
        device->CommitQueuedTimeline();

        Assert::IsTrue(timeline.GetColorAtTime(0) == device->GetRecentTimeline().GetColorAtTime(0, 0));
        device->Disable();
    }

    TEST_METHOD(GetRecentTimelineQueued) {
        auto device = make_shared<DeviceMock>();
        device->Reset();
        device->Enable();
        LightTimeline timeline1 = LightTimeline::NewMorph(LightColor(1, 2, 3, 4), LightColor(2, 3, 4, 5), 60000);
        LightTimeline timeline2 = LightTimeline::NewInstant(LightColor(1, 2, 3, 4));
        device->QueueTimeline(Timeline(1, timeline1));
        device->CommitQueuedTimeline();

        device->QueueTimeline(Timeline(1, timeline2));
        device->CommitQueuedTimeline(false);

        Assert::IsTrue(timeline2.GetColorAtTime(0) == device->GetRecentTimeline().GetColorAtTime(0, 0));
        device->Disable();
    }

        };

    }
}
