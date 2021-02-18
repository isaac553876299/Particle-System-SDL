#include <iostream>
#include "App.h"

int main(int argc, char** argv)
{
	App* example = new App;
	while (example->Running());
	RELEASE(example);
	return 0;
}
