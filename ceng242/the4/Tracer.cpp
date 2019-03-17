#include "Tracer.h"
#include <sstream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
// Name     : Tracer
// Priority : { UP, LEFT, DOWN, RIGHT, ATTACK }

// Armor    : BRICK
// Weapon   : SHOVEL
// HP       : 100

Tracer::Tracer(uint id, int x, int y): Player(id, x, y) {this->HP = 100;}

Armor Tracer::getArmor() const {return BRICK;}

Weapon Tracer::getWeapon() const {return SHOVEL;}

std::vector<Move> Tracer::getPriorityList() const {return { UP, LEFT, DOWN, RIGHT, ATTACK }; }

const std::string Tracer::getFullName() const {
  std::stringstream id;
  id << this->getBoardID();
  std::string fullName = "Tracer" + id.str();
  return fullName;
}

const std::string Tracer::getName() const {
  return "Tracer";
}


Color::Code Tracer::getColorID() const {return Color::FG_YELLOW;}
