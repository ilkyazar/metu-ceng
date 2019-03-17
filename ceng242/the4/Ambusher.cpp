#include "Ambusher.h"
#include <sstream>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
// Name     : Ambusher
// Priority : { ATTACK }

// Armor    : NOARMOR
// Weapon   : SEMIAUTO
// HP       : 100

Ambusher::Ambusher(uint id, int x, int y): Player(id, x, y) {this->HP = 100;};

Armor Ambusher::getArmor() const {return NOARMOR;}

Weapon Ambusher::getWeapon() const {return SEMIAUTO;}

std::vector<Move> Ambusher::getPriorityList() const {return { ATTACK }; }

const std::string Ambusher::getFullName() const {
  std::stringstream id;
  id << this->getBoardID();
  std::string fullName = "Ambusher" + id.str();
  return fullName;
}

const std::string Ambusher::getName() const {
  return "Ambusher";
}


Color::Code Ambusher::getColorID() const {return Color::FG_BLUE;}
