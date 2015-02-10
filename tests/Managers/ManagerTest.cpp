#include "CppUnitTest.h"
#include "../../src/Managers/ConfigManager.h"
#include "../../src/Config/MainConfigFile.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace lightfx;
using namespace lightfx::managers;
using namespace lightfx::config;

namespace managers {
    TEST_CLASS(ManagerTest) {
public:

    TEST_METHOD(SetLightFXExtender) {
        // Test template class by using a specialization of the class

        auto lightFXExtender = make_shared<LightFXExtender>();
        auto configManager = make_shared<ConfigManager>();
        configManager->SetLightFXExtender(lightFXExtender->shared_from_this());
        Assert::IsTrue(lightFXExtender == configManager->GetLightFXExtender());
    }

    TEST_METHOD(AddConfigFile) {
        // Test template class by using a specialization of the class

        wstring configKey = L"SomeConfigKey";
        auto configFile = make_shared<MainConfigFile>();
        auto configManager = make_shared<ConfigManager>();
        configManager->AddChild(configKey, configFile);

        Assert::IsTrue(configFile == configManager->GetChild(configKey));
        Assert::IsTrue(configManager == configFile->GetConfigManager());
    }

    };
}