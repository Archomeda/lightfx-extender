#pragma once

// Windows Vista or higher is required
#define NTDDI_VERSION NTDDI_VISTA
#define _WIN32_WINNT _WIN32_WINNT_VISTA

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Disable the min and max macros since they are conflicting with C++11 functions
#ifndef NOMINMAX
#define NOMINMAX
#endif
