#include "CppUnitTest.h"

// Standard includes
#include <algorithm>

// Project includes
#include "../src/timelines/Timeline.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::timelines;

namespace lightfx_tests {
    namespace timelines {

        TEST_CLASS(TimelineTest) {
public:

    TEST_METHOD(ConstructorByAll) {
        LightTimelineItem lightTimelineItem(LightColor(1, 2, 3, 4), 0, 0);
        LightTimeline lightTimeline;
        lightTimeline.AddItem(lightTimelineItem);

        Timeline timeline(2, lightTimeline);

        Assert::IsTrue(lightTimeline.GetColorAtTime(0) == timeline.GetTimeline(0).GetColorAtTime(0), L"Timeline 1");
        Assert::IsTrue(lightTimeline.GetColorAtTime(0) == timeline.GetTimeline(1).GetColorAtTime(0), L"Timeline 2");
    }

    TEST_METHOD(ConstructorByVector) {
        LightTimelineItem timelineItem1(LightColor(1, 2, 3, 4), 0, 0);
        LightTimeline timeline1;
        timeline1.AddItem(timelineItem1);
        LightTimelineItem timelineItem2(LightColor(2, 3, 4, 5), 0, 0);
        LightTimeline timeline2;
        timeline2.AddItem(timelineItem2);

        Timeline timeline({ timeline1, timeline2 });

        Assert::IsTrue(timeline1.GetColorAtTime(0) == timeline.GetTimeline(0).GetColorAtTime(0), L"Timeline 1");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == timeline.GetTimeline(1).GetColorAtTime(0), L"Timeline 2");
    }

    TEST_METHOD(SetTimelineByIndex) {
        LightTimelineItem lightTimelineItem(LightColor(1, 2, 3, 4), 0, 0);
        LightTimeline lightTimeline;
        lightTimeline.AddItem(lightTimelineItem);
        Timeline timeline;

        timeline.SetTimeline(1, lightTimeline);

        Assert::IsTrue(lightTimeline.GetColorAtTime(0) == timeline.GetTimeline(1).GetColorAtTime(0));
    }

    TEST_METHOD(SetTimelineByVector) {
        LightTimelineItem timelineItem1(LightColor(1, 2, 3, 4), 0, 0);
        LightTimeline timeline1;
        timeline1.AddItem(timelineItem1);
        LightTimelineItem timelineItem2(LightColor(2, 3, 4, 5), 0, 0);
        LightTimeline timeline2;
        timeline2.AddItem(timelineItem2);
        Timeline timeline;

        timeline.SetTimeline({ timeline1, timeline2 });

        Assert::IsTrue(timeline1.GetColorAtTime(0) == timeline.GetTimeline(0).GetColorAtTime(0), L"Timeline 1");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == timeline.GetTimeline(1).GetColorAtTime(0), L"Timeline 2");
    }

    TEST_METHOD(GetTotalDurationAll) {
        LightTimelineItem timelineItem1(LightColor(1, 2, 3, 4), 100, 0);
        LightTimeline timeline1;
        timeline1.AddItem(timelineItem1);
        LightTimelineItem timelineItem2(LightColor(2, 3, 4, 5), 150, 50);
        LightTimeline timeline2;
        timeline2.AddItem(timelineItem2);

        Timeline timeline({ timeline1, timeline2 });

        Assert::AreEqual<unsigned long>(max(timeline1.GetTotalDuration(), timeline2.GetTotalDuration()), timeline.GetTotalDuration());
    }

    TEST_METHOD(GetTotalDurationByIndex) {
        LightTimelineItem timelineItem1(LightColor(1, 2, 3, 4), 100, 0);
        LightTimeline timeline1;
        timeline1.AddItem(timelineItem1);
        LightTimelineItem timelineItem2(LightColor(2, 3, 4, 5), 150, 50);
        LightTimeline timeline2;
        timeline2.AddItem(timelineItem2);

        Timeline timeline({ timeline1, timeline2 });

        Assert::AreEqual<unsigned long>(timeline1.GetTotalDuration(), timeline.GetTotalDuration(0), L"Timeline 1");
        Assert::AreEqual<unsigned long>(timeline2.GetTotalDuration(), timeline.GetTotalDuration(1), L"Timeline 2");
    }

    TEST_METHOD(GetColorAtTimeAll) {
        LightTimelineItem timelineItem1a(LightColor(1, 2, 3, 4), 0, 0);
        LightTimelineItem timelineItem1b(LightColor(10, 20, 30, 40), 0, 1000);
        LightTimeline timeline1;
        timeline1.AddItem(timelineItem1a);
        timeline1.AddItem(timelineItem1b);
        LightTimelineItem timelineItem2a(LightColor(2, 3, 4, 5), 0, 0);
        LightTimelineItem timelineItem2b(LightColor(20, 30, 40, 50), 0, 1000);
        LightTimeline timeline2;
        timeline2.AddItem(timelineItem2a);
        timeline2.AddItem(timelineItem2b);
        Timeline timeline({ timeline1, timeline2 });

        Assert::IsTrue(timeline1.GetColorAtTime(0) == timeline.GetColorAtTime(0)[0], L"Timeline 1, time 0");
        Assert::IsTrue(timeline1.GetColorAtTime(1000) == timeline.GetColorAtTime(1000)[0], L"Timeline 1, time 1000");
        Assert::IsTrue(timeline2.GetColorAtTime(0) == timeline.GetColorAtTime(0)[1], L"Timeline 2, time 0");
        Assert::IsTrue(timeline2.GetColorAtTime(1000) == timeline.GetColorAtTime(1000)[1], L"Timeline 2, time 1000");
    }

    TEST_METHOD(GetColorAtTimeByIndex) {
        LightTimelineItem timelineItem1(LightColor(1, 2, 3, 4), 0, 0);
        LightTimelineItem timelineItem2(LightColor(10, 20, 30, 40), 0, 1000);
        LightTimeline lightTimeline;
        lightTimeline.AddItem(timelineItem1);
        lightTimeline.AddItem(timelineItem2);
        Timeline timeline(1, lightTimeline);

        Assert::IsTrue(lightTimeline.GetColorAtTime(0) == timeline.GetColorAtTime(0, 0), L"Time 0");
        Assert::IsTrue(lightTimeline.GetColorAtTime(1000) == timeline.GetColorAtTime(0, 1000), L"Time 1000");
    }

        };

    }
}
