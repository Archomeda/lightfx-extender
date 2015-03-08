#include "CppUnitTest.h"

// Project includes
#include "../src/timelines/LightTimeline.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::timelines;

namespace lightfx_tests {
    namespace timelines {

        TEST_CLASS(LightTimelineTest) {
public:

    TEST_METHOD(AddItem) {
        LightColor color(1, 2, 3, 4);
        LightTimelineItem lightTimelineItem(color, 0, 0);
        LightTimeline lightTimeline;

        lightTimeline.AddItem(lightTimelineItem);

        Assert::IsTrue(color == lightTimeline.GetColorAtTime(0));
    }

    TEST_METHOD(GetTotalDurationZero) {
        LightTimeline lightTimeline;

        lightTimeline.AddItem(LightTimelineItem(LightColor(), 0, 0));

        Assert::AreEqual<unsigned long>(0, lightTimeline.GetTotalDuration());
    }

    TEST_METHOD(GetTotalDuration) {
        LightTimeline lightTimeline;

        lightTimeline.AddItem(LightTimelineItem(LightColor(), 10, 20));
        lightTimeline.AddItem(LightTimelineItem(LightColor(), 30, 40));

        Assert::AreEqual<unsigned long>(100, lightTimeline.GetTotalDuration());
    }

    TEST_METHOD(GetColorAtTimeDelayedInstant) {
        LightColor color1(100, 100, 100, 100);
        LightColor color2(200, 200, 200, 200);
        LightTimelineItem lightTimelineItem1(color1, 0, 0);
        LightTimelineItem lightTimelineItem2(color2, 0, 100);
        LightTimeline lightTimeline;

        lightTimeline.AddItem(lightTimelineItem1);
        lightTimeline.AddItem(lightTimelineItem2);

        Assert::IsTrue(color1 == lightTimeline.GetColorAtTime(0), L"Time 0");
        Assert::IsTrue(color1 == lightTimeline.GetColorAtTime(99), L"Time 99");
        Assert::IsTrue(color2 == lightTimeline.GetColorAtTime(100), L"Time 100");
        Assert::IsTrue(color2 == lightTimeline.GetColorAtTime(101), L"Time 101");
    }

    TEST_METHOD(GetColorAtTimeDelayedTransition) {
        LightColor color1(100, 100, 100, 100);
        LightColor color2(200, 200, 200, 200);
        LightTimelineItem lightTimelineItem1(color1, 0, 0);
        LightTimelineItem lightTimelineItem2(color2, 100, 100);
        LightTimeline lightTimeline;

        lightTimeline.AddItem(lightTimelineItem1);
        lightTimeline.AddItem(lightTimelineItem2);

        Assert::IsTrue(color1 == lightTimeline.GetColorAtTime(0), L"Time 0");
        Assert::IsTrue(color1 == lightTimeline.GetColorAtTime(100), L"Time 100");
        Assert::IsTrue(LightColor(101, 101, 101, 101) == lightTimeline.GetColorAtTime(101), L"Time 101");
        Assert::IsTrue(LightColor(150, 150, 150, 150) == lightTimeline.GetColorAtTime(150), L"Time 150");
        Assert::IsTrue(LightColor(199, 199, 199, 199) == lightTimeline.GetColorAtTime(199), L"Time 199");
        Assert::IsTrue(color2 == lightTimeline.GetColorAtTime(200), L"Time 200");
    }

    TEST_METHOD(NewInstant) {
        LightColor color(1, 2, 3, 4);
        LightTimeline lightTimeline = LightTimeline::NewInstant(color);

        Assert::IsTrue(color == lightTimeline.GetColorAtTime(0), L"Color");
        Assert::AreEqual<unsigned long>(0, lightTimeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewMorph) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        unsigned long morphTime = 1000;
        LightTimeline lightTimeline = LightTimeline::NewMorph(startColor, endColor, morphTime);

        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(0), L"StartColor");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(morphTime), L"EndColor");
        Assert::AreEqual<unsigned long>(morphTime, lightTimeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewMorphDelayed) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        unsigned long morphTime = 1000;
        unsigned long startDelay = 500;
        LightTimeline lightTimeline = LightTimeline::NewMorph(startColor, endColor, morphTime, startDelay);

        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startDelay), L"StartColor");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startDelay + morphTime), L"EndColor");
        Assert::AreEqual<unsigned long>(startDelay + morphTime, lightTimeline.GetTotalDuration(), L"TotalDuration");
    }

    TEST_METHOD(NewPulse) {
        LightColor startColor(100, 100, 100, 100);
        LightColor endColor(200, 200, 200, 200);
        LightColor resetColor(0, 0, 0, 0);
        unsigned long pulseTime = 1000;
        unsigned long pulseAmount = 2;
        unsigned long startColorHoldTime = 50;
        unsigned long endColorHoldTime = 100;
        LightTimeline lightTimeline = LightTimeline::NewPulse(startColor, endColor, resetColor, pulseTime, pulseAmount, startColorHoldTime, endColorHoldTime);

        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(0), L"Pulse 1, phase 1");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startColorHoldTime / 2), L"Pulse 1, phase 2");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startColorHoldTime / 2 + pulseTime), L"Pulse 1, phase 3");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 1, phase 4");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startColorHoldTime / 2 + pulseTime * 2 + endColorHoldTime), L"Pulse 1, phase 5");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startColorHoldTime + pulseTime * 2 + endColorHoldTime), L"Pulse 1, phase 6");

        unsigned long startPulse2 = startColorHoldTime + pulseTime * 2 + endColorHoldTime;
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startPulse2), L"Pulse 2, phase 1");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2), L"Pulse 2, phase 2");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime), L"Pulse 2, phase 3");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 2, phase 4");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime * 2 + endColorHoldTime), L"Pulse 2, phase 5");
        Assert::IsTrue(resetColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime + pulseTime * 2 + endColorHoldTime), L"Pulse 2, phase 6");

        Assert::AreEqual<unsigned long>((pulseTime * 2 + startColorHoldTime + endColorHoldTime) * pulseAmount, lightTimeline.GetTotalDuration(), L"TotalDuration");
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
        LightTimeline lightTimeline = LightTimeline::NewPulse(startColor, endColor, resetColor, pulseTime, pulseAmount, startColorHoldTime, endColorHoldTime, startDelay);

        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startDelay), L"Pulse 1, phase 1");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startDelay + startColorHoldTime / 2), L"Pulse 1, phase 2");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startDelay + startColorHoldTime / 2 + pulseTime), L"Pulse 1, phase 3");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startDelay + startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 1, phase 4");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startDelay + startColorHoldTime / 2 + pulseTime * 2 + endColorHoldTime), L"Pulse 1, phase 5");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startDelay + startColorHoldTime + pulseTime * 2 + endColorHoldTime), L"Pulse 1, phase 6");

        unsigned long startPulse2 = startDelay + startColorHoldTime + pulseTime * 2 + endColorHoldTime;
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startPulse2), L"Pulse 2, phase 1");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2), L"Pulse 2, phase 2");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime), L"Pulse 2, phase 3");
        Assert::IsTrue(endColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime + endColorHoldTime), L"Pulse 2, phase 4");
        Assert::IsTrue(startColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime / 2 + pulseTime * 2 + endColorHoldTime), L"Pulse 2, phase 5");
        Assert::IsTrue(resetColor == lightTimeline.GetColorAtTime(startPulse2 + startColorHoldTime + pulseTime * 2 + endColorHoldTime), L"Pulse 2, phase 6");

        Assert::AreEqual<unsigned long>((pulseTime * 2 + startColorHoldTime + endColorHoldTime) * pulseAmount + startDelay, lightTimeline.GetTotalDuration(), L"TotalDuration");
    }

        };

    }
}
