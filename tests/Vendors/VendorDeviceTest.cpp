#include "CppUnitTest.h"

// Project includes
#include "VendorBaseMock.h"
#include "../../src/Vendors/VendorDevice.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::timelines;
using namespace lightfx::vendors;

namespace lightfx_tests {
    namespace vendors {

        TEST_CLASS(VendorDeviceTest) {
public:

    TEST_METHOD(SetVendor) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        Assert::IsTrue(vendor == vendorDevice.GetVendor());
    }

    TEST_METHOD(SetHardwareId) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        unsigned int hardwareId = 42;

        vendorDevice.SetHardwareId(hardwareId);
        Assert::AreEqual(hardwareId, vendorDevice.GetHardwareId());
    }

    TEST_METHOD(SetDeviceName) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        wstring name = L"TestDevice";

        vendorDevice.SetDeviceName(name);
        Assert::AreEqual(name, vendorDevice.GetDeviceName());
    }

    TEST_METHOD(SetDeviceType) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        VendorDeviceType type = VendorDeviceType::DeviceOther;

        vendorDevice.SetDeviceType(type);
        Assert::AreEqual(static_cast<unsigned int>(type), static_cast<unsigned int>(vendorDevice.GetDeviceType()));
    }

    TEST_METHOD(SetNumberOfLights) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        size_t lights = 12;

        vendorDevice.SetNumberOfLights(lights);
        Assert::AreEqual(lights, vendorDevice.GetNumberOfLights());
    }

    TEST_METHOD(SetLightData) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        VendorDeviceLightData data = {};
        data.name = L"TestLight";

        vendorDevice.SetNumberOfLights(1);
        vendorDevice.SetLightData(0, data);
        Assert::AreEqual(data.name, vendorDevice.GetLightData(0).name);
    }

    TEST_METHOD(SetQueuedTimeline) {
        auto vendor = make_shared<VendorBaseMock>();
        VendorDevice vendorDevice(vendor);
        Timeline timeline = Timeline(1, LightTimeline::NewInstant(LightColor(1, 2, 3, 4)));

        vendorDevice.SetQueuedTimeline(timeline);
        Assert::IsTrue(timeline.GetColorAtTime(0) == vendorDevice.GetQueuedTimeline().GetColorAtTime(0));
    }

        };

    }
}
