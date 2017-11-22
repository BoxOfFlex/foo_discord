#include <iostream>
#include <ctime>
#include <cstdlib>
#include "stdafx.h"
#include "util.h"
#define _CRT_SECURE_NO_WARNING

const char* APPLICATION_ID = "382709212939026433";
DiscordRichPresence discordPresence;
bool initialized = false;

char oldState[256];

void handleDiscordReady() {}

void handleDiscordDisconnected(int errcode, const char* message) {}

void handleDiscordError(int errcode, const char* message) {}

void handleDiscordJoin(const char* secret) {
	std::cout << "Somone wants to join your game" << std::endl;

}

void handleDiscordSpectate(const char* secret) {
	std::cout << "Somone wants to Spectate you" << std::endl;
}

void handleDiscordJoinRequest(const DiscordJoinRequest* request) {}

void discordInit()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = handleDiscordReady;
	handlers.disconnected = handleDiscordDisconnected;
	handlers.errored = handleDiscordError;
	handlers.joinGame = handleDiscordJoin;
	handlers.spectateGame = handleDiscordSpectate;
	handlers.joinRequest = handleDiscordJoinRequest;
	Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

void initPresence() {
	if (initialized) return;
	memset(&discordPresence, 0, sizeof(discordPresence));
	initialized = true;
}

void UpdatePresence(wchar_t *songname, wchar_t *artist, wchar_t *album)
{
	initPresence();

	char songinfo[256];
	char albuminfo[256];
	char albumname[256];

	size_t chars;
	wcstombs_s(&chars, albumname, 256, album, 255);

	sprintf_s(songinfo, 256, "%s - %s", util::wide_to_utf8(artist), util::wide_to_utf8(songname));
	sprintf_s(albuminfo, 256, "Album: %s", albumname);

	//char track_seconds[256];
	//wcstombs_s(&chars, track_seconds, 256, tracktime, 255);

	//long track_seconds_int = strtol(track_seconds, NULL, 10);

	//time_t now = time(NULL);
	//time_t later = time(NULL) + track_seconds_int;

	//discordPresence.startTimestamp = now;
	//discordPresence.endTimestamp = later;

	discordPresence.largeImageKey = "logo";
	discordPresence.largeImageText = albuminfo;
	discordPresence.state = songinfo;
	sprintf_s(oldState, 256, songinfo);
	discordPresence.instance = 1;
	discordPresence.smallImageKey = "playing";
	discordPresence.smallImageText = "Playing";
	Discord_UpdatePresence(&discordPresence);
}

void UpdatePresenceState(char *state, char *stext)
{
	initPresence();

	discordPresence.largeImageKey = "logo";
	if(state == "stopped"){
		discordPresence.state = state;
		discordPresence.largeImageText = "foobar2000";
	}
	else if (state == "paused") {
		char newState[256];
		sprintf_s(newState, 256, "%s [%s]", oldState, stext);
		discordPresence.state = newState;
	}
	else {
		discordPresence.state = oldState;
	}
	
	discordPresence.smallImageKey = state;
	discordPresence.smallImageText = stext;
	Discord_UpdatePresence(&discordPresence);
}