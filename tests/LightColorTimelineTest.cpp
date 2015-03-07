#include "CppUnitTest.h"

// Project includes
#include "../src/LightColorTimeline.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;

namespace lightfx_tests {

    TEST_CLASS(LightColorTimelineTest) {
public:

    TEST_METHOD(AddAction) {
        LightColor color(1, 2, 3, 4);
        LightColorTimelineAction timelineAction(color, 0, 0);
        LightColorTimeline timeline;

        timeline.AddAction(timelineAction);

        Assert::IsTrue(color == timeline.GetColorAtTime(0));
    }

    TEST_METHOD(GetTotalDurationZero) {
        LightColorTimeline timeline;

        timeline.AddAction(LightColorTimelineAction(LightColor(), 0, 0));

        Assert::AreEqual<unsigned long>(0, timeline.GetTotalDuration());
    }

    TEST_METHOD(GetTotalDuration) {
        LightColorTimeline timeline;

        timeline.AddAction(LightColorTimelineAction(LightColor(), 10, 20));
        timeline.AddAction(LightColorTimelineAction(LightColor(), 30, 40));

        Assert::AreEqual<unsigned long>(100, timeline.GetTotalDuration());
    }

    TEST_METHOD(GetColorAtTimeDelayedInstant) {
        LightColor color1(100, 100, 100, 100);
        LightColor color2(200, 200, 200, 200);
        LightColorTimelineAction timelineAction1(color1, 0, 0);
        LightColorTimelineAction timelineAction2(color2, 0, 100);
        LightColorTimeline timeline;

        timeline.AddAction(timelineAction1);
        timeline.AddAction(timelineAction2);

        Assert::IsTrue(color1 == timeline.GetColorAtTime(0), L"Time 0");
        Assert::IsTrue(color1 == timeline.GetColorAtTime(99), L"Time 99");
        Assert::IsTrue(color2 == timeline.GetColorAtTime(100), L"Time 100");
        Assert::IsTrue(color2 == timeline.GetColorAtTime(101), L"Time 101");
    }

    TEST_METHOD(GetColorAtTimeDelayedTransition) {
        LightColor color1(100, 100, 100, 100);
        LightColor color2(200, 200, 200, 200);
        LightColorTimelineAction timelineAction1(color1, 0, 0);
        LightColorTimelineAction timelineAction2(color2, 100, 100);
        LightColorTimeline timeline;

        timeline.AddAction(timelineAction1);
        timeline.AddAction(timelineAction2);

        Assert::IsTrue(color1 == timeline.GetColorAtTime(0), L"Time 0");
        Assert::IsTrue(color1 == timeline.GetColorAtTime(100), L"Time 100");
        Assert::IsTrue(LightColor(101, 101, 101, 101) == timeline.GetColorAtTime(101), L"Time 101");
        Assert::IsTrue(LightColor(150, 150, 150, 150) == timeline.GetColorAtTime(150), L"Time 150");
        Assert::IsTrue(LightColor(199, 199, 199, 199) == timeline.GetColorAtTime(199), L"Time 199");
        Assert::IsTrue(color2 == timeline.GetColorAtTime(200), L"Time 200");
    }

    TEST_METHOD(NewInstant) {
        LightColor color(1, 2, 3, 4);
        LightColorTimeline timeline = LightColorTimeline::NewInstant(color);

        Assert::IsTrue(color == timeline.GetColorAtTime(0), L"Color");
        Assert::AreEqual<unsigned long>(0, timeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewMorph) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        unsigned long morphTime = 1000;
        LightColorTimeline timeline = LightColorTimeline::NewMorph(startColor, endColor, morphTime);

        Assert::IsTrue(startColor == timeline.GetColorAtTime(0), L"StartColor");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(morphTime), L"EndColor");
        Assert::AreEqual<unsigned long>(morphTime, timeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewMorphDelayed) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        unsigned long morphTime = 1000;
        unsigned long startDelay = 500;
        LightColorTimeline timeline = LightColorTimeline::NewMorph(startColor, endColor, morphTime, startDelay);

        Assert::IsTrue(startColor == timeline.GetColorAtTime(startDelay), L"StartColor");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startDelay + morphTime), L"EndColor");
        Assert::AreEqual<unsigned long>(startDelay + morphTime, timeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewPulse) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        LightColor resetColor(0, 0, 0, 0);
        unsigned long pulseTime = 1000;
        unsigned long pulseAmount = 2;
        unsigned long startColorHoldTime = 50;
        unsigned long endColorHoldTime = 100;
        LightColorTimeline timeline = LightColorTimeline::NewPulse(startColor, endColor, resetColor, pulseTime, pulseAmount, startColorHoldTime, endColorHoldTime);

        Assert::IsTrue(startColor == timeline.GetColorAtTime(0), L"Pulse 1, phase 1");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startColorHoldTime / 2), L"Pulse 1, phase 2");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startColorHoldTime / 2 + pulseTime / 2), L"Pulse 1, phase 3");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startColorHoldTime / 2 + pulseTime / 2 + endColorHoldTime), L"Pulse 1, phase 4");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 1, phase 5");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startColorHoldTime + pulseTime + endColorHoldTime), L"Pulse 1, phase 6");

        unsigned long startPulse2 = startColorHoldTime + pulseTime + endColorHoldTime;
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startPulse2), L"Pulse 2, phase 1");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2), L"Pulse 2, phase 2");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime / 2), L"Pulse 2, phase 3");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime / 2 + endColorHoldTime), L"Pulse 2, phase 4");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 2, phase 5");
        Assert::IsTrue(resetColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime + pulseTime + endColorHoldTime), L"Pulse 2, phase 6");

        Assert::AreEqual<unsigned long>((pulseTime + startColorHoldTime + endColorHoldTime) * pulseAmount, timeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewPulseDelayed) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        LightColor resetColor(0, 0, 0, 0);
        unsigned long pulseTime = 1000;
        unsigned long pulseAmount = 2;
        unsigned long startColorHoldTime = 50;
        unsigned long endColorHoldTime = 100;
        unsigned long startDelay = 500;
        LightColorTimeline timeline = LightColorTimeline::NewPulse(startColor, endColor, resetColor, pulseTime, pulseAmount, startColorHoldTime, endColorHoldTime, startDelay);

        Assert::IsTrue(startColor == timeline.GetColorAtTime(startDelay), L"Pulse 1, phase 1");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startDelay + startColorHoldTime / 2), L"Pulse 1, phase 2");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startDelay + startColorHoldTime / 2 + pulseTime / 2), L"Pulse 1, phase 3");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startDelay + startColorHoldTime / 2 + pulseTime / 2 + endColorHoldTime), L"Pulse 1, phase 4");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startDelay + startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 1, phase 5");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startDelay + startColorHoldTime + pulseTime + endColorHoldTime), L"Pulse 1, phase 6");

        unsigned long startPulse2 = startDelay + startColorHoldTime + pulseTime + endColorHoldTime;
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startPulse2), L"Pulse 2, phase 1");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2), L"Pulse 2, phase 2");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime / 2), L"Pulse 2, phase 3");
        Assert::IsTrue(endColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime / 2 + endColorHoldTime), L"Pulse 2, phase 4");
        Assert::IsTrue(startColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 2, phase 5");
        Assert::IsTrue(resetColor == timeline.GetColorAtTime(startPulse2 + startColorHoldTime + pulseTime + endColorHoldTime), L"Pulse 2, phase 6");

        Assert::AreEqual<unsigned long>((pulseTime + startColorHoldTime + endColorHoldTime) * pulseAmount + startDelay, timeline.GetTotalDuration(), L"TotalDuration");
    }

    };

}
