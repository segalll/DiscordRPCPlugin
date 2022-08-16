# DiscordRPCPlugin
Custom plugin for BakkesMod that integrates Discord Rich Presence into Rocket League.

https://bakkesplugins.com/plugins/view/21

## Manual Installation

1. Copy `DiscordRPCPlugin.dll` to `C:\Users\<your user>\AppData\Roaming\bakkesmod\bakkesmod\plugins`
2. Add `plugin load discordrpcplugin` to `C:\Users\<your user>\AppData\Roaming\bakkesmod\bakkesmod\cfg\plugins.cfg`

## Initial Setup

Make sure to follow the initial setup instructions or else this plugin will not work for Steam users. A button to open the instructions is located in the plugin settings (F2 -> Plugins -> DiscordRPCPlugin)

Essentially, the dummy discord_game_sdk.dll needs to replace the original in the Rocket League binary folder. I recommend saving the original just in case.

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
