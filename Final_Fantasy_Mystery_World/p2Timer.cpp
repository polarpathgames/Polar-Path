// ----------------------------------------------------
// p2Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "p2Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
p2Timer::p2Timer()
{
	Start();
	running = true;
}

// ---------------------------------------------
void p2Timer::Start()
{
	started_at = SDL_GetTicks();
	running = true;
}

// ---------------------------------------------
uint32 p2Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float p2Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}

bool p2Timer::IsRunning() const
{
	return running;
}
