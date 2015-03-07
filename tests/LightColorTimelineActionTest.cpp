#include "CppUnitTest.h"

// Project includes
#include "../src/LightColorTimelineAction.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;

namespace lightfx_tests {

    TEST_CLASS(LightColorTimelineActionTest) {
public:

    TEST_METHOD(Constuctor) {
        LightColor color(1, 2, 3, 4);
        unsigned long duration = 2000;
        unsigned long startDelay = 100;
        LightColorTimelineAction timelineAction(color, duration, startDelay);

        Assert::IsTrue(color == timelineAction.GetColor(), L"Color");
        Assert::AreEqual(duration, timelineAction.GetDuration(), L"Duration");
        Assert::AreEqual(startDelay, timelineAction.GetStartDelay(), L"StartDelay");
    }

    TEST_METHOD(SetColor) {
        LightColor color(1, 2, 3, 4);
        LightColorTimelineAction timelineAction;
        timelineAction.SetColor(color);

        Assert::IsTrue(color == timelineAction.GetColor());
    }

    TEST_METHOD(SetDuration) {
        unsigned long duration = 2000;
        LightColorTimelineAction timelineAction;
        timelineAction.SetDuration(duration);

        Assert::AreEqual(duration, timelineAction.GetDuration());
    }

    TEST_METHOD(SetStartDelay) {
        unsigned long startDelay = 100;
        LightColorTimelineAction timelineAction;
        timelineAction.SetStartDelay(startDelay);

        Assert::AreEqual(startDelay, timelineAction.GetStartDelay());
    }

    };

}
