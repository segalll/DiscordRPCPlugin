#include "pch.h"
#include "DiscordRPCPlugin.h"

#include "Windows.h"
#include "shellapi.h"

// plugin settings
std::string DiscordRPCPlugin::GetPluginName() {
    return "DiscordRPCPlugin";
}

void DiscordRPCPlugin::SetImGuiContext(uintptr_t ctx) {
    ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

void DiscordRPCPlugin::RenderSettings() {
    if (ImGui::Button("INITIAL SETUP")) {
        gameWrapper->Execute([this](GameWrapper* gw) { cvarManager->executeCommand("openmenu drpc_setup"); });
    }

    CVarWrapper enableCvar = cvarManager->getCvar("drpc_enabled");
    if (!enableCvar) return;
    bool enabled = enableCvar.getBoolValue();
    if (ImGui::Checkbox("Enable Discord RPC", &enabled)) {
        enableCvar.setValue(enabled);
    }

    CVarWrapper onlineCvar = cvarManager->getCvar("drpc_online");
    if (!onlineCvar) return;
    bool online = onlineCvar.getBoolValue();
    if (ImGui::Checkbox("Show online match activity", &online)) {
        onlineCvar.setValue(online);
    }

    CVarWrapper replayCvar = cvarManager->getCvar("drpc_replay");
    if (!replayCvar) return;
    bool replay = replayCvar.getBoolValue();
    if (ImGui::Checkbox("Show replay viewing activity", &replay)) {
        replayCvar.setValue(replay);
    }

    CVarWrapper freeplayCvar = cvarManager->getCvar("drpc_freeplay");
    if (!freeplayCvar) return;
    bool freeplay = freeplayCvar.getBoolValue();
    if (ImGui::Checkbox("Show freeplay activity", &freeplay)) {
        freeplayCvar.setValue(freeplay);
    }

    CVarWrapper trainingCvar = cvarManager->getCvar("drpc_training");
    if (!trainingCvar) return;
    bool training = trainingCvar.getBoolValue();
    if (ImGui::Checkbox("Show custom training activity", &training)) {
        trainingCvar.setValue(training);
    }

    CVarWrapper spectateCvar = cvarManager->getCvar("drpc_spectate");
    if (!spectateCvar) return;
    bool spectate = spectateCvar.getBoolValue();
    if (ImGui::Checkbox("Show spectate activity", &spectate)) {
        spectateCvar.setValue(spectate);
    }

    CVarWrapper localCvar = cvarManager->getCvar("drpc_local");
    if (!localCvar) return;
    bool local = localCvar.getBoolValue();
    if (ImGui::Checkbox("Show local match activity", &local)) {
        localCvar.setValue(local);
    }
}

// help window
void DiscordRPCPlugin::Render() {
    if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    ImGui::Text("THIS IS NECESSARY FOR STEAM USERS ON INITIAL INSTALL OF THE PLUGIN\n\n\n");
    ImGui::Text("Click the button below to open your bakkesmod dll folder. Copy discord_game_sdk.dll to your clipboard.");
    if (ImGui::Button("DLL folder")) {
        HMODULE b = GetModuleHandleA("bakkesmod.dll");
        char path[128];
        DWORD length = GetModuleFileNameA(b, path, 128);
        path[length - 13] = '\0'; // remove bakkesmod.dll from path
        
        ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
    }
    ImGui::Text("Now click the button below to open your Rocket League folder. Rename the discord_game_sdk.dll to something else for safe keeping, and paste your new dll in the folder.");
    if (ImGui::Button("Rocket League folder")) {
        char path[128];
        DWORD length = GetModuleFileNameA(NULL, path, 128);
        path[length - 16] = '\0'; // remove RocketLeague.exe from path

        ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
    }
    ImGui::Text("Now restart your game and your rich presence should be working.");

    ImGui::End();

    if (!isWindowOpen_) {
        cvarManager->executeCommand("togglemenu " + GetMenuName());
    }
}

// Name of the menu that is used to toggle the window.
std::string DiscordRPCPlugin::GetMenuName() {
    return "drpc_setup";
}

// Title to give the menu
std::string DiscordRPCPlugin::GetMenuTitle() {
    return menuTitle_;
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool DiscordRPCPlugin::ShouldBlockInput() {
    return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool DiscordRPCPlugin::IsActiveOverlay() {
    return true;
}

// Called when window is opened
void DiscordRPCPlugin::OnOpen() {
    isWindowOpen_ = true;
}

// Called when window is closed
void DiscordRPCPlugin::OnClose() {
    isWindowOpen_ = false;
}