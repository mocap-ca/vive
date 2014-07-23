

#include "VIVEPluginProject.h"
#include "VIVEPluginProjectGameMode.h"
#include "VIVEPluginProjectPlayerController.h"

AVIVEPluginProjectGameMode::AVIVEPluginProjectGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PlayerControllerClass = AVIVEPluginProjectPlayerController::StaticClass();
}


