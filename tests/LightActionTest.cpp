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
        LightAction lightAction(1, color);
        Assert::IsTrue(color == lightAction.GetStartColor(0));
    }

    TEST_METHOD(Constuctor2) {
        LightAction lightAction(1);
        Assert::AreEqual(size_t(1), lightAction.GetCurrentColor().size());
    }

    TEST_METHOD(Constuctor3) {
        LightActionType type = LightActionType::Morph;
        LightColor color(1, 2, 3, 4);
        unsigned int time = 2000;
        LightAction lightAction(type, 1, color, time);
        Assert::IsTrue(type == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(color == lightAction.GetStartColor(0), L"Color");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
    }

    TEST_METHOD(Constuctor4) {
        LightActionType type = LightActionType::Morph;
        LightColor color(1, 2, 3, 4);
        unsigned int time = 2000;
        unsigned int repeat = 10;
        LightAction lightAction(type, 1, color, time, repeat);
        Assert::IsTrue(type == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(color == lightAction.GetStartColor(0), L"Color");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(Constuctor5) {
        LightActionType type = LightActionType::Morph;
        LightColor startColor(1, 2, 3, 4);
        LightColor endColor(5, 6, 7, 8);
        unsigned int time = 2000;
        unsigned int startHoldTime = 20;
        unsigned int endHoldTime = 30;
        unsigned int repeat = 10;
        LightAction lightAction(type, 1, startColor, endColor, time, startHoldTime, endHoldTime, repeat);
        Assert::IsTrue(type == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(startColor == lightAction.GetStartColor(0), L"StartColor");
        Assert::IsTrue(endColor == lightAction.GetEndColor(0), L"EndColor");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(startHoldTime, lightAction.GetStartColorHoldTime(), L"StartColorHoldTime");
        Assert::AreEqual(endHoldTime, lightAction.GetEndColorHoldTime(), L"EndColorHoldTime");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(SetStartColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction(1, LightColor());
        lightAction.SetStartColor(0, color);
        Assert::IsTrue(color == lightAction.GetStartColor(0));
    }

    TEST_METHOD(SetEndColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction(1, LightColor());
        lightAction.SetEndColor(0, color);
        Assert::IsTrue(color == lightAction.GetEndColor(0));
    }

    TEST_METHOD(SetResetColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction(1, LightColor());
        lightAction.SetResetColor(0, color);
        Assert::IsTrue(color == lightAction.GetResetColor(0));
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
