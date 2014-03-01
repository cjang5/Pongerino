#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <stdio.h>

//window dimensions
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//sdl variables
SDL_Window* mainWindow = NULL;
SDL_Renderer* globalRenderer = NULL;

//the bars
SDL_Texture* leftBar = NULL;
SDL_Texture* rightBar = NULL;
int leftBarRectPosY = 0;
float rightBarRectPosY = 0.0;
bool movingUp = false; //determine whether the AI should be moving up or down

//forward declarations
bool init();
bool loadMedia();
SDL_Texture* loadTexture(std::string path);
void close();

bool init() {
	//initialization flag
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to initialize SDL! SDL ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			printf("Warning: texture filtering is not in 'linear' mode!\n");

		//create window
		mainWindow = SDL_CreateWindow("Bar mover", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (mainWindow == NULL) {
			printf("Failed to create window! SDL ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//create renderer
			globalRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);

			if (globalRenderer == NULL) {
				printf("Failed to create renderer! SDL ERROR: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//initialize draw color mode for renderer
				SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//imgFlags
				int imgFlags = IMG_INIT_PNG;

				if (!IMG_Init(imgFlags) & imgFlags) {
					printf("Failed to initialize SDL_Image! IMG ERROR: %s\n:", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	//initialization flag
	bool success = true;

	//left bar image
	leftBar = loadTexture("assets/bar_left.png");

	//right bar image
	rightBar = loadTexture("assets/bar_right.png");

	if (leftBar == NULL) {
		printf("Failed to load 'bar_left' image!\n");
		success = false;
	}

	if (rightBar == NULL) {
		printf("Failed to load 'bar_right' image!\n");
		success = false;
	}

	return success;
}

SDL_Texture* loadTexture(std::string path) {
	//final texture to be returned
	SDL_Texture* finalTexture = NULL;

	//loaded image surface
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		printf("Failed to load surface! IMG ERROR: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//create texture from the surface
		finalTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);

		if (finalTexture == NULL) {
			printf("Failed to create texture from surface! ERROR: %s\n", SDL_GetError());
		}

		//free the surface because we don't need it
		SDL_FreeSurface(loadedSurface);
	}

	return finalTexture;
}

void close() {
	//destroy textures
	SDL_DestroyTexture(leftBar);
	leftBar = NULL;
	SDL_DestroyTexture(rightBar);
	rightBar = NULL;

	//destroy renderer
	SDL_DestroyRenderer(globalRenderer);
	globalRenderer = NULL;

	//destroy window
	SDL_DestroyWindow(mainWindow);
	mainWindow = NULL;

	//quit
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			//main loop flag
			bool quit = false;

			//sdl event handler
			SDL_Event e;

			//main loop
			while (!quit) {
				//while there are events on the queue
				while (SDL_PollEvent(&e) != 0) {
					//if user clicks 'x' to quit
					if (e.type == SDL_QUIT)
						quit = true;

					//if a key is pressed
					if (e.type == SDL_KEYDOWN) {
						//events based on what key is pressed
						switch (e.key.keysym.sym) {
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_DOWN:
							leftBarRectPosY += 10;
							break;
						case SDLK_UP:
							leftBarRectPosY -= 10;
							break;
						}
					}
				}
				//set teh color
				SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				//clear screen
				SDL_RenderClear(globalRenderer);

				//render to dimensions of the left bar
				SDL_Rect leftBarRect;
				leftBarRect.x = 0;
				leftBarRect.y = leftBarRectPosY;
				leftBarRect.w = 100;
				leftBarRect.h = 300;

				//dont let the bar go off the screen
				if (leftBarRect.y < 0)
					leftBarRectPosY = 0;
				if (leftBarRect.y > SCREEN_HEIGHT - 300)
					leftBarRectPosY = SCREEN_HEIGHT - 300;

				//set viewport to leftBarRect
				SDL_RenderSetViewport(globalRenderer, &leftBarRect);

				//draw the left bar image
				SDL_RenderCopy(globalRenderer, leftBar, NULL, NULL);

				//rect for the AI bar
				SDL_Rect rightBarRect;
				rightBarRect.x = SCREEN_WIDTH - 100;
				rightBarRect.y = rightBarRectPosY;
				rightBarRect.w = 100;
				rightBarRect.h = 300;

				//set viewport to rightBarRect
				SDL_RenderSetViewport(globalRenderer, &rightBarRect);

				//dont let the AI bar go off the screen
				if (rightBarRect.y < 0) {
					rightBarRectPosY = 0;
					movingUp = false;
				}
				if (rightBarRect.y > SCREEN_HEIGHT - 300) {
					rightBarRectPosY = SCREEN_HEIGHT - 300;
					movingUp = true;
				}

				//draw the AI image
				SDL_RenderCopy(globalRenderer, rightBar, NULL, NULL);

				//increment the ai bar so it keeps moving
				if (movingUp) {
					rightBarRectPosY -= 0.01;
				}
				else {
					rightBarRectPosY += 0.01;
				}

				//update the screen
				SDL_RenderPresent(globalRenderer);
			}
		}
	}

	close();

	return 0;
}