#include "stdafx.h"

#include "playback_listener.h"
#include "track_data.h"
#include "Discord.h"

bool init = false;

void playback_listener::on_playback_new_track(metadb_handle_ptr p_track) {
	try {
		// create new track data from metadb handle
		track_data data(p_track);

		if (init == false) {
			discordInit();
			init = true;
		}
		
		UpdatePresence(data.get_title(), data.get_artist(), data.get_album());
	}
	catch (pfc::exception e) {
		popup_message::g_show("Caught exception", "Error");
	}
}

void playback_listener::on_playback_starting(play_control::t_track_command p_command, bool p_paused) {

}

void playback_listener::on_playback_stop(play_control::t_stop_reason p_reason) {
	if (p_reason != play_control::t_stop_reason::stop_reason_starting_another) {
		UpdatePresenceState("stopped", "Stopped");
	}
}

void playback_listener::on_playback_pause(bool p_state) {
	if (p_state) {
		UpdatePresenceState("paused", "Paused");
	}
	else {
		UpdatePresenceState("playing", "Playing");
	}
}