#include "Dummy.h"
#include <sstream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
// Name     : Dummy
// Priority : { NOOP }

// Armor    : NOARMOR
// Weapon   : NOWEAPON
// HP       : 1000
Dummy::Dummy(uint id, int x, int y): Player(id, x, y) {this->HP = 1000;}

Armor Dummy::getArmor() const {return NOARMOR;}

Weapon Dummy::getWeapon() const {return NOWEAPON;}

std::vector<Move> Dummy::getPriorityList() const {return { NOOP }; }

const std::string Dummy::getFullName() const {
  std::stringstream id;
  id << this->getBoardID();
  std::string fullName = "Dummy" + id.str();
  return fullName;
}

const std::string Dummy::getName() const {
  return "Dummy";
}


Color::Code Dummy::getColorID() const {return Color::FG_DEFAULT;}
