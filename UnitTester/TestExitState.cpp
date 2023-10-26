#include "../UnitTester/Defines.h"

#ifdef _EXITSTATE_OBJ_
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include "../ExitState.h"
#include <iostream>
#include <string>
using std::cout;

int numExitStateTests;

inline bool exists_test3(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

TEST(ExitState_Init)
{
	cout << "Running Test: (48) ExitState_Init...\n\tResult: ";
	SaiphApp::SetScore(12345); // put score in data field
	bool success = false;
	ExitState exit;
	exit.Init(); // serialize to disk
	SaiphApp::SetScore(9876); // swap out score

	if (exists_test3("Resources/config.xml")) {
		SaiphApp::LoadConfigData("Resources/config.xml"); // try loading old score back in
		success = (12345 == SaiphApp::GetScore());
	}

	if(success) {
		cout<<"SUCCESS!!\n\n";
	} else {
		cout<<"Failure!\n\n";
	}
	CHECK(success);
	exit.Shutdown();
	SaiphApp::CleanUp();
}

TEST(ExitState_Heartbeat)
{
	cout << "Running Test: (9) ExitState_Heartbeat...\n\tResult: ";
	SaiphApp::SetScore(12345);
	bool success = false;
	ExitState exit;
	exit.Init();
	success = (exit.Heartbeat(0.1f) == NOCHANGE);

	if (success)
	{
		cout << "SUCCESS!!\n\n";
	}
	else
	{
		cout << "Failure!\n\n";
	}
	CHECK(success);
	exit.Shutdown();
}
#endif _EXITSTATE_OBJ_
