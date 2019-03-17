#include "Player.h"
#include <string>
#include <iostream>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
Player::Player(uint id, int x, int y): coordinate(x, y), id(id) {}
Player::~Player() {}

uint Player::getID() const {
  return this->id;
}
const Coordinate& Player::getCoord() const {
  return this->coordinate;
}

int Player::getHP() const {
  return this->HP;
}

std::string Player::getBoardID() const {
  if (this->id < 10) {
    std::string unitsDigit = std::to_string(this->id);
    std::string boardID = "0" + unitsDigit;
    return boardID;
  }
  else {
    std::string boardID = std::to_string(this->id);
    return boardID;
  }
}

Armor Player::getArmor() const {
  return this->getArmor();
}

Weapon Player::getWeapon() const {
  return this->getWeapon();
}

std::vector<Move> Player::getPriorityList() const {
  return this->getPriorityList();
}

const std::string Player::getFullName() const {
  return this->getFullName();
}

const std::string Player::getName() const {
  return this->getName();
}


bool Player::isDead() const {
  return (this->HP <= 0);
}
//"Tracer00(100) moved UP."
void Player::executeMove(Move move) {
    std::string moveStr;
    switch (move) {
      case UP:
        moveStr = "UP";
        break;
      case DOWN:
        moveStr = "DOWN";
        break;
      case LEFT:
        moveStr = "LEFT";
        break;
      case RIGHT:
        moveStr = "RIGHT";
        break;
      default:
        moveStr = "noop";
    }

    if (moveStr != "noop") {
      this->coordinate = this->coordinate.operator+(move);
      std::cout << this->getFullName() << "(" << this->getHP() << ") moved " << moveStr << "." << std::endl;
    }
}

// "Tracer00(100) ATTACKED Tracer01(100)! (-10)"
bool Player::attackTo(Player *player) {
  if (!player->isDead() && player != this) {
    int lhsDamage = Entity::damageForWeapon(this->getWeapon());
    int rhsArmor = Entity::damageReductionForArmor(player->getArmor());
    int playerOldHP = player->getHP();
    player->HP -= std::max((lhsDamage - rhsArmor), 0);
    int damageDone = player->getHP() - playerOldHP;
    if (damageDone == 0) std::cout << this->getFullName() << "(" << this->getHP() << ") ATTACKED " << player->getFullName() << "(" << playerOldHP << ")! (-" << damageDone << ")" << std::endl;
    else
      std::cout << this->getFullName() << "(" << this->getHP() << ") ATTACKED " << player->getFullName() << "(" << playerOldHP << ")! (" << damageDone << ")" << std::endl;
  }
  return (player->isDead());
}

void Player::setHPAfterStorm(int damage) {
  this->HP -= damage;
}

bool compare(const Player &lhs, const Player &rhs) {
  return lhs.getID() < rhs.getID();
}


/**
 * Return different colors for different Player classes (override!).
 *
 * Note: This method is optional. You may leave this as-is.
 *
 * @return The associated color code with the class.
 */
 /*
Color::Code Player::getColorID() const {
  return this->getColorID();
}
*/
