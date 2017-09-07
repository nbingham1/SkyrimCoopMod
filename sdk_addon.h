#ifndef sdk_addon_h
#define sdk_addon_h

void IniReadString(char *inifile, char *section, char *param, char *def, char *output, int length);

uint GetWeather();

// SetTimeScale - Sets the time scale.
uint TimeDifferenceToTimeScale(float diff);
void SetTimeScale(int i_Timescale);
uint GetCurrentTimeScale();

namespace ID_TESIdleForm
{
	enum
	{
		WolfIdleWarn = 0x00037F8E,
		WolfIdleToRunRight90 = 0x000B0102,
		WolfIdleToRunRight180 = 0x000B0103,
		WolfIdleToRunLeft90 = 0x000B0100,
		WolfIdleToRunLeft180 = 0x000B0101,
		WolfIdleStop = 0x000A8677,
		WolfIdleHowl = 0x000A82BE,
		CharacterTurnStop = 0x00095A01,
		CharacterTurnRight = 0x00095A00,
		CharacterTurnLeft = 0x000959FF,
		CharacterMoveStop = 0x000959FB,
		CharacterMoveStart = 0x000959FA,
		ForwardJump = 0x00044F45,
		HorseJumpRoot = 0x00044F44,
		JumpDirectionalStart = 0x00088483,
		JumpFall = 0x000A791D,
		JumpFallDirectional = 0x000A7912,
		JumpLand = 0x000A8524,
		JumpLandDirectional = 0x000A8525,
		JumpLandSwimming = 0x0010E835,
		JumpRoot = 0x00088302,
		JumpSoftEnd = 0x000A87BD,
		JumptSoftEndFailSafe = 0x000ABEF8,
		JumpStandingStart = 0x000884A2
	};
}

namespace ID_TESRace
{
	enum
	{
		AlduinRace = 0x000E7713,
		ArgonianRace = 0x00013740,
		ArgonianRaceVampire = 0x0008883A,
		AtronachFlameRace = 0x000131F5,
		AtronachFrostRace = 0x000131F6,
		AtronachStormRace = 0x000131F7,
		BearBlackRace = 0x000131E8,
		BearBrownRace = 0x000131E7,
		BearSnowRace = 0x000131E9,
		BretonRace = 0x00013741,
		BretonRaceChild = 0x0002C65C,
		BretonRaceChildVampire = 0x00108272,
		BretonRaceVampire = 0x00088836,
		C00GiantOutsideWhiterunRace = 0x000CAE13,
		C06WolfSpiritRace = 0x00106C10,
		CartHorseRace = 0x000DE505,
		ChaurusRace = 0x000131EB,
		ChaurusReaperRace = 0x000A5601,
		ChickenRace = 0x000A919D,
		CowRace = 0x0004E785,
		DA03BarbasDogRace = 0x000CD657,
		DA13AfflictedRace = 0x00097A3D,
		DarkElfRace = 0x00013742,
		DarkElfRaceVampire = 0x0008883D,
		DeerRace = 0x000CF89B,
		DefaultRace = 0x00000019,
		DogCompanionRace = 0x000F1AC4,
		DogRace = 0x00012E82,
		DraugrMagicRace = 0x000F71DC,
		DraugrRace = 0x00000D53,
		DremoraRace = 0x000131F0,
		dunMiddenEmptyRace = 0x000B214B,
		DwarvenCenturionRace = 0x000131F1,
		DwarvenSphereRace = 0x000131F2,
		DwarvenSpiderRace = 0x000131F3,
		ElderRace = 0x00067CD8,
		ElderRaceVampire = 0x000A82BA,
		ElkRace = 0x000131ED,
		FalmerRace = 0x000131F4,
		FoxRace = 0x00109C7C,
		FrostbiteSpiderRace = 0x000131F8,
		FrostbiteSpiderRaceGiant = 0x0004E507,
		FrostbiteSpiderRaceLarge = 0x00053477,
		GiantRace = 0x000131F9,
		HagravenRace = 0x000131FB,
		HareRace = 0x0006DC99,
		HighElfRace = 0x00013743,
		HighElfRaceVampire = 0x00088840,
		HarkerRace = 0x000131FC,
		HorseRace = 0x000131FD,
		IceWraithRace = 0x000131FE,
		ImperialRace = 0x00013744,
		ImperialRaceChild = 0x0002C659,
		ImperialRaceVampire = 0x00088844,
		InvisibleRace = 0x00071E6A,
		KhajiitRace = 0x00013745,
		KhajiitRaceVampire = 0x00088845,
		MagicAnomalyRace = 0x000B6F95,
		MammothRace = 0x000131FF,
		ManakinRace = 0x0010760A,
		MG07DogRace = 0x000F905F,
		MudcrabRace = 0x000BA545,
		NordRace = 0x00013746,
		NordRaceAstrid = 0x0007EAF3,
		NordRaceChild = 0x0002C65B,
		NordRaceVampire = 0x00088794,
		OrcRace = 0x00013747,
		OrcRaceVampire = 0x000A82B9,
		RedguardRace = 0x00013748,
		RedguardRaceVampire = 0x00088846,
		RigidSkeletonRace = 0x000B9FD7,
		SabreCatRace = 0x00013200,
		SabreCatSnowyRace = 0x00013202,
		SkeeverRace = 0x00013201,
		SkeeverWhiteRace = 0x000C3EDF,
		SkeletonNecroPriestRace = 0x000EBE18,
		SkeletonNecroRace = 0x000EB872,
		SkeletonRace = 0x000B7998,
		SlaughterfishRace = 0x00013203,
		SprigganMatronRace = 0x000F3903,
		SprigganRace = 0x00013204,
		SprigganSwarmRace = 0x0009AA44,
		SwarmRace = 0x0009AA3C,
		TrollFrostRace = 0x00013206,
		TrollRace = 0x00013205,
		UndeadDragonRace = 0x001052A3,
		WerewolfBeastRace = 0x000CDD84,
		WhiteStagRace = 0x00104F45,
		WispRace = 0x00013208,
		WispShadeRace = 0x000F1182,
		WitchlightRace = 0x00013209,
		WolfRace = 0x0001320A,
		WoodElfRace = 0x00013749,
		WoodElfRaceVampire = 0x00088884
	};
}


#endif