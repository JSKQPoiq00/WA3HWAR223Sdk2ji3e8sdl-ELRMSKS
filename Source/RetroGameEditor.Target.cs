// palinoia interactive 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class RetroGameEditorTarget : TargetRules
{
	public RetroGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "RetroGame" } );
	}
}
