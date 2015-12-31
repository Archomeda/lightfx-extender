#pragma once

// The initializer takes care of downloading a fresh copy of LFXE.dll
// into the %APPDATA% folder if it doesn't exist yet. This will only
// be done once when using LightFX Extender for the first time. This
// is to ensure that LFXE.dll is always available.

#include <string>

#include "Windows.h"

std::string InitializeLightFXExtender();
