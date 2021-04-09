#include "event_manager.h"
#include "interfaces.h"
#include "globals.h"
#include "sound.h"
#include "math.h"
#include "studio.h"
#include "visuals.h"
#pragma comment(lib,"Winmm.lib")

std::vector<IGameEventListener2*> event_manager_list;

#define REG_EVENT_LISTENER(p, e, n, s) p = new CGameEventListener(n, e, s)

c_event_override::c_event_override() {

}

c_event_override* event_override = new c_event_override();

color get_random_color(int idx) {
	switch (idx) {
	case 0: return color::White; break;
	case 1: return color::Red; break;
	case 2: return color::Yellow; break;
	case 3: return color::Green; break;
	case 4: return color::Blue; break;
	case 5: return color::Pink; break;
	case 6: return color::LightBlue; break;
	}
}

void player_hurt(IGameEvent* game_event) {
	int attackerid = game_event->GetInt("attacker");
	int userid = game_event->GetInt("userid");
	int entityid = interfaces->engine->get_player_for_user_id(attackerid);
	if (entityid == interfaces->engine->get_local_player()) {
		c_entity* target = interfaces->entity_list->get_client_entity(interfaces->engine->get_player_for_user_id(userid));
		PlaySoundA(hitsound, NULL, SND_ASYNC | SND_MEMORY);
	}
}

void bullet_impact(IGameEvent* game_event) {
}

void weapon_fire(IGameEvent* game_event) {
}

void player_death(IGameEvent* game_event) {
}

void round_end(IGameEvent* game_event) {
}

void round_prestart(IGameEvent* game_event) {
}

void round_start(IGameEvent* game_event) {
	
}

CGameEventListener* _player_hurt;
CGameEventListener* _bullet_impacts;
CGameEventListener* _weapon_fire;
CGameEventListener* _player_death;
CGameEventListener* _round_end;
CGameEventListener* _round_prestart;
CGameEventListener* _round_start;

void c_event_override::register_self()
{
	REG_EVENT_LISTENER(_player_hurt, &player_hurt, "player_hurt", false);
	REG_EVENT_LISTENER(_bullet_impacts, &bullet_impact, "bullet_impact", false);
	REG_EVENT_LISTENER(_weapon_fire, &weapon_fire, "weapon_fire", false);
	REG_EVENT_LISTENER(_player_death, &player_death, "player_death", false);
	REG_EVENT_LISTENER(_round_end, &round_end, "round_end", false);
	REG_EVENT_LISTENER(_round_prestart, &round_prestart, "round_prestart", false);
	REG_EVENT_LISTENER(_round_start, &round_start, "round_start", false);
}
