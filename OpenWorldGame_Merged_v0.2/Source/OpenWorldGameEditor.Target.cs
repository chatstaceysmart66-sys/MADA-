using UnrealBuildTool;
using System.Collections.Generic;

public class OpenWorldGameEditorTarget : TargetRules
{
    public OpenWorldGameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("OpenWorldGame");
    }
}
