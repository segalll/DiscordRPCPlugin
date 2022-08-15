#define CFUNC(func, ...) __declspec(dllexport) void func(__VA_ARGS__) { return; }
extern "C" {
	CFUNC(DiscordCreate, void)
	CFUNC(DiscordVersion, void)
	CFUNC(rust_eh_personality, void)
}
