#include "common/types.h"

#ifndef packets_h
#define packets_h

#define ActorPKTDead		0x00000001
#define ActorPKTSleep		0x00000002
#define ActorPKTSit			0x00000004
#define ActorPKTSneak		0x00000008
#define ActorPKTRun			0x00000010
#define ActorPKTSprint		0x00000020
#define ActorPKTJump		0x00000040
#define ActorPKTDraw		0x00000080
#define ActorPKTLeftAtk		0x00000100
#define ActorPKTRightAtk	0x00000200
#define ActorPKTBleadout	0x00000400
#define ActorPKTFly			0x00000800
#define ActorPKTGhost		0x00001000
#define ActorPKTSwim		0x00002000
#define ActorPKTLeftPwrAtk	0x00004000
#define ActorPKTRightPwrAtk 0x00008000
#define ActorPKTBlocking	0x00010000
#define ActorPktStagger		0x00020000
#define ActorPktRecoil		0x00040000

struct reflisthdl;

struct item_pkt
{
	item_pkt();
	~item_pkt();

	uint form;
	uint count;
	unsigned char equip;
};

struct actorbase_pkt
{
	actorbase_pkt();
	~actorbase_pkt();

	uint actor_id;
	uint form;
	// Attributes
	float health;
	float magicka;
	float stamina;

	// Skills
	float onehanded;
	float twohanded;
	float marksman;
	float block;
	float smithing;
	float heavyarmor;
	float lightarmor;
	float pickpocket;
	float lockpicking;
	float sneak;
	float alchemy;
	float speechcraft;
	float alteration;
	float conjuration;
	float destruction;
	float illusion;
	float restoration;
	float enchanting;

	// AI Data
	float aggression;
	float confidence;
	float energy;
	float morality;
	float mood;
	float assistance;

	// Other Statistics
	float healthrate;
	float magickarate;
	float staminarate;
	float attackdamagemult;
	float speedmult;
	float weaponspeedmult;
	float inventoryweight;
	float carryweight;
	float critchance;
	float meleedamage;
	float unarmeddamage;
	float mass;
	float voicepoints;
	float voicerate;
	float damageresist;
	float poisonresist;
	float fireresist;
	float electricresist;
	float frostresist;
	float magicresist;
	float normalweaponsresist;
	float paralysis;
	float invisibility;
	float nighteye;
	float detectliferange;
	float waterbreathing;
	float waterwalking;
	float jumpingbonus;
	float wardpower;
	float warddeflection;
	float equippeditemcharge;
	float equippedstaffcharge;
	float armorperks;
	float shieldoerks;
	float bowspeedbonus;

	uint num_items;
	item_pkt items[1000];

	uint size();
};

void DuplicateItems(reflisthdl *ref);
void RemoveItems(reflisthdl *ref);
void GetActorBasePkt(actorbase_pkt *pkt, reflisthdl *ref);
void SetActorBasePkt(actorbase_pkt *pkt, reflisthdl *ref);

struct actor_pkt
{
	actor_pkt();
	~actor_pkt();

	uint  actor_id;
	float position_x;
	float position_y;
	float position_z;
	float angle_x;
	float angle_y;
	float angle_z;

	float health;
	float magicka;
	float stamina;
	float healthrate;
	float magickarate;
	float staminarate;
	float carryweight;

	uint state;
	unsigned char move;
	unsigned char turn;
};

void GetActorPkt(actor_pkt *pkt, reflisthdl *ref);
void SetActorPkt(actor_pkt *pkt, reflisthdl *ref);

#define GamePKTSize	20000
#define GamePKTMenu 0x01

struct game_pkt
{
	game_pkt();
	~game_pkt();

	unsigned char data[GamePKTSize];

	float get_gametime();
	void set_gametime(float t);
	uint get_weather();
	void set_weather(uint w);
	uint get_flags();
	void set_flags(uint f);

	uint get_num_actors();
	void set_num_actors(uint n);		// You MUST set num_actors BEFORE num_actorbases AND num_itembases
	actor_pkt *get_actors(uint i);

	uint get_num_actorbases();
	void set_num_actorbases(uint n);	// You MUST set num_actorbases BEFORE num_itembases
	actorbase_pkt *get_actorbases(uint i);

	uint size();
};

#endif