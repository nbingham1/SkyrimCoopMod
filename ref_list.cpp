#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#include "common\skyscript.h"
#include "common\obscript.h"
#include "common\types.h"
#include "common\enums.h"
#include "common\plugin.h"

#include "sdk_addon.h"
#include "cmd_queue.h"
#include "ref_list.h"
#include "packets.h"

#define RADIUS 2000.0f

reflisthdl reflist[20];
unsigned char id_translation_table[20][2];

uint weather = 0;
game_pkt packet;
CActor *player = NULL;
TESObjectREFR *player_ref = NULL;
void *countMutex = NULL;

uint num_actors = 0;
uint num_actorbases = 0;

bool paused = false;

reflisthdl::reflisthdl()
{
	actor_id	= 0;
	ref			= NULL;
	lookat		= NULL;
	form		= 0;
	state		= 0;
	health		= 0.0f;
	magicka		= 0.0f;
	stamina		= 0.0f;
	base_health	= 0.0f;
	base_magicka= 0.0f;
	base_stamina= 0.0f;
	x_offset	= 0.0f;
	y_offset	= 0.0f;
	z_offset	= 0.0f;
	flags		= 0;
	mutex		= NULL;
}

reflisthdl::~reflisthdl()
{
	actor_id	= 0;
	ref			= NULL;
	lookat		= NULL;
	form		= 0;
	state		= 0;
	health		= 0.0f;
	magicka		= 0.0f;
	stamina		= 0.0f;
	base_health	= 0.0f;
	base_magicka= 0.0f;
	base_stamina= 0.0f;
	x_offset	= 0.0f;
	y_offset	= 0.0f;
	z_offset	= 0.0f;
	flags		= 0;
	mutex		= NULL;
}

void rlInit()
{
	player = Game::GetPlayer();
	player_ref = (TESObjectREFR*)player;
	num_actors = 0;
	num_actorbases = 0;
	weather = 0;
	paused = false;
	countMutex = CreateMutex(NULL, FALSE, NULL);

	for (int i = 0; i < 20; i++)
	{
		id_translation_table[i][0] = 0;
		id_translation_table[i][1] = 0;
		if (reflist[i].mutex == NULL)
			reflist[i].mutex = CreateMutex(NULL, TRUE, NULL);
		else
			WaitForSingleObject(reflist[i].mutex, INFINITE);
		reflist[i].actor_id = i;
		ReleaseMutex(reflist[i].mutex);
	}
}

void rlRelease()
{
	player = Game::GetPlayer();
	player_ref = (TESObjectREFR*)player;
	num_actors = 0;
	num_actorbases = 0;
	weather = 0;
	paused = false;

	for (int i = 0; i < 20; i++)
	{
		WaitForSingleObject(reflist[i].mutex, INFINITE);
		reflist[i].flags = 0x00;
		ReleaseMutex(reflist[i].mutex);
	}
}

bool rlUpdate()
{
	uint open_id = 999;
	TESObjectREFR *act = NULL;
	TESForm *act_form = NULL;

	// Set the sizes
	uint i = 0;

	if (player_ref != NULL)
	{
		act = (TESObjectREFR*)Game::FindRandomActor(ObjectReference::GetPositionX(player_ref), 
													ObjectReference::GetPositionY(player_ref),
													ObjectReference::GetPositionZ(player_ref), RADIUS);
	}

	for (i = 0; act != NULL && i < 20; i++)
	{
		// This actor is already tracked no need to keep the pointer
		while (WaitForSingleObject(reflist[i].mutex, INFINITE) == WAIT_TIMEOUT);

		if ((reflist[i].flags & 0x02) > 0)
		{
			if (reflist[i].ref == act || reflist[i].lookat == act)
				act = NULL;
			// Check to see if this reference is still valid.
			// If not, remove the reference from the list.
			else if ((reflist[i].flags & 0x04) > 0 && ObjectReference::IsDisabled(reflist[i].ref))
			{
				WaitForSingleObject(countMutex, INFINITE);
				if ((reflist[i].flags & 0x01) == 0)
					num_actorbases--;
				else
					num_actorbases++;

				num_actors--;
				ReleaseMutex(countMutex);
				reflist[i].flags = 0x00;
			}
		}
		else if (open_id == 999)
			open_id = i;

		if (open_id != i)
			ReleaseMutex(reflist[i].mutex);
	}

	// If this actor is not being tracked, then we need to add it to the list
	if (act != NULL && open_id != 999)
	{
		/*reflist[open_id].ref = act;
		reflist[open_id].flags = 0x06;
		act_form = ObjectReference::GetBaseObject(act);
		if (act_form != NULL)
			reflist[open_id].form = Form::GetFormID(act_form);

		reflist[open_id].health = Actor::GetActorValue((CActor*)act, "Health");
		reflist[open_id].magicka = Actor::GetActorValue((CActor*)act, "Magicka");
		reflist[open_id].stamina = Actor::GetActorValue((CActor*)act, "Stamina");
		reflist[open_id].base_health = reflist[open_id].health;
		reflist[open_id].base_magicka = reflist[open_id].magicka;
		reflist[open_id].base_stamina = reflist[open_id].stamina;
		reflist[open_id].x_offset = ObjectReference::GetWidth(reflist[open_id].ref)/2.0f;
		reflist[open_id].y_offset = ObjectReference::GetLength(reflist[open_id].ref)/2.0f;
		reflist[open_id].z_offset = ObjectReference::GetHeight(reflist[open_id].ref)/2.0f;

		ReleaseMutex(reflist[open_id].mutex);

		WaitForSingleObject(countMutex, INFINITE);
		num_actors++;
		num_actorbases++;
		ReleaseMutex(countMutex);*/

		// To make sure that quest items are available to all players
		//if (act != (TESObjectREFR*)Game::GetPlayer())
		//	DuplicateItems(&reflist[open_id]);
	}

	return true;
}

bool rlSend(SOCKET connection)
{
	WaitForSingleObject(countMutex, INFINITE);
	packet.set_num_actors(num_actors);
	packet.set_num_actorbases(num_actorbases);
	ReleaseMutex(countMutex);

	packet.set_gametime(Utility::GetCurrentGameTime());
	packet.set_weather(GetWeather());

	uint flags = 0;

	if (Utility::IsInMenuMode())
	{
		flags |= GamePKTMenu;
		PrintNote("Menu Mode");
	}

	packet.set_flags(flags);

	uint a = 0;
	uint ab = 0;
	uint ib = 0;
	uint i = 0;
	uint size = 0;
	WaitForSingleObject(countMutex, INFINITE);
	uint na = num_actors;
	ReleaseMutex(countMutex);

	// Update the actors that are being tracked, and fill the packet.
	for (i = 0; a < na && i < 20; i++)
	{
		while (WaitForSingleObject(reflist[i].mutex, INFINITE) == WAIT_TIMEOUT);
		
		if ((reflist[i].flags & 0x02) > 0)
		{
			GetActorPkt(packet.get_actors(a++), &reflist[i]);
			size += sizeof(actor_pkt);

			if ((reflist[i].flags & 0x01) == 0 && size < GamePKTSize/2)
			{
				GetActorBasePkt(packet.get_actorbases(ab), &reflist[i]);
				size += packet.get_actorbases(ab++)->size();
				reflist[i].flags |= 0x01;
				WaitForSingleObject(countMutex, INFINITE);
				num_actorbases--;
				ReleaseMutex(countMutex);
			}
		}

		ReleaseMutex(reflist[i].mutex);
	}
	packet.set_num_actorbases(ab);

	return (send(connection, (char*)&packet, packet.size(), 0) != SOCKET_ERROR);
}

bool rlReceive(SOCKET connection, bool SetWeather)
{
	uint nexttimescale = 0;

	uint receive_length = recv(connection, (char*)&packet, sizeof(packet), 0);
	if (receive_length <= 0)
		return false;

	nexttimescale = TimeDifferenceToTimeScale(Utility::GetCurrentGameTime() - packet.get_gametime());
	if (GetCurrentTimeScale() != nexttimescale)
		SetTimeScale(nexttimescale);

	if (SetWeather)
	{
		tsStart();
		Weather::ForceActive((TESWeather*)Game::GetFormById(packet.get_weather()), TRUE);
		tsEnd();
	}

	if ((packet.get_flags() & GamePKTMenu) > 0 && !paused)
	{
		Debug::ToggleAI();
		Game::DisablePlayerControls(true, true, false, true, true, false, true, false, 0);
		cqPushCommand("SetGlobalTimeMultiplier 0.000001", NULL);
		PrintNote("Waiting for other player.");
		paused = true;
	}
	else if ((packet.get_flags() & GamePKTMenu) == 0 && paused)
	{
		Debug::ToggleAI();
		Game::EnablePlayerControls(true, true, true, true, true, true, true, true, 0);
		cqPushCommand("SetGlobalTimeMultiplier 1.0", NULL);
		paused = false;
	}

	actorbase_pkt *actor_base = NULL;
	actor_pkt	  *actor = NULL;
	reflisthdl	  *ref = NULL;
	TESObjectREFR *temp = NULL;
	uint id = 0;

	for (uint i = 0; i < packet.get_num_actorbases(); i++)
	{
		actor_base = packet.get_actorbases(i);
		while (WaitForSingleObject(reflist[i].mutex, INFINITE) == WAIT_TIMEOUT);
		SetActorBasePkt(actor_base, &reflist[actor_base->actor_id]);
		ReleaseMutex(reflist[i].mutex);

		WaitForSingleObject(countMutex, INFINITE);
		num_actors++;
		ReleaseMutex(countMutex);
	}

	for (uint i = 0; i < packet.get_num_actors(); i++)
	{
		actor = packet.get_actors(i);
		while (WaitForSingleObject(reflist[i].mutex, INFINITE) == WAIT_TIMEOUT);
		SetActorPkt(actor, &reflist[actor->actor_id]);
		ReleaseMutex(reflist[i].mutex);
	}

	return true;
}