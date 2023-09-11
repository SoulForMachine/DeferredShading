#include "Application.h"
#include "DeferredRenderer.h"


int main()
{
	Application framework("Deferred shading", 1024, 768);
	framework.Run(new DeferredRenderer);
}
