using UnrealBuildTool;
using System.IO;

public class VIVEPlugin : ModuleRules
{
    public VIVEPlugin(TargetInfo Target)
    {
        PrivateIncludePaths.AddRange(new string[] { "VIVEPlugin/Private" });
        PublicIncludePaths.AddRange(new string[] { "VIVEPlugin/Public" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets" });
    }
}