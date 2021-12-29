#include "sledge/misc/steam.h"
#include "util/log.h"

#include <steam/steam_api.h>

bool Steam::Init() {
	if (!SteamAPI_Init()) {
		LogError("SteamAPI_Init failed");
		return false;
	}

	LogInfo("SteamAPI initialized");

	bool bSubscribed = SteamApps()->BIsSubscribed();
	bool bSubscribedFS = SteamApps()->BIsSubscribedFromFamilySharing();
	bool bSubscribedFW = SteamApps()->BIsSubscribedFromFreeWeekend();

	if (bSubscribed || bSubscribedFS || bSubscribedFW)
		return true;

	return false;
}