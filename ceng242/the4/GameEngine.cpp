#include "GameEngine.h"
#include <iostream>
#include <algorithm>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
/**
 * Constructor.
 *
 * GameEngine "owns" these players.
 * GameEngine also "owns" the vector.
 *
 * @param boardSize The side length of the board.
 * @param players All players to participate in the game.
 */
GameEngine::GameEngine(uint boardSize, std::vector<Player *> *players): board(boardSize, players) {
  this->currentRound = 1;
}

GameEngine::~GameEngine() {
  delete board.getPlayersVector();
}

const Board& GameEngine::getBoard() const {
  return this->board;
}

Player* GameEngine::operator[](uint id) const {
  return board.getPlayer(id);
}

/**
 * Decide whether the game is finished.
 *
 * @return true if there is only 1 player (alive), on top of the hill; or there are 0 players. False otherwise.
 */
bool GameEngine::isFinished() const {
  std::vector<Player*>* v = board.getPlayersVector();
  std::vector<Player*> &vRef = *v;
  if (v->size() == 0) return true;
  else if (v->size() == 1 && !vRef[0]->isDead()) return true;
  return false;
}

/**
 * Take turn for every player.
 *
 * How-to:
 * - Announce turn start (cout).
 * Example: -- ROUND 1 START --
 * - board.updateStorm(currentRound)
 * - For every player (sorted according to their IDs) that isn't dead (HP <= 0):
 *      - takeTurnForPlayer(player).
 * - Announce turn end (cout).
 * Example: -- ROUND 1 END --
 */

std::vector<Player*>* GameEngine::sortPlayersByID() {
  std::vector<Player*>* playersCopy = board.getPlayersVector();
  //std::sort(playersCopy->begin(), playersCopy->end(), std::less<Player*>());
  for (int i = 1; i < playersCopy->size(); ++i) {
    for (int j = 0; j < playersCopy->size(); ++j) {
      if (playersCopy->at(j)->getID() > playersCopy->at(i)->getID())
        std::swap(playersCopy->at(j), playersCopy->at(i));
    }
  }
  return playersCopy;
}

void GameEngine::takeTurn() {
  std::cout << "-- START ROUND " << this->currentRound << " --" << std::endl;
  this->board.updateStorm(this->currentRound);
  std::vector<Player*>* v = this->sortPlayersByID();
  std::vector<Player*> &vRef = *v;
  int i = 0;
  while(i < v->size()) {
    if (!vRef[i]->isDead())
      takeTurnForPlayer(vRef[i]->getID());
    i++;
  }
  std::cout << "-- END ROUND " << this->currentRound << " --" << std::endl;
  this->currentRound += 1;
}

Move GameEngine::takeTurnForPlayer(uint playerID) {
  Player* player = board.getPlayer(playerID); //Get player with ID.
  Coordinate coord = player->getCoord();

  if (!player) return NOOP; //Return NOOP if not exists.

  std::vector<Move> priorityList = player->getPriorityList(); //Get player's priority list.
  std::vector<Coordinate> visibility = this->board.visibleCoordsFromCoord(coord); //Get player's visibility from the board (visibleCoordsFromCoord).

  if(this->board.isStormInCoord(coord)) { //If the player is in the storm (isStormInCoord)
    int stormDamage = Entity::stormDamageForRound(this->currentRound);
    std::cout << player->getFullName() << "(" << player->getHP() << ")" << " is STORMED! (-" << stormDamage << ")" << std::endl; //announce the damage
    player->setHPAfterStorm(stormDamage); //and give player stormDamage.
  }

  if(player->isDead()) {
    std::cout << player->getFullName() << "(" << player->getHP() << ")" << " DIED." << std::endl; //If dead, announce the death
    this->board.removePlayer(player->getID()); //remove player from the board/list/anywhere
    return NOOP; //and return NOOP
  }

  for (int i = 0; i < priorityList.size(); i++) {
      if (priorityList[i] == NOOP) return NOOP;

      else if(priorityList[i] == ATTACK) {
        std::vector<Player*> attackablePlayers;

        for (int j = 0; j < visibility.size(); j++) {
          if (this->board[visibility[j]])
            attackablePlayers.push_back(this->board[visibility[j]]);
        }

        if (attackablePlayers.size() == 0) continue;
        else {
          Player* p = pickLowestID(attackablePlayers);
          bool isPlayerDead = player->attackTo(p);
          if (isPlayerDead) {
            std::cout << p->getFullName() << "(" << p->getHP() << ")" << " DIED." << std::endl; //If dead, announce the death
            this->board.removePlayer(p->getID()); //remove player from the board/list/anywhere
          }
          return ATTACK;
        }
      }

      else {
        Coordinate coordWithMove = this->board.calculateCoordWithMove(priorityList[i], coord);
        if (coordWithMove != coord && isGettingCloserToHill(coord, coordWithMove)) {
          player->executeMove(priorityList[i]);
          return priorityList[i];
        }
        else
          continue;
      }
  }
  return NOOP;
}

Player* GameEngine::pickLowestID(std::vector<Player*> attackablePlayers) {
  Player* p = attackablePlayers[0];
  int i  = 0;
  if (attackablePlayers.size() >= 2) {
    while(i < attackablePlayers.size()) {
      if (attackablePlayers[i]->getID() < p->getID())
        p = attackablePlayers[i];
      i++;
    }
  }
  return p;
}

bool GameEngine::isGettingCloserToHill(Coordinate coord, Coordinate coordWithMove) {
  int hillX = (board.getSize() - 1) / 2;
  int hillY = hillX;

  Coordinate hill = Coordinate(hillX, hillY);

  int distance = hill.Coordinate::operator-(coord);
  int newDistance = hill.Coordinate::operator-(coordWithMove);

  return newDistance < distance;
}
/**
 * Find the winner player.
 *
 * nullptr if there are 0 players left, or the game isn't finished yet.
 *
 * @return The winner player.
 */
Player* GameEngine::getWinnerPlayer() const {
  if (board.getPlayersVector()->size() == 0 || !isFinished()) return nullptr;

  int hillX = (board.getSize() - 1) / 2;
  int hillY = hillX;

  Coordinate hill = Coordinate(hillX, hillY);

  return board[hill];
}
