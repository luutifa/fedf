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

#include "effect.hpp"
#include <iostream>

Effect::Effect(GameData* igameData, std::string dir, bool right, SDL_Rect icoordSize):
gameData(igameData),
done(false),
coordSize(icoordSize) {
    if (right)
        sprite = new AnimatedSprite(gameData, dir, CHARACTER_RIGHT_SPRITE_FILE, coordSize.w, coordSize.h, &done);
    else
        sprite = new AnimatedSprite(gameData, dir, CHARACTER_LEFT_SPRITE_FILE, coordSize.w, coordSize.h, &done);
    sprite->doOnce(0);
}

Effect::~Effect() {
    delete sprite;
}

bool Effect::draw() {
    SDL_Rect sr = sprite->getFrame();
    SDL_BlitSurface(sprite->getSurface(), &sr, gameData->buffer, &coordSize);
    return done;
}
