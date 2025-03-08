// MentalEngine.cpp: определяет точку входа для приложения.
//

#include "MentalEngine.h"
#include "Application.hpp"

using namespace std;

int main()
{
	Application app = Application("Mental Engine (OpenGL).");
	app.run();
	return 0;
}
