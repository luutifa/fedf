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

#include "gfxutil.hpp"
#include "player.hpp"
#include "player_controls.hpp"
#include "config.hpp"
#include "util.hpp"
#include "character.hpp"
#include <SDL/SDL.h>
#include <iostream>

Player::Player(GameData* igameData, PlayerControls icontrols, SDL_Rect spawn, bool iright, Character icharacter):
health(100),
gameData(igameData),
character(icharacter),
controls(icontrols),
active(false),
moved(false),
fall(0),
jump(0),
xjump(0),
speed(6),
right(iright) {
    #ifdef DEBUG
        std::cout << "Player constructor\n";
    #endif
    coord.x = spawn.x;
    coord.y = spawn.y;
    coord.w = 64;
    coord.h = 64;
    
    file2surface("gfx/plshadow.png", &shadow);
    
    sprite = new AnimatedSprite(igameData, icharacter.dir, CHARACTER_SPRITE_FILE);
    
    #ifdef DEBUG
        std::cout << "Player constructor finish\n";
    #endif
}

void Player::clean() {
    #ifdef DEBUG
        std::cout << "Player clean\n";
    #endif
    SDL_FreeSurface(shadow);
    delete sprite;
    #ifdef DEBUG
        std::cout << "Player clean finish\n";
    #endif
}

unsigned int Player::getHealth() {
    return (health > 0) ? health : 0;
}

void Player::takeDamage(unsigned int amnt) {
    if (active)
        health -= amnt;
}

SDL_Rect Player::getCoord() {
    return coord;
}

void Player::start() {
    active = true;
}

void Player::stop() {
    active = false;
}

bool Player::isActive() {
    return active;
}

void Player::push(int vec) {
    coord.x += vec;
}

void Player::update() {
    #ifdef DEBUG
        std::cout << "Player update\n";
    #endif
    if (health < 0)
        health = 0;
    
    if (fall > 0) {
        coord.y += fall;
        fall++;
        moved = true;
        #ifdef DEBUG
            std::cout << "Falling...\n";
        #endif
    }
    if (jump > 0){
        fall = 0;
        coord.y -= jump;
        jump--;
        moved = true;
        #ifdef DEBUG
            std::cout << "Jumping...\n";
        #endif
    }
    coord.x+=xjump;
    if (coord.y+coord.h > gameData->stageFloor) {
        fall = 0;
        jump = 0;
        xjump = 0;
        coord.y = gameData->stageFloor-coord.h;
        moved = false;
        #ifdef DEBUG
            std::cout << "Fell to ground...\n";
        #endif
    }
    if ((fall == 0) && (jump == 0) && (coord.y+coord.h < gameData->stageFloor)) {
        fall=1;
        moved=true;
        #ifdef DEBUG
            std::cout << "Now falling...\n";
        #endif
    }
    
    if (active) {
        if (gameData->keystate[controls.LEFT] && jump==0 && fall==0) {
            moved = true;
            if (coord.x > 0)
                coord.x-=speed;
        } if (gameData->keystate[controls.RIGHT] && jump==0 && fall==0) {
            moved = true;
            if (coord.x+coord.w < WIDTH)
                coord.x+=speed;
        } if (gameData->keystate[controls.JUMP]) {
            if (jump == 0 && fall == 0) {
                moved = true;
                jump = 16;
                if (gameData->keystate[controls.RIGHT])
					xjump = speed;
				if (gameData->keystate[controls.LEFT])
					xjump = -speed;
			}
        }
    }
    
    if (coord.x < 0)
        coord.x = 0;
    if (coord.y <= 0)
        coord.y = 1;
    if (coord.x+coord.w > WIDTH)
        coord.x = WIDTH-coord.w;
    
    if (!moved)
        sprite->hold();
        
    moved = false;
    
    #ifdef DEBUG
        std::cout << "Player update finish\n";
    #endif
}

void Player::drawShadow() {
	SDL_Rect shadowRect = buildRect(coord.x, gameData->stageFloor-16, 64, 32);
    SDL_BlitSurface(shadow, NULL, gameData->buffer, &shadowRect);
}

void Player::draw() {
    SDL_Rect animSrc = sprite->getFrame();
	SDL_BlitSurface(sprite->getSurface(), &animSrc, gameData->buffer, &coord);
}
