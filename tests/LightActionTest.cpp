#include "CppUnitTest.h"

// Project includes
#include "../src/LightAction.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;

namespace lightfx_tests {

    TEST_CLASS(LightActionTest) {
public:

    TEST_METHOD(Constuctor) {
        LightAction lightAction(1);
        Assert::AreEqual(size_t(1), lightAction.GetStartColor().size(), L"StartColor");
        Assert::AreEqual(size_t(1), lightAction.GetEndColor().size(), L"EndColor");
        Assert::AreEqual(size_t(1), lightAction.GetCurrentColor().size(), L"CurrentColor");
    }

    TEST_METHOD(NewInstant) {
        LightColor color(1, 2, 3, 4);
        LightAction lightAction = LightAction::NewInstant(1, color);
        Assert::IsTrue(LightActionType::Instant == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(color == lightAction.GetEndColor(0), L"Color");
    }

    TEST_METHOD(NewMorph) {
        LightColor startColor(1, 2, 3, 4);
        LightColor endColor(5, 6, 7, 8);
        unsigned int time = 2000;
        LightAction lightAction = LightAction::NewMorph(1, startColor, endColor, time);
        Assert::IsTrue(LightActionType::Morph == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(startColor == lightAction.GetStartColor(0), L"StartColor");
        Assert::IsTrue(endColor == lightAction.GetEndColor(0), L"EndColor");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
    }

    TEST_METHOD(NewPulse1) {
        LightColor startColor(1, 2, 3, 4);
        LightColor endColor(5, 6, 7, 8);
        unsigned int time = 2000;
        unsigned int repeat = 10;
        LightAction lightAction = LightAction::NewPulse(1, startColor, endColor, time, repeat);
        Assert::IsTrue(LightActionType::Pulse == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(startColor == lightAction.GetStartColor(0), L"StartColor");
        Assert::IsTrue(endColor == lightAction.GetEndColor(0), L"EndColor");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(NewPulse2) {
        LightColor startColor(1, 2, 3, 4);
        LightColor endColor(5, 6, 7, 8);
        unsigned int time = 2000;
        unsigned int startHoldTime = 20;
        unsigned int endHoldTime = 30;
        unsigned int repeat = 10;
        LightAction lightAction = LightAction::NewPulse(1, startColor, endColor, time, startHoldTime, endHoldTime, repeat);
        Assert::IsTrue(LightActionType::Pulse == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(startColor == lightAction.GetStartColor(0), L"StartColor");
        Assert::IsTrue(endColor == lightAction.GetEndColor(0), L"EndColor");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(startHoldTime, lightAction.GetStartColorHoldTime(), L"StartColorHoldTime");
        Assert::AreEqual(endHoldTime, lightAction.GetEndColorHoldTime(), L"EndColorHoldTime");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(NewPulse3) {
        LightColor startColor(1, 2, 3, 4);
        LightColor endColor(5, 6, 7, 8);
        LightColor resetColor(9, 10, 11, 12);
        unsigned int time = 2000;
        unsigned int startHoldTime = 20;
        unsigned int endHoldTime = 30;
        unsigned int repeat = 10;
        LightAction lightAction = LightAction::NewPulse(1, startColor, endColor, resetColor, time, startHoldTime, endHoldTime, repeat);
        Assert::IsTrue(LightActionType::Pulse == lightAction.GetActionType(), L"ActionType");
        Assert::IsTrue(startColor == lightAction.GetStartColor(0), L"StartColor");
        Assert::IsTrue(endColor == lightAction.GetEndColor(0), L"EndColor");
        Assert::IsTrue(resetColor == lightAction.GetResetColor(0), L"ResetColor");
        Assert::AreEqual(time, lightAction.GetActionTime(), L"Time");
        Assert::AreEqual(startHoldTime, lightAction.GetStartColorHoldTime(), L"StartColorHoldTime");
        Assert::AreEqual(endHoldTime, lightAction.GetEndColorHoldTime(), L"EndColorHoldTime");
        Assert::AreEqual(repeat, lightAction.GetActionRepeatAmount(), L"RepeatAmount");
    }

    TEST_METHOD(SetStartColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction(1);
        lightAction.SetStartColor(0, color);
        Assert::IsTrue(color == lightAction.GetStartColor(0));
    }

    TEST_METHOD(SetEndColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction(1);
        lightAction.SetEndColor(0, color);
        Assert::IsTrue(color == lightAction.GetEndColor(0));
    }

    TEST_METHOD(SetResetColor) {
        LightColor color(11, 22, 33, 44);
        LightAction lightAction(1);
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
