# DiscordRPCPlugin
Custom plugin for BakkesMod that integrates Discord Rich Presence into Rocket League.

## Installation

1. Copy `DiscordRPCPlugin.dll` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/plugins`
2. Copy `discordrpcplugin.set` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/plugins/settings`
3. Add `plugin load discordrpcplugin` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/cfg/plugins.cfg`

## Commands

**enableRPC 1|0** - Enable Discord RPC (Default: 1)

**showOnlineGames 1|0** - Should show online games in Rich Presence (Default: 1)

**showReplayViewing 1|0** - Should show replay viewing in Rich Presence (Default: 1)

**showFreeplay 1|0** - Should show freeplay in Rich Presence (Default: 1)

**showCustomTraining 1|0** - Should show custom training in Rich Presence (Default: 1)

**showOnlineSpectating 1|0** - Should show spectating online games in Rich Presence (Default: 1)

## Modifying

If you are going to modify and compile on your own, keep in mind this requires the win32-static [discord-rpc SDK](https://github.com/discordapp/discord-rpc/releases).
You should also change the include and lib locations to point to the BakkesMod SDK and discord-rpc SDK.
