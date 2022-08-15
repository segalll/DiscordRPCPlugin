#include "pch.h"
#include "DiscordRPCPlugin.h"

#include "discord_rpc.h"
#include "util.h"


BAKKESMOD_PLUGIN(DiscordRPCPlugin, "Discord Rich Presence integration", plugin_version, PLUGINTYPE_FREEPLAY)

void initDiscord() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize(discord_application_id, &handlers, 0, nullptr);
}

void DiscordRPCPlugin::onEnableChange() {
    if (enabled.get()) {
        initDiscord();
    } else {
        Discord_Shutdown();
    }
}

void DiscordRPCPlugin::setMatchTime(ServerWrapper wrapper) {
    if (wrapper.GetbOverTime()) {
        pd.remaining = 0;
        if (!pd.overtime) {
            pd.overtime = true;
            pd.start = time(0);
        }
        pd.state += " (OT)";
    } else if (!wrapper.GetbUnlimitedTime()) {
        pd.remaining = wrapper.GetSecondsRemaining() + 1;
    }
}

void DiscordRPCPlugin::setMap() {
    std::string map = getMap(gameWrapper->GetCurrentMap());
    if (map == "logo") {
        pd.image = "logo";
        pd.hover = "";
    } else {
        pd.image = mapToImageKey(map);
        pd.hover = map;
    }
}

void DiscordRPCPlugin::handleGame(ActivityData activityData) {
    auto playlistWrapper = activityData.wrapper.GetPlaylist();

    std::string gameType;
    if (playlistWrapper) {
        gameType = getPlaylistName(playlistWrapper.GetPlaylistId(), activityData.wrapper.GetMaxTeamSize());
    }

    if (gameType == "Training") {
        handlePsyonixTraining(activityData);
        return;
    }

    int teamGoals[2] = { 0, 0 };
    if (!activityData.wrapper.GetTeams().IsNull() && activityData.wrapper.GetTeams().Get(0) && activityData.wrapper.GetTeams().Get(1)) {
        teamGoals[0] = activityData.wrapper.GetTeams().Get(0).GetScore();
        teamGoals[1] = activityData.wrapper.GetTeams().Get(1).GetScore();
    }

    auto localPlayer = gameWrapper->GetLocalCar();
    if (localPlayer && localPlayer.GetPRI()) {
        pd.team = localPlayer.GetPRI().GetTeamNum();
    }
    
    if (gameType.find("Ranked") != std::string::npos && localPlayer && playlistWrapper) {
        auto uidWrapper = gameWrapper->GetUniqueID();
        int playlistId = playlistWrapper.GetPlaylistId();

        auto mmrWrapper = gameWrapper->GetMMRWrapper();
        auto rank = mmrWrapper.GetPlayerRank(uidWrapper, playlistId);
        float mmr = mmrWrapper.GetPlayerMMR(uidWrapper, playlistId);

        std::string rankStr = getRank(rank.Tier);
        if (rankStr.size() > 0) {
            pd.smallImage = rankToImageKey(rankStr);
            pd.smallHover = rankStr + " (MMR: " + std::to_string((int)mmr) + ")";
        }
    }

    int plyGoals = 0, oppGoals = 0;
    if (pd.team == 0) {
        plyGoals = teamGoals[0];
        oppGoals = teamGoals[1];
    } else if (pd.team == 1) {
        plyGoals = teamGoals[1];
        oppGoals = teamGoals[0];
    }

    if (activityData.wrapper.GetbMatchEnded()) {
        pd.details = "In " + gameType + " lobby";
        pd.start = 0;
        pd.remaining = 0;
        
        if (plyGoals > oppGoals) {
            pd.state = "Won ";
        } else {
            pd.state = "Lost ";
        }
        pd.state += "(" + std::to_string(plyGoals) + " - " + std::to_string(oppGoals) + ")";
    } else {
        pd.details = "Playing " + gameType;
        if (plyGoals > oppGoals) {
            pd.state = "Winning ";
        } else if (plyGoals < oppGoals) {
            pd.state = "Losing ";
        } else {
            pd.state = "Tied ";
        }
        pd.state += std::to_string(plyGoals) + " - " + std::to_string(oppGoals);

        setMatchTime(activityData.wrapper);
    }

    setMap();
}

void DiscordRPCPlugin::handleReplay(ActivityData activityData) {
    auto playlistWrapper = activityData.wrapper.GetPlaylist();

    std::string gameType;
    if (playlistWrapper) {
        gameType = getPlaylistName(playlistWrapper.GetPlaylistId(), activityData.wrapper.GetMaxTeamSize());
    }

    int teamGoals[2] = { 0, 0 };
    if (!activityData.wrapper.GetTeams().IsNull() && activityData.wrapper.GetTeams().Get(0) && activityData.wrapper.GetTeams().Get(1)) {
        teamGoals[0] = activityData.wrapper.GetTeams().Get(0).GetScore();
        teamGoals[1] = activityData.wrapper.GetTeams().Get(1).GetScore();
    }
    pd.state = "(Blue) " + std::to_string(teamGoals[0]) + " - " + std::to_string(teamGoals[1]) + " (Red)";

    pd.details = "Watching " + gameType;

    setMatchTime(activityData.wrapper);

    setMap();
}

void DiscordRPCPlugin::handleFreeplay(ActivityData activityData) {
    pd.details = "In freeplay";
    
    pd.state = "";
    pd.smallImage = "";
    pd.smallHover = "";

    setMap();

    auto playlistWrapper = activityData.wrapper.GetPlaylist();
    if (playlistWrapper && playlistWrapper.GetPlaylistId() == 19) {
        pd.details = "In workshop map";
        pd.hover = gameWrapper->GetCurrentMap();
    }
}

void DiscordRPCPlugin::handleCustomTraining(ActivityData activityData) {
    pd.details = "In custom training";

    setMap();

    TrainingEditorWrapper tew = activityData.wrapper.memory_address;

    if (!tew) return;

    if (tew.GetTrainingFileName().ToString() != "") {
        if (!tew.GetTrainingData() || !tew.GetTrainingData().GetTrainingData()) return;
        auto tesdw = tew.GetTrainingData().GetTrainingData();

        int currentShot = tew.GetActiveRoundNumber() + 1;

        std::string currentName = capitalize(tesdw.GetTM_Name().ToString());

        pd.state = currentName + " (Shot " + std::to_string(currentShot) + "/" + std::to_string(tew.GetTotalRounds()) + ")";
    }
}

void DiscordRPCPlugin::handleSpectate(ActivityData activityData) {
    auto playlistWrapper = activityData.wrapper.GetPlaylist();

    std::string gameType;
    if (playlistWrapper) {
        gameType = getPlaylistName(playlistWrapper.GetPlaylistId(), activityData.wrapper.GetMaxTeamSize());
    }

    int teamGoals[2] = { 0, 0 };
    if (!activityData.wrapper.GetTeams().IsNull() && activityData.wrapper.GetTeams().Get(0) && activityData.wrapper.GetTeams().Get(1)) {
        teamGoals[0] = activityData.wrapper.GetTeams().Get(0).GetScore();
        teamGoals[1] = activityData.wrapper.GetTeams().Get(1).GetScore();
    }
    pd.state = "(Blue) " + std::to_string(teamGoals[0]) + " - " + std::to_string(teamGoals[1]) + " (Red)";

    pd.details = "Watching " + gameType;

    setMatchTime(activityData.wrapper);

    setMap();
}

void DiscordRPCPlugin::handlePsyonixTraining(ActivityData activityData) {
    pd.details = "In training";

    pd.state = "";
    pd.smallImage = "";
    pd.smallHover = "";

    setMap();
}

ActivityData DiscordRPCPlugin::getCurrentActivityData() {
    if (gameWrapper->IsInReplay()) { // must come before online game
        return { ActivityType::Replay, gameWrapper->GetGameEventAsReplay() };
    } else if (gameWrapper->IsInOnlineGame()) {
        auto pc = gameWrapper->GetPlayerController();
        if (pc && pc.GetPRI() && pc.GetPRI().IsSpectator()) {
            return { ActivityType::Spectate, gameWrapper->GetOnlineGame() };
        }
        return { ActivityType::Online, gameWrapper->GetOnlineGame() };
    } else if (gameWrapper->IsInFreeplay()) {
        return { ActivityType::Freeplay, gameWrapper->GetGameEventAsServer() };
    } else if (gameWrapper->IsInCustomTraining()) {
        return { ActivityType::Training, gameWrapper->GetGameEventAsServer() };
    } else if (gameWrapper->IsInGame()) {
        return { ActivityType::Local, gameWrapper->GetGameEventAsServer() };
    } else {
        return { ActivityType::Menu, 0 };
    }
}

void DiscordRPCPlugin::checkActivity() {
    if (!*enabled) return;

    ActivityData activityData = getCurrentActivityData();

    if (pd.type != activityData.type) { // activity has changed
        pd.type = activityData.type;
        pd.start = time(0); // current time
        pd.remaining = 0;
    }

    if (activityData.type == ActivityType::Menu) {
        pd.details = "On main menu";
        pd.state = "";
        pd.image = "logo";
        pd.hover = "";
        pd.smallImage = "";
        pd.smallHover = "";
    } else if (activityData.wrapper) {
        if (*activityCvars[(int)activityData.type]) { // current activity cvar is enabled
            activityHandlers[(int)activityData.type](activityData);
        } else {
            gameWrapper->SetTimeout(std::bind(&DiscordRPCPlugin::checkActivity, this), 1.0f);
            return;
        }
    }

    updateRPC();
}

void DiscordRPCPlugin::updateRPC() {
    DiscordRichPresence rp;
    memset(&rp, 0, sizeof(rp));

    rp.state = pd.state.c_str();
    rp.details = pd.details.c_str();
    rp.startTimestamp = pd.start;
    if (pd.remaining != 0) {
        rp.endTimestamp = time(0) + pd.remaining;
    }
    rp.largeImageKey = pd.image.c_str();
    rp.largeImageText = pd.hover.c_str();
    rp.smallImageKey = pd.smallImage.c_str();
    rp.smallImageText = pd.smallHover.c_str();

    Discord_UpdatePresence(&rp);

    gameWrapper->SetTimeout(std::bind(&DiscordRPCPlugin::checkActivity, this), 1.0f);
}

void DiscordRPCPlugin::onLoad() {
    auto enabledCvar = cvarManager->registerCvar("drpc_enabled", "1", "Enable Discord RPC");
    enabled = std::make_shared<bool>(false);
    enabledCvar.bindTo(enabled);
    enabledCvar.addOnValueChanged(std::bind(&DiscordRPCPlugin::onEnableChange, this));

    auto onlineCvar = cvarManager->registerCvar("drpc_online", "1", "Show online match activity");
    activityCvars[0] = std::make_shared<bool>(false);
    onlineCvar.bindTo(activityCvars[0]);

    auto replaysCvar = cvarManager->registerCvar("drpc_replay", "1", "Show replay viewing activity");
    activityCvars[1] = std::make_shared<bool>(false);
    replaysCvar.bindTo(activityCvars[1]);

    auto freeplayCvar = cvarManager->registerCvar("drpc_freeplay", "1", "Show freeplay activity");
    activityCvars[2] = std::make_shared<bool>(false);
    freeplayCvar.bindTo(activityCvars[2]);

    auto trainingCvar = cvarManager->registerCvar("drpc_training", "1", "Show custom training activity");
    activityCvars[3] = std::make_shared<bool>(false);
    trainingCvar.bindTo(activityCvars[3]);

    auto spectateCvar = cvarManager->registerCvar("drpc_spectate", "1", "Show spectate activity");
    activityCvars[4] = std::make_shared<bool>(false);
    spectateCvar.bindTo(activityCvars[4]);

    auto localCvar = cvarManager->registerCvar("drpc_local", "1", "Show local match activity");
    activityCvars[5] = std::make_shared<bool>(false);
    localCvar.bindTo(activityCvars[5]);

    initDiscord();

    checkActivity();
}

void DiscordRPCPlugin::onUnload() {
    Discord_Shutdown();
}