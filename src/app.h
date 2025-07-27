#pragma once

union SDL_Event;

namespace hermes {

class application
{
public:
	application();
	~application();

	void Run();
	bool Running() const { return running; }

private:
	void init();
	bool running = false;
};

}; // namespace hermes