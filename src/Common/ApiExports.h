#pragma once

#ifndef LFXE_API

#ifdef LFXE_TESTING

#ifdef LFXE_EXPORTS
#define LFXE_API _declspec(dllexport)
#else
#define LFXE_API _declspec(dllimport)
#endif

#else
#define LFXE_API
#endif

#endif
