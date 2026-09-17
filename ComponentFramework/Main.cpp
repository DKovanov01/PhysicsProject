#include <memory>
#include "SceneManager.h"
#include "Debug.h"
#include "MemoryMonitor.h"
 
int main(int argc, char* args[]) {
	// We'll write info to a log file
	Debug::DebugInit("GameEngineLog.txt");
	
	// The only unique pointer we'll have in the whole engine. We'll use shared for the rest
	std::unique_ptr<SceneManager> game = std::make_unique<SceneManager>();
	if (game->Initialize("Game Engine", 1280, 720) == true) {
		game->Run();
	}

	writeMemoryLog();
	return 0;
}