#ifndef __m1INPUT_H__
#define __m1INPUT_H__

#include "m1Module.h"
#include "SDL\include\SDL_scancode.h"
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_gamecontroller.h"
#include "SDL\include\SDL_haptic.h"

//#define NUM_KEYS 352
#define NUM_CONTROLLER_BUTTONS SDL_CONTROLLER_BUTTON_MAX //16
#define NUM_MOUSE_BUTTONS 5
#define DEAD_ZONE 10000 // 0 - 32767
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct axis {
	int value = 0;
	j1KeyState state = j1KeyState::KEY_IDLE;
};

struct ButtonsUsed {

	int UP;
	int DOWN;
	int LEFT;
	int RIGHT;
	int DIRECTION_UP;
	int DIRECCTION_DOWN;
	int DIRECTION_LEFT;
	int DIRECCTION_RIGHT;
	int DIAGONALS;
	int BASIC_ATTACK;
	int FAST_SKILLS;
	int ABILTY1;
	int ABILITY2;
	int ABILITY3;
	int INVENTORY;

};


struct ButtonChar {
	char* UP = nullptr;
	char* DOWN = nullptr;
	char* LEFT = nullptr;
	char* RIGHT = nullptr;
	char* DIRECTION_UP = nullptr;
	char* DIRECCTION_DOWN = nullptr;
	char* DIRECTION_LEFT = nullptr;
	char* DIRECCTION_RIGHT = nullptr;
	char* DIAGONALS = nullptr;
	char* FAST_SKILLS = nullptr;
	char* BASIC_ATTACK = nullptr;
	char* ABILITY1 = nullptr;
	char* ABILITY2 = nullptr;
	char* ABILITY3 = nullptr;
	char* INVENTORY = nullptr;
};


struct KeyboardButtons {
	ButtonsUsed buttons_code;
	ButtonChar buttons_char;
};

struct ControllerButtons {
	ButtonsUsed buttons_code;
	ButtonChar buttons_char;
};
enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

class m1Input : public m1Module
{

public:

	m1Input();

	// Destructor
	virtual ~m1Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	void UpdateEvents(SDL_Event &event);

	void UpdateController();

	void UpdateMouse();

	void UpdateKeyboard();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	bool GetKeyDown(int id) const
	{
		return keyboard[id] == KEY_DOWN;
	}
	bool GetKeyRepeat(int id) const
	{
		return keyboard[id] == KEY_REPEAT;
	}
	bool GetKeyDownOrRepeat(int id) const
	{
		return keyboard[id] == KEY_DOWN || keyboard[id] == KEY_REPEAT;
	}
	bool GetKeyUp(int id) const
	{
		return keyboard[id] == KEY_UP;
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	j1KeyState GetControllerButton(int id) const
	{
		return controller_buttons[id];
	}
	bool GetControllerButtonDown(int id)const {
		return controller_buttons[id] == KEY_DOWN;
	}
	bool GetControllerButtonRepeat(int id)const {
		return controller_buttons[id] == KEY_REPEAT;
	}
	bool GetControllerButtonDownOrRepeat(int id)const {
		return controller_buttons[id] == KEY_DOWN || controller_buttons[id] == KEY_REPEAT;
	}
	bool GetControllerButtonUp(int id)const {
		return controller_buttons[id] == KEY_UP;
	}

	bool GetAnyMovementKey();

	float GetAxis(const SDL_GameControllerAxis &axis);
	// * Return 0 if joystick is between dead zone, 1 if greater, -1 lower
	int GetAxisRaw(const SDL_GameControllerAxis &axis);
	int GetAxisDown(const SDL_GameControllerAxis &axis) const;
	bool GetAxisUp(const SDL_GameControllerAxis &axis);

	void DefaultControls();

	bool ControllerVibration(float strength, uint32 duration);
	

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);
	bool MovedMouse();

	KeyboardButtons keyboard_buttons;
	ControllerButtons controller_Buttons;
	SDL_GameController* controller = nullptr;

	bool is_a_key_down = false;

	//Controler Haptic

	SDL_Joystick *joystick;
	SDL_GameController *Pad;
	SDL_HapticEffect effect;
	std::string text_input;

	SDL_Haptic *haptic = nullptr;


private:
	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	SDL_Event ev;

	j1KeyState controller_buttons[NUM_CONTROLLER_BUTTONS];

	int			mouse_motion_x = 0;
	int			mouse_motion_y = 0;
	int			mouse_x = 0;
	int			mouse_y = 0;
	int			last_mouse_x = 0;
	int			last_mouse_y = 0;
	axis		joy[SDL_CONTROLLER_AXIS_MAX];
};

#endif // __j1INPUT_H__