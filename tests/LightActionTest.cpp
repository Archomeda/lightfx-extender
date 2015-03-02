#include "CppUnitTest.h"

// Project includes
#include "../src/LightAction.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;

namespace lightfx_tests {

    TEST_CLASS(LightActionTest) {
public:

    TEST_METHOD(Constuctor1) {
        LightColor color(1, 2, 3, 4);
        LightAction lightAction(color);
        Assert::IsTrue(color == lightAction.GetStartColor());
    }

    TEST_METHOD(Constuctor2) {
        LightActionType type = LightActionType::Morph;
        LightColor color(1, 2, 3, 4);
        unsigned int time = 2000;
        LightAction lightAction(type, color, time);
        Assert::IsTrue(type == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(color == lightAction.GetStartColor(), L"Color");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
    }

    TEST_METHOD(Constuctor3) {
        LightActionType type = LightActionType::Morph;
        LightColor color(1, 2, 3, 4);
        unsigned int time = 2000;
        unsigned int repeat = 10;
        LightAction lightAction(type, color, time, repeat);
        Assert::IsTrue(type == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(color == lightAction.GetStartColor(), L"Color");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(Constuctor4) {
        LightActionType type = LightActionType::Morph;
        LightColor startColor(1, 2, 3, 4);
        LightColor endColor(5, 6, 7, 8);
        unsigned int time = 2000;
        unsigned int startHoldTime = 20;
        unsigned int endHoldTime = 30;
        unsigned int repeat = 10;
        LightAction lightAction(type, startColor, endColor, time, startHoldTime, endHoldTime, repeat);
        Assert::IsTrue(type == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(startColor == lightAction.GetStartColor(), L"StartColor");
        Assert::IsTrue(endColor == lightAction.GetEndColor(), L"EndColor");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(startHoldTime, lightAction.GetStartColorHoldTime(), L"StartColorHoldTime");
        Assert::AreEqual(endHoldTime, lightAction.GetEndColorHoldTime(), L"EndColorHoldTime");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(SetStartColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction;
        lightAction.SetStartColor(color);
        Assert::IsTrue(color == lightAction.GetStartColor());
    }

    TEST_METHOD(SetEndColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction;
        lightAction.SetEndColor(color);
        Assert::IsTrue(color == lightAction.GetEndColor());
    }

    TEST_METHOD(SetActionType) {
        LightActionType type = LightActionType::Pulse;
        LightAction lightAction;
        lightAction.SetActionType(type);
        Assert::IsTrue(type == lightAction.GetActionType());
    }

    TEST_METHOD(SetActionTime) {
        unsigned int time = 1000;
        LightAction lightAction;
        lightAction.SetActionTime(time);
        Assert::AreEqual(time, lightAction.GetActionTime());
    }

    TEST_METHOD(SetStartColorHoldTime) {
        unsigned int time = 1000;
        LightAction lightAction;
        lightAction.SetStartColorHoldTime(time);
        Assert::AreEqual(time, lightAction.GetStartColorHoldTime());
    }

    TEST_METHOD(SetEndColorHoldTime) {
        unsigned int time = 1000;
        LightAction lightAction;
        lightAction.SetEndColorHoldTime(time);
        Assert::AreEqual(time, lightAction.GetEndColorHoldTime());
    }

    TEST_METHOD(SetActionRepeatAmount) {
        unsigned int repeat = 10;
        LightAction lightAction;
        lightAction.SetActionRepeatAmount(repeat);
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount());
    }

    };

}
