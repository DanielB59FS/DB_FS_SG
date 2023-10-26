#include <iostream>
#include "Defines.h"
#include "Bugs.h"
#include "../UnitTest++/UnitTest++.h"
#include "../SaiphApp.h"
#include "../View/ViewManager.h"

int main()
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if(0 == UnitTest::RunAllTests())
		std::cout << "\n\n\t\t (\\_/) ,Nice Work!\n\t\t=(^.^)=\n\t\t(\")_(\")\n\nASCII-Bunny congratulates you on a job well done!\n\n\n";

	system("pause");
	ViewManager::GetInstance().Shutdown();
}