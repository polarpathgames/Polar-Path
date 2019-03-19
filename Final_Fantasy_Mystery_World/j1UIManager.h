#ifndef __j1UIMANAGER_H__
#define __j1UIMANAGER_H__

#include "j1Module.h"
#include "p2Animation.h"
#include <list>

enum UIType
{
	IMAGE,
	LABEL,
	BUTTON,
	NON,
};

struct SDL_Texture;
struct SDL_Rect;

class GUI;
class GUI_Image;
class GUI_Label;
class GUI_Button;

class j1UIManager: public j1Module
{
public:
	j1UIManager();
	virtual ~j1UIManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	GUI_Image* AddImage(int x, int y, SDL_Rect* rect, Animation *anim, j1Module* callback, GUI* parent);
	GUI_Button* AddButton(int x, int y, SDL_Rect normal, SDL_Rect mouse_in, SDL_Rect clicked, j1Module* callback, GUI* parent);
	GUI_Label* AddLabel(int x, int y, std::string text, j1Module* callback, GUI* parent);

	void DestroyUI();
	const SDL_Texture* GetAtlas() const;

private:
	SDL_Texture* atlas = nullptr;
	std::list<GUI*> ui_list;

	bool debug_ui = false;

public:



};
#endif // !__j1UIMANAGER_H__
