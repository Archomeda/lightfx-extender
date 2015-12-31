#include "CppUnitTest.h"

// Project includes
#include "../src/LFXE/Timelines/LightTimelineItem.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx::timelines;

namespace lightfx_tests {
    namespace timelines {

        TEST_CLASS(LightTimelineItemTest) {
public:

    TEST_METHOD(Constuctor) {
        LightColor color(1, 2, 3, 4);
        unsigned long duration = 2000;
        long startDelay = 100;
        LightTimelineItem lightTimelineItem(color, duration, startDelay);

        Assert::IsTrue(color == lightTimelineItem.GetColor(), L"Color");
        Assert::AreEqual(duration, lightTimelineItem.GetDuration(), L"Duration");
        Assert::AreEqual(startDelay, lightTimelineItem.GetStartDelay(), L"StartDelay");
    }

    TEST_METHOD(SetColor) {
        LightColor color(1, 2, 3, 4);
        LightTimelineItem lightTimelineItem;
        lightTimelineItem.SetColor(color);

        Assert::IsTrue(color == lightTimelineItem.GetColor());
    }

    TEST_METHOD(SetDuration) {
        unsigned long duration = 2000;
        LightTimelineItem lightTimelineItem;
        lightTimelineItem.SetDuration(duration);

        Assert::AreEqual(duration, lightTimelineItem.GetDuration());
    }

    TEST_METHOD(SetStartDelay) {
        long startDelay = 100;
        LightTimelineItem lightTimelineItem;
        lightTimelineItem.SetStartDelay(startDelay);

        Assert::AreEqual(startDelay, lightTimelineItem.GetStartDelay());
    }

        };

    }
}
