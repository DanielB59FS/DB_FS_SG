// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the STRATEGYTWO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// STRATEGYTWO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef STRATEGYTWO_EXPORTS
#define STRATEGYTWO_API __declspec(dllexport)
#else
#define STRATEGYTWO_API __declspec(dllimport)
#endif

#include "../Model/IDeflectionStrategy.h"

class CStrategyTwo : public IDeflectionStrategy {
public:
	void Deflect(float &bulletHeading) const;
	void Destroy();
};

extern "C" STRATEGYTWO_API IDeflectionStrategy* CreateStrategy();
