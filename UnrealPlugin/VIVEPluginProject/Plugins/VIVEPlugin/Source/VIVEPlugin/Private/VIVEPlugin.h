#pragma once

#include "ModuleManager.h"

class VIVEPluginImpl : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};