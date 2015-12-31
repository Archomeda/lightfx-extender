#include "CppUnitTest.h"
#include "ManagerMock.h"
#include "../../LFXE/LightFXExtender.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::managers;

namespace lightfx_tests {
    namespace managers {

        TEST_CLASS(ManagerTest) {
public:

    TEST_METHOD(SetLightFXExtender) {
        auto lightFXExtender = make_shared<LightFXExtender>();
        auto manager = make_shared<ManagerMock>();
        manager->SetLightFXExtender(lightFXExtender->shared_from_this());
        Assert::IsTrue(lightFXExtender == manager->GetLightFXExtender());
    }

        };
    }
}
