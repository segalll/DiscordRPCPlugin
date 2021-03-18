#pragma once

#pragma comment(lib, "pluginsdk.lib")
#pragma comment(lib, "discord-rpc.lib")

#include "bakkesmod/plugin/bakkesmodplugin.h"

struct GameState {
    int type;
    ServerWrapper wrapper;
};

struct Data {
    int type;
    int64_t start;
    int64_t remaining;
    bool overtime;
    int team;
    std::string details;
    std::string state;
    std::string image;
    std::string hover;
    std::string smallImage;
    std::string smallHover;
};

class DiscordRPCPlugin : public BakkesMod::Plugin::BakkesModPlugin {
   public:
    virtual void onLoad();
    virtual void onUnload();
    void OnEnableChange(std::string eventName);
    void CheckGameType(Data input);
    void updateRPC(Data input);
    GameState GetCurrentGameType();
    Data HandleGame(Data input, GameState gameState);
    Data HandleReplay(Data input, GameState gameState);
    Data HandleFreeplay(Data input, GameState gameState);
    Data HandleTraining(Data input, GameState gameState);
    Data HandleSpectate(Data input, GameState gameState);
};