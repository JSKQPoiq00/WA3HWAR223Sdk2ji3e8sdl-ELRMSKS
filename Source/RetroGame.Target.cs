// palinoia interactive 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class RetroGameTarget : TargetRules
{
	public RetroGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "RetroGame" } );
	}
}
