#pragma once
#include "checksum_crc.h"
#include "vector3.h"
#include <memory>

#define in_attack  (1 << 0)
#define in_jump   (1 << 1)
#define in_duck   (1 << 2)
#define in_forward  (1 << 3)
#define in_back   (1 << 4)
#define in_use   (1 << 5)
#define in_cancel  (1 << 6)
#define in_left   (1 << 7)
#define in_right  (1 << 8)
#define in_moveleft  (1 << 9)
#define in_moveright (1 << 10)
#define in_attack2  (1 << 11)
#define in_run   (1 << 12)
#define in_reload  (1 << 13)
#define in_alt1   (1 << 14)
#define in_alt2   (1 << 15)
#define in_score  (1 << 16)   // used by client.dll for when scoreboard is held down
#define in_speed  (1 << 17) // player is holding the speed key
#define in_walk   (1 << 18) // player holding walk key
#define in_zoom   (1 << 19) // zoom key for hud zoom
#define in_weapon1  (1 << 20) // weapon defines these bits
#define in_weapon2  (1 << 21) // weapon defines these bits
#define in_bullrush  (1 << 22)
#define in_grenade1  (1 << 23) // grenade 1
#define in_grenade2  (1 << 24) // grenade 2
#define in_lookspin  (1 << 25)

class c_usercmd
{
public:
	c_usercmd()
	{
		memset(this, NULL, sizeof(*this)); //-V598
	};

	virtual ~c_usercmd()
	{

	};

	CRC32_t get_checksum(void) const
	{
		CRC32_t crc;
		CRC32_Init(&crc);

		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tickcount, sizeof(tickcount));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

		CRC32_Final(&crc);

		return crc;
	}

	int     command_number;	 // 0x04 For matching server and client commands for debugging
	int     tickcount;		 // 0x08 the tick the client created this command
	vec3	viewangles;		 // 0x0C Player instantaneous view angles.
	vec3	aimdirection;		 // 0x18
	float   forwardmove;		  // 0x24
	float   sidemove;			  // 0x28
	float   upmove;			 // 0x2C
	int     buttons;			 // 0x30 Attack button states
	char    impulse;			// 0x34
	int     weaponselect;		// 0x38 Current weapon id
	int     weaponsubtype;	// 0x3C
	int     random_seed;     // 0x40 For shared random functions
	short   mousedx;			// 0x44 mouse accum in x from create move
	short   mousedy;			 // 0x46 mouse accum in y from create move
	bool    predicted;		// 0x48 Client only, tracks whether we've predicted this command at least once
	char    pad_0x4C[0x18];		// 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

class c_verified_cmd
{
public:
	c_usercmd m_cmd;
	CRC32_t  m_crc;
};
