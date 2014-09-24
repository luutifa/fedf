// Copyright 2014 Lauri Gustafsson, Kalle Korhonen and Julius Heino
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

#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

#include <SDL/SDL.h>
#include <string>

class GameData {
    public:
        GameData(SDL_Surface* screen);
        SDL_Surface* buffer;
        unsigned int stageFloor;
        SDL_Event gameDataEvent;
        Uint8* keystate;
        bool running;
        std::string name;
};

#endif
