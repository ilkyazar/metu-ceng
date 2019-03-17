#include "GameParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Ambusher.h"
#include "Berserk.h"
#include "Dummy.h"
#include "Pacifist.h"
#include "Tracer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

/**
 * It should boil down to:
 *
 * Tracer(0, 2, 0)
 * Berserk(1, 2, 2)
 */
/**
 * Parse the file with given name and create players accordingly.
 *
 * GameParser DOES NOT have any responsibility over these Players.
 *
 * Note: The file will always exists, and there will be no erroneous input.
 *
 * @param filename The name of the file to be parsed.
 * @return  pair.first: Board size.
 *          pair.second: The vector of the constructed players.
 */
 /**
 * Board Size: 3
 * Player Count: 2
 * 0::Tracer::2::0
 * 1::Berserk::2::2
 */
std::pair<int, std::vector<Player *> *> GameParser::parseFileWithName(const std::string& filename) {
  std::ifstream file;
  std::string line;
  std::string line1, line2;

  file.open(filename.c_str());

  std::string textForSize, textForPlayerCount;
  std::string boardSize, playerCount;

  std::getline(file, line1);
  std::istringstream ss1(line1);
  std::getline(ss1, textForSize, ':');
  std::getline(ss1, boardSize, '\n');

  std::getline(file, line2);
  std::istringstream ss2(line2);
  std::getline(ss2, textForPlayerCount, ':');
  std::getline(ss2, playerCount, '\n');

  int i = 0;
  std::string idtext, xtext, ytext;
  std::string name;
  std::string coma;


  std::vector <Player*> *playerVector = new std::vector<Player*>(0);

  while (i < std::stoi(playerCount)) {
    std::getline(file, line);
    std::istringstream ss(line);
    std::getline(ss, idtext, ':');
    std::getline(ss, coma, ':');
    std::getline(ss, name, ':');
    std::getline(ss, coma, ':');

    std::getline(ss, xtext, ':');
    std::getline(ss, coma, ':');

    std::getline(ss, ytext, '\n');

    int id = std::stoi(idtext);
    int x = std::stoi(xtext);
    int y = std::stoi(ytext);

    if (name == "Berserk") {
      Player* berserk = new Berserk(id, x, y);
      playerVector->push_back(berserk);
    }
    else if (name == "Ambusher") {
      Player* ambusher = new Ambusher(id, x, y);
      playerVector->push_back(ambusher);
    }
    else if (name == "Dummy") {
      Player* dummy = new Dummy(id, x, y);
      playerVector->push_back(dummy);
    }
    else if (name == "Pacifist") {
      Player* pacifist = new Pacifist(id, x, y);
      playerVector->push_back(pacifist);
    }
    else {
      Player* tracer = new Tracer(id, x, y);
      playerVector->push_back(tracer);
    }

    i++;
  }

  file.close();

  std::pair<int, std::vector<Player *> *> pair;
  pair.first = std::stoi(boardSize);
  pair.second = playerVector;

  return pair;
}
