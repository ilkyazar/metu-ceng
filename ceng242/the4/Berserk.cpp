#include "Berserk.h"
#include <sstream>

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
// Name     : Berserk
// Priority : { ATTACK, UP, LEFT, DOWN, RIGHT }

// Armor    : WOODEN
// Weapon   : PISTOL
// HP       : 100

Berserk::Berserk(uint id, int x, int y): Player(id, x, y) {this->HP = 100;}

Weapon Berserk::getWeapon() const {return PISTOL;}

Armor Berserk::getArmor() const {return WOODEN;}

std::vector<Move> Berserk::getPriorityList() const {return { ATTACK, UP, LEFT, DOWN, RIGHT }; }

const std::string Berserk::getFullName() const {
  std::stringstream id;
  id << this->getBoardID();
  std::string fullName = "Berserk" + id.str();
  return fullName;
}

const std::string Berserk::getName() const {
  return "Berserk";
}

Color::Code Berserk::getColorID() const {return Color::FG_RED;}
