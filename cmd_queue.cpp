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

struct cmdqueuehdl
{
	cmdqueuehdl()
	{
		ref = NULL;
	}
	cmdqueuehdl(char *c, void *r)
	{
		strcpy(cmd, c);
		ref = r;
	}
	~cmdqueuehdl()
	{
	}

	char cmd[256];	// The console command to execute
	void *ref;		// The selected reference
};

#define QUEUESIZE 100
cmdqueuehdl cmdlist[QUEUESIZE];
int start = 0, end = 0;
void *consoleMutex = NULL;
void *mutex = NULL;

void cqInit()
{
	start = 0;
	end = 0;
	mutex = CreateMutex(NULL, FALSE, NULL);
	consoleMutex = CreateMutex(NULL, FALSE, NULL);
}

void cqRelease()
{
	start = 0;
	end = 0;
}

void cqPushCommand(char *cmd, void *ref)
{
	WaitForSingleObject(mutex, INFINITE);
	strcpy(cmdlist[end].cmd, cmd);
	cmdlist[end].ref = ref;
		
	end = (end+1)%QUEUESIZE;
	ReleaseMutex(mutex);
}

void cqExecuteCommand()
{
	WaitForSingleObject(mutex, INFINITE);
	if (start != end)
	{
		ExecuteConsoleCommand(cmdlist[start].cmd, cmdlist[start].ref);
		start = (start+1)%QUEUESIZE;
	}
	ReleaseMutex(mutex);
}

void tsWait(uint t)
{
	WaitForSingleObject(consoleMutex, INFINITE);
	Wait(0);
	ReleaseMutex(consoleMutex);
}

void tsStart()
{
	WaitForSingleObject(consoleMutex, INFINITE);
}

void tsEnd()
{
	ReleaseMutex(consoleMutex);
}