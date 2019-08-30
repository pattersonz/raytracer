#ifndef RENDER
#define RENDER
#include <iostream>

#include "rayTrace.cpp"
#include "openGL.cpp"
#include "rasterize.cpp"
#include <string>

void removeTerm(int& argc, char**& argv, int toRemove)
{
	char** tempArgs = new char*[argc - 1];
	int j = 0;
	for (int i = 0; i < argc; i++)
		if (i != toRemove)
			tempArgs[j++] = argv[i];

	--argc;
	argv = tempArgs;

}


int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++)
	{
		if ( std::strcmp(argv[i],"--openGL") == 0)
		{
			removeTerm(argc, argv, i);
			return openGL(argc, argv);
		}
		else if ( std::strcmp(argv[i],"--rayTracer") == 0)
		{
			removeTerm(argc, argv, i);
			return rayTrace(argc, argv);
		}
		else if ( std::strcmp(argv[i],"--rasterizer") == 0)
		{
			removeTerm(argc, argv, i);
			return rasterize(argc, argv);
		}
	}
	std::cout << "Error no rendertype selected\n";
	return -1;
}

#endif