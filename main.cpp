// Copyright 2014, 2015 Lauri Gustafsson, Kalle Korhonen and Julius Heino
/*
This file is part of Fedora Fighters.

    Fedora Fighters is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Fedora Fighters is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Fedora Fighters, see COPYING. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stage.hpp"
#include "game_data.hpp"
#include "game.hpp"
#include "gfxutil.hpp"
#include "config.hpp"
#include "player.hpp"
#include "menu.hpp"
#include "util.hpp"
#include <SDL/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

void doSplashScreen(GameData* gameData, SDL_Surface* screen) {
	SDL_Surface* splash;
	file2surface("gfx/splash.png", &splash);
	SDL_BlitSurface(splash, NULL, gameData->buffer, NULL);
	#ifdef DBHACK
        SDL_BlitSurface(gameData->buffer, NULL, screen, NULL);
    #endif
	SDL_Flip(screen);
	SDL_Delay(SPLASH_DELAY);
	SDL_FreeSurface(splash);
}

int main(int argc, char* argv[]) {
    std::cout << "Copyright 2014 Lauri Gustafsson, Kalle Korhonen and Julius Heino.\nFedora Fighters " << VERSION << std::endl << std::endl <<
    "Fedora Fighters is free software: you can redistribute it and/or modify\n" <<
    "it under the terms of the GNU General Public License as published by\n" <<
    "the Free Software Foundation, either version 3 of the License, or\n" <<
    "(at your option) any later version.\n" <<
    "\n" <<
    "Fedora Fighters is distributed in the hope that it will be useful,\n" <<
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n" <<
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" <<
    "GNU General Public License for more details.\n" <<
    "\n" <<
    "You should have received a copy of the GNU General Public License\n" <<
    "along with Fedora Fighters, see COPYING. If not, see <http://www.gnu.org/licenses/>.\n";

    atexit(SDL_Quit);
    srand(609);
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return -1;
	
	SDL_Surface *screen;
	
    #ifndef DBHACK
        #ifdef NOFS
            screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
        #else
            screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
        #endif
    #else
		#ifdef NOFS
			screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE);
		#else
			screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE|SDL_FULLSCREEN);
		#endif
    #endif
	
	if (!screen)
        return -2;
    
    GameData gameData(screen);

    std::string caption = gameData.name;
    caption += " ";
    caption += VERSION;
    SDL_WM_SetCaption(caption.c_str(), NULL);
    SDL_ShowCursor(false);
    
    doSplashScreen(&gameData, screen);

	Game* game = NULL;
	Menu* menu = new Menu(&gameData, true);
	bool inMenu = true;

    unsigned int timeLast;
    int delay = 0;
    int frames = 0;
    int fpsLast = 0;
    int fps = 0;
    while (gameData.running)
	{	
        timeLast = SDL_GetTicks();
		SDL_PollEvent(&gameData.gameEvent);
        gameData.keystate = SDL_GetKeyState(NULL);
		
		if (gameData.gameEvent.type == SDL_QUIT) gameData.running = false;
		
		if (inMenu) {
			game = menu->update();
			if (game != NULL) {
				inMenu = false;
				delete menu;
			}
		} else {
			game->update();
			if (game->isFinished()) {
				delete game;
				menu = new Menu(&gameData);
				inMenu = true;
			}
		}
        
        #ifdef DBHACK
            SDL_BlitSurface(gameData.buffer, NULL, screen, NULL);
        #endif
		SDL_Flip(screen);
        #ifndef BENCHMARK
            delay = (1000/gameData.fps - (SDL_GetTicks() - timeLast));
            SDL_Delay(delay < 0 ? 0 : delay);
        #endif
        
        frames++;
        if (fpsLast+10000<SDL_GetTicks()) {
            fpsLast=SDL_GetTicks();
            fps = frames/10.0;
            std::cout << "FPS: " << fps << std::endl;
            if (fps < (gameData.fps - 5))
				std::cout << "Target framerate not achieved! If this message repeats, your computer is not fast enough for intended gameplay!\n";
            frames = 0;
        }
        gameData.frame++;
	}
	
	SDL_Quit();

    return 0;
}
