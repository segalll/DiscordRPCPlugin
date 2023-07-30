# DiscordRPCPlugin

[![Rocket Plugins](https://img.shields.io/badge/downloads-20k-blue)](https://bakkesplugins.com/plugins/view/21)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Custom plugin for BakkesMod that integrates Discord Rich Presence into Rocket League.

https://bakkesplugins.com/plugins/view/21

## Manual Installation

1. Copy `DiscordRPCPlugin.dll` to `C:\Users\<your user>\AppData\Roaming\bakkesmod\bakkesmod\plugins`
2. Add `plugin load discordrpcplugin` to `C:\Users\<your user>\AppData\Roaming\bakkesmod\bakkesmod\cfg\plugins.cfg`

## Initial Setup

Make sure to follow the initial setup instructions or else this plugin will not work for Steam users. A button to open the instructions is located in the plugin settings (F2 -> Plugins -> DiscordRPCPlugin)

Essentially, the dummy `discord_game_sdk.dll` needs to replace the original in the Rocket League binary folder. I recommend saving the original just in case. I believe the dummy `discord_game_sdk.dll` is in `C:\Users\<your user>\AppData\Roaming\bakkesmod\bakkesmod\data\DiscordRPCPlugin\dlls`.

Also, this plugin may be blacklisted from loading by Bakkesmod because I left it unupdated, causing it to crash on new maps. If this is true, you will need to rename the DLL to something else and change the corresponding `plugin load` in `plugins.cfg` to account for this change.

## Disclaimer

This plugin may crash in new non-standard game modes due to not being updated to support them in a while.

## Console Variables

**drpc_enabled 1|0** - Enable Discord RPC (Default: 1)

**drpc_online 1|0** - Show online match activity (Default: 1)

**drpc_replay 1|0** - Show replay viewing activity (Default: 1)

**drpc_freeplay 1|0** - Show freeplay activity (Default: 1)

**drpc_training 1|0** - Show custom training activity (Default: 1)

**drpc_spectate 1|0** - Show spectate activity (Default: 1)

**drpc_local 1|0** - Show local match activity (Default: 1)

## Modifying

If you are going to modify and compile on your own, keep in mind this requires the discord-rpc SDK in the DiscordRPCPlugin project folder, modified to be built with /MT from the win64-static [discord-rpc SDK](https://github.com/discordapp/discord-rpc/releases).
