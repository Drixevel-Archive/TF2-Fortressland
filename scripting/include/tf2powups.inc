#if defined _tf2powups_included
  #endinput
#endif
#define _tf2powups_included

enum eRuneTypes
{
	Rune_Invalid = -1,
	Rune_Strength,
	Rune_Haste,
	Rune_Regen,
	Rune_Resist,
	Rune_Vampire,
	Rune_Reflect,
	Rune_Precision,
	Rune_Agility,
	Rune_Plague,
	Rune_King,
	Rune_Knockout,
	Rune_Supernova,

	Rune_LENGTH
}

#define RuneTypes eRuneTypes
#define RuneTypes_t eRuneTypes 	// Cuz

stock static TFCond g_RuneConds[view_as< int >(Rune_LENGTH)] = {	// Dammit dvander
	TFCond_RuneStrength,
	TFCond_RuneHaste,
	TFCond_RuneRegen,
	TFCond_RuneResist,
	TFCond_RuneVampire,
	TFCond_RuneWarlock,
	TFCond_RunePrecision,
	TFCond_RuneAgility,
	TFCond_PlagueRune,
	TFCond_KingRune,
	TFCond_RuneKnockout,
	TFCond_SupernovaRune
};

/**
 *	Create a rune
 *
 *	@param pos 			Location of rune (z axis goes up +48 hu on spawn)
 *	@param type 		RuneTypes type of rune to spawn.
 *	@param teammaybe 	Team number of rune, -2 for no team.
 *	@param thrown 		'Throws' the rune, similar to dropping it as a player
 *	@param thrownz 		'Throws' the rune vertically
 *	@param angle 		Angle of rune if 'thrown' is true
 *
 *	@return 			Created powerup entity
**/
native int TF2_CreateRune(float pos[3], RuneTypes type, int teammaybe = -2, bool thrown = false, bool thrownz = false, float angle[3] = {0.0, 0.0, 0.0});

/**
 *	Force a client to drop a rune that they are carrying
 *
 *	@param client 		Client to force to drop their rune
 *	@param thrown 		Same as 'thrown' in TF2_CreateRune
 *	@param teammaybe 	Same as 'teammaybe' in TF2_CreateRune
 *
 *	@return 			Rune that was dropped, -1 if there wasn't one
**/
native int TF2_DropRune(int client, bool thrown, int teammaybe);

/**
 *	When a rune is created
 *
 *	@param pos 			Location of rune (z axis goes up +48 hu after this is called)
 *	@param type 		RuneTypes type of rune to spawn.
 *	@param teammaybe 	Team number of rune, -2 for no team.
 *	@param thrown 		'Throws' the rune, similar to dropping it as a player
 						Only appears to work when actually thrown by a player
 *	@param thrownz 		'Throws' the rune vertically
 *	@param angle 		Angle of rune if 'thrown' is true
 *						Doesn't seem to work when being set
**/
forward Action TF2_OnRuneSpawn(float pos[3], RuneTypes &type, int &teammaybe, bool &thrown, bool &thrownz, float angle[3]);

/**
 *	When a rune is created
 *
 *	@param pos 			Location of rune
 *	@param type 		RuneTypes type of rune to spawn.
 *	@param teammaybe 	Team number of rune, -2 for no team.
 *	@param thrown 		'Throws' the rune, similar to dropping it as a player
 *	@param thrownz 		'Throws' the rune vertically
 *	@param angle 		Angle of rune if 'thrown' is true
**/
forward void TF2_OnRuneSpawnPost(int rune, float pos[3], RuneTypes type, int teammaybe, bool thrown, bool thrownz, float ang[3]);

/**
 *	If a player can pick up this rune
 *
 *	@param rune 		Rune to be touched
 *	@param client 		Client touching this rune
 *	@param status 		True if client can touch, false otherwise
**/
forward Action TF2_CanRuneBeTouched(int rune, int client, bool &status);

/**
 *	When a player picks up a rune
 *
 *	@param client 		Client picking up the rune
 *	@param rune 		Rune ent idx
 *	@param type 		RuneTypes type of the rune
**/
forward void TF2_OnRunePickup(int client, int rune, RuneTypes type);

/**
 *	Get the type of a rune
 *
 *	@param rune 		Rune entity
 *
 *	@return 			RuneTypes type of this rune
 *	@error 				Invalid rune entity passed, entity passed was not a rune
**/
// 1288 linux
// 1268 windows
stock RuneTypes TF2_GetRuneType(int rune)
{
	if (!IsValidEntity(rune))
	{
		ThrowError("Entity %d is invalid!", rune);
		return Rune_Invalid;
	}

	char cls[32]; GetEntityClassname(rune, cls, sizeof(cls));

	if (strncmp(cls, "item_power", 10, false))
	{
		ThrowError("Entity %d (%s) is not a powerup rune!", rune, cls);
		return Rune_Invalid;
	}
	return view_as< RuneTypes >(GetEntData(rune, FindDataMapInfo(rune, "m_iszModel") + 24));
}

/**
 *	Get the carried rune type of a player
 *
 *	@param client 		Client index
 *
 *	@return 			RuneTypes type of carried rune, Rune_Invalid on failure
 *	@error 				Invalid client or client not in-game
**/
// This is literally CTFPlayerShared::GetCarryingRuneType
stock RuneTypes TF2_GetCarryingRuneType(int client)
{
	if (!(0 < client <= MaxClients))
	{
		ThrowError("Client %d is not valid!", client);
		return Rune_Invalid;
	}

	if (!IsClientInGame(client))
	{
		ThrowError("Client %d is not in-game!", client);
		return Rune_Invalid;
	}

	if (!TF2_IsPlayerInCondition(client, TFCond_HasRune))
		return Rune_Invalid;

	int count;

	for (;;)
	{
		if (TF2_IsPlayerInCondition(client, g_RuneConds[count]))
			return view_as< RuneTypes >(count);

		if (++count == Rune_LENGTH)
			return Rune_Invalid;
	}
	return count;
}

public SharedPlugin __pl_tf2powups = {
	name = "tf2powups",
	file = "tf2powups.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public void __pl_tf2powups_SetNTVOptional()
{
	MarkNativeAsOptional("TF2_CreateRune");
	MarkNativeAsOptional("TF2_DropRune");
}
#endif