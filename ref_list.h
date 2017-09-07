#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#ifndef ref_list_h
#define ref_list_h

struct reflisthdl
{
	reflisthdl();
	~reflisthdl();

	uint actor_id;

	TESObjectREFR	*ref;
	TESObjectREFR	*lookat;
	void			*mutex;
	uint			form;
	uint			state;
	float			health;
	float			magicka;
	float			stamina;

	float			base_health;
	float			base_magicka;
	float			base_stamina;

	float			x_offset;
	float			y_offset;
	float			z_offset;
	unsigned char	flags;
};

void rlInit();
void rlRelease();
bool rlUpdate();
bool rlSend(SOCKET connection);
bool rlReceive(SOCKET connection, bool SetWeather);

#endif