#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480 
#define SCREEN_SIZE 1

#include "j1Module.h"

struct SDL_Window;
struct SDL_Surface;

class j1Window : public j1Module
{
public:

	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	uint		width;
	uint		height;
	uint		scale;

private:
	std::string	title;
};

#endif // __j1WINDOW_H__