#ifndef UNITTESTCPP_H
#define UNITTESTCPP_H

//lint -esym(1509,*Fixture)
#include "Config.h"
#include "Test.h"
#include "TestList.h"
#include "TestSuite.h"
#include "TestResults.h"

#include "TestMacros.h"

#include "CheckMacros.h"
#include "TestRunner.h"
#include "TimeConstraint.h"

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#endif
