#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

#include <array>

constexpr auto discord_application_id = "562098317547405322";

enum class ActivityType {
    Online,
    Replay,
    Freeplay,
    Training,
    Spectate,
    Local,
    Menu
};

struct ActivityData {
    ActivityType type;
    ServerWrapper wrapper;
};

struct PresenceData {
    ActivityType type;
    int64_t start, remaining;
    bool overtime;
    int team;
    std::string details, state, image, hover, smallImage, smallHover;
};

class DiscordRPCPlugin: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow {

    std::shared_ptr<bool> enabled;

    PresenceData pd;

    std::array<std::shared_ptr<bool>, 6> activityCvars;
    std::array<std::function<void(ActivityData)>, 6> activityHandlers = {
        std::bind(&DiscordRPCPlugin::handleGame, this, std::placeholders::_1),
        std::bind(&DiscordRPCPlugin::handleReplay, this, std::placeholders::_1),
        std::bind(&DiscordRPCPlugin::handleFreeplay, this, std::placeholders::_1),
        std::bind(&DiscordRPCPlugin::handleCustomTraining, this, std::placeholders::_1),
        std::bind(&DiscordRPCPlugin::handleSpectate, this, std::placeholders::_1),
        std::bind(&DiscordRPCPlugin::handleGame, this, std::placeholders::_1)
    };

    virtual void onLoad();
    virtual void onUnload();

    void RenderSettings() override;
    std::string GetPluginName() override;
    void SetImGuiContext(uintptr_t ctx) override;

    bool isWindowOpen_ = false;
    std::string menuTitle_ = "DiscordRPCPlugin setup guide";

    virtual void Render() override;
    virtual std::string GetMenuName() override;
    virtual std::string GetMenuTitle() override;
    virtual bool ShouldBlockInput() override;
    virtual bool IsActiveOverlay() override;
    virtual void OnOpen() override;
    virtual void OnClose() override;

    void onEnableChange();

    void setMatchTime(ServerWrapper wrapper);
    void setMap();

    void handleGame(ActivityData activityData);
    void handleReplay(ActivityData activityData);
    void handleFreeplay(ActivityData activityData);
    void handleCustomTraining(ActivityData activityData);
    void handleSpectate(ActivityData activityData);
    void handlePsyonixTraining(ActivityData activityData);

    ActivityData getCurrentActivityData();
    void checkActivity();
    void updateRPC();
};

