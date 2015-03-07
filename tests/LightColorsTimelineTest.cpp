#include "CppUnitTest.h"

// Standard includes
#include <algorithm>

// Project includes
#include "../src/LightColorsTimeline.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;

namespace lightfx_tests {

    TEST_CLASS(LightColorsTimelineTest) {
public:

    TEST_METHOD(ConstructorByAll) {
        LightColorTimelineAction timelineAction(LightColor(1, 2, 3, 4), 0, 0);
        LightColorTimeline timeline;
        timeline.AddAction(timelineAction);

        LightColorsTimeline colorsTimeline(2, timeline);

        Assert::IsTrue(timeline.GetColorAtTime(0) == colorsTimeline.GetTimeline(0).GetColorAtTime(0), L"Timeline 1");
        Assert::IsTrue(timeline.GetColorAtTime(0) == colorsTimeline.GetTimeline(1).GetColorAtTime(0), L"Timeline 2");
    }

    TEST_METHOD(ConstructorByVector) {
        LightColorTimelineAction timelineAction1(LightColor(1, 2, 3, 4), 0, 0);
        LightColorTimeline timeline1;
        timeline1.AddAction(timelineAction1);
        LightColorTimelineAction timelineAction2(LightColor(2, 3, 4, 5), 0, 0);
        LightColorTimeline timeline2;
        timeline2.AddAction(timelineAction2);

        LightColorsTimeline colorsTimeline({ timeline1, timeline2 });

        Assert::IsTrue(timeline1.GetColorAtTime(0) == colorsTimeline.GetTimeline(0).GetColorAtTime(0), L"Timeline 1");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == colorsTimeline.GetTimeline(1).GetColorAtTime(0), L"Timeline 2");
    }

    TEST_METHOD(SetTimelineByIndex) {
        LightColorTimelineAction timelineAction(LightColor(1, 2, 3, 4), 0, 0);
        LightColorTimeline timeline;
        timeline.AddAction(timelineAction);
        LightColorsTimeline colorsTimeline;
        
        colorsTimeline.SetTimeline(1, timeline);

        Assert::IsTrue(timeline.GetColorAtTime(0) == colorsTimeline.GetTimeline(1).GetColorAtTime(0));
    }

    TEST_METHOD(SetTimelineByVector) {
        LightColorTimelineAction timelineAction1(LightColor(1, 2, 3, 4), 0, 0);
        LightColorTimeline timeline1;
        timeline1.AddAction(timelineAction1);
        LightColorTimelineAction timelineAction2(LightColor(2, 3, 4, 5), 0, 0);
        LightColorTimeline timeline2;
        timeline2.AddAction(timelineAction2);
        LightColorsTimeline colorsTimeline;

        colorsTimeline.SetTimeline({ timeline1, timeline2 });

        Assert::IsTrue(timeline1.GetColorAtTime(0) == colorsTimeline.GetTimeline(0).GetColorAtTime(0), L"Timeline 1");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == colorsTimeline.GetTimeline(1).GetColorAtTime(0), L"Timeline 2");
    }

    TEST_METHOD(GetTotalDurationAll) {
        LightColorTimelineAction timelineAction1(LightColor(1, 2, 3, 4), 100, 0);
        LightColorTimeline timeline1;
        timeline1.AddAction(timelineAction1);
        LightColorTimelineAction timelineAction2(LightColor(2, 3, 4, 5), 150, 50);
        LightColorTimeline timeline2;
        timeline2.AddAction(timelineAction2);

        LightColorsTimeline colorsTimeline({ timeline1, timeline2 });

        Assert::AreEqual<unsigned long>(max(timeline1.GetTotalDuration(), timeline2.GetTotalDuration()), colorsTimeline.GetTotalDuration());
    }

    TEST_METHOD(GetTotalDurationByIndex) {
        LightColorTimelineAction timelineAction1(LightColor(1, 2, 3, 4), 100, 0);
        LightColorTimeline timeline1;
        timeline1.AddAction(timelineAction1);
        LightColorTimelineAction timelineAction2(LightColor(2, 3, 4, 5), 150, 50);
        LightColorTimeline timeline2;
        timeline2.AddAction(timelineAction2);

        LightColorsTimeline colorsTimeline({ timeline1, timeline2 });

        Assert::AreEqual<unsigned long>(timeline1.GetTotalDuration(), colorsTimeline.GetTotalDuration(0), L"Timeline 1");
        Assert::AreEqual<unsigned long>(timeline2.GetTotalDuration(), colorsTimeline.GetTotalDuration(1), L"Timeline 2");
    }

    TEST_METHOD(GetColorAtTimeAll) {
        LightColorTimelineAction timelineAction1a(LightColor(1, 2, 3, 4), 0, 0);
        LightColorTimelineAction timelineAction1b(LightColor(10, 20, 30, 40), 0, 1000);
        LightColorTimeline timeline1;
        timeline1.AddAction(timelineAction1a);
        timeline1.AddAction(timelineAction1b);
        LightColorTimelineAction timelineAction2a(LightColor(2, 3, 4, 5), 0, 0);
        LightColorTimelineAction timelineAction2b(LightColor(20, 30, 40, 50), 0, 1000);
        LightColorTimeline timeline2;
        timeline2.AddAction(timelineAction2a);
        timeline2.AddAction(timelineAction2b);
        LightColorsTimeline colorsTimeline({ timeline1, timeline2 });

        Assert::IsTrue(timeline1.GetColorAtTime(0) == colorsTimeline.GetColorAtTime(0)[0], L"Timeline 1, time 0");
        Assert::IsTrue(timeline1.GetColorAtTime(1000) == colorsTimeline.GetColorAtTime(1000)[0], L"Timeline 1, time 1000");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == colorsTimeline.GetColorAtTime(0)[1], L"Timeline 2, time 0");
        Assert::IsTrue(timeline2.GetColorAtTime(1000) == colorsTimeline.GetColorAtTime(1000)[1], L"Timeline 2, time 1000");
    }
    
    TEST_METHOD(GetColorAtTimeByIndex) {
        LightColorTimelineAction timelineAction1(LightColor(1, 2, 3, 4), 0, 0);
        LightColorTimelineAction timelineAction2(LightColor(10, 20, 30, 40), 0, 1000);
        LightColorTimeline timeline;
        timeline.AddAction(timelineAction1);
        timeline.AddAction(timelineAction2);
        LightColorsTimeline colorsTimeline(1, timeline);

        Assert::IsTrue(timeline.GetColorAtTime(0) == colorsTimeline.GetColorAtTime(0, 0), L"Time 0");
        Assert::IsTrue(timeline.GetColorAtTime(1000) == colorsTimeline.GetColorAtTime(0, 1000), L"Time 1000");
    }

    };

}
