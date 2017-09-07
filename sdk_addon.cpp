#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "common\skyscript.h"
#include "common\obscript.h"
#include "common\types.h"
#include "common\enums.h"
#include "common\plugin.h"

#include "sdk_addon.h"
#include "cmd_queue.h"
#include "ref_list.h"
#include "packets.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

extern HMODULE g_hModule;
extern std::string GetPathFromFilename(std::string filename);

void IniReadString(char *inifile, char *section, char *param, char *def, char *output, int length)
{
	char curdir[MAX_PATH];
	GetModuleFileNameA(g_hModule, curdir, sizeof(curdir));
	std::string fname = GetPathFromFilename(curdir) + inifile;
	GetPrivateProfileStringA(section, param, def, output, length, fname.c_str());
}

uint GetWeather()
{
	TESWeather *weather = Weather::GetCurrentWeather();
	if (weather != NULL)
		return Form::GetFormID((TESForm*)weather);
	else
		return 0;
}

int timescaleMin = 20;
int timescaleMax = 100000;
uint currtimescale = 20;

uint TimeDifferenceToTimeScale(float diff)
{
	return max(20*(int)((10002.0f/(1.0f+exp(diff)) - 5000.0f)), 20);
}

// SetTimeScale - Sets the time scale.
void SetTimeScale(int i_Timescale)
{
	// Enforce minimums/maximums.
	if ( i_Timescale < timescaleMin )
		i_Timescale = timescaleMin;
	if ( i_Timescale > timescaleMax ) 
		i_Timescale = timescaleMax;

	// Build command.
	char command[64];
	sprintf_s(command, "set timescale to %d", i_Timescale);

	// Change timescale.
	cqPushCommand(command, NULL);
	currtimescale = i_Timescale;
}

uint GetCurrentTimeScale()
{
	return currtimescale;
}