#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#define INF 32767
using namespace std;

class Path{
public:
    vector<int>oddNodes, evenNodes;
    int oddDistance, evenDistance;

    vector<int>alternativeOddNodes, alternativeEvenNodes;
    int alternativeOddDistance, alternativeEvenDistance;


    Path(int oddDistance, int evenDistance) {
      this->oddDistance = oddDistance;
      this->evenDistance = evenDistance;
      this->alternativeOddDistance = INF;
      this->alternativeEvenDistance = INF;
    }

};

  string printValue(int i) {
    if (i == INF)
      return "-";
    else return to_string(i);
  }

  std::ostream & operator<<(std::ostream &Str, Path const & v) {
    // print something from v to str, e.g: Str << v.getX();
    Str << printValue(v.oddDistance) << " " << printValue(v.evenDistance) << " / " << printValue(v.alternativeOddDistance) << " " << printValue(v.alternativeEvenDistance);
    return Str;
  }


ifstream inpFile;
Path** fWarshall;
int ammo, roomCount, chamber, key, scientist;
bool *oddLocks, *evenLocks;
vector<int> ammoAmount;


void initializeFWarshall() {
  fWarshall = (Path**) malloc((roomCount+1)*sizeof(Path*));
  for (int i = 0; i < roomCount+1; i++) {
    fWarshall[i] = (Path*) malloc((roomCount+1)*sizeof(Path)); //room nr start from 0
    for (int j = 0; j < roomCount+1; j++) {
      fWarshall[i][j] = Path(INF,INF);
    }
  }
}

void printFWarshall() {
  for (int i = 0; i < roomCount+1; i++) {
    for (int j = 0; j < roomCount+1; j++) {
      if (i == 0) cout << j;
      else if (j == 0) cout << i;
      else {
        cout << fWarshall[i][j];
      }
      cout << ", ";
    }
    cout << endl;
  }
}

bool isLocked(int index, int time) {
  if ((time+1)%2 == 0) { //even
    return evenLocks[index];
  }
  else
    return oddLocks[index];
}

void addPath(int from, int to, int weight) {
  if (!(from == 1 && isLocked(to, 1))) {
      fWarshall[from][to] = Path(weight,INF);
      fWarshall[from][to].oddNodes.push_back(from);
  }
}

void fillEdgeInfo() {
  int edgeCount;
  inpFile >> edgeCount;

  for (int i = 0; i < edgeCount; i++) {
    int from, to, weight;
    inpFile >> from;
    inpFile >> to;
    inpFile >> weight;
    addPath(from, to, weight);
    addPath(to, from, weight);
  }
}

void fillLockedRoomInfo() {
  int oddNr, evenNr, tmp;

  evenLocks = (bool*) malloc((roomCount+1) * sizeof(bool));
  oddLocks = (bool*) malloc((roomCount+1) * sizeof(bool));

  for(int i = 1; i < roomCount + 1; i++) {
    oddLocks[i] = false;
    evenLocks[i] = false;
  }

  inpFile >> oddNr;
  for(int i = 0; i < oddNr; i++) {
    inpFile >> tmp;
    oddLocks[tmp] = true;
  }

  inpFile >> evenNr;
  for(int i = 0; i < evenNr; i++) {
    inpFile >> tmp;
    evenLocks[tmp] = true;
  }
}


void printResultingPath(int from, int to) {
  cout << endl;
  Path p = fWarshall[from][to];
  cout << "FROM  " << from << " TO  " << to << endl;

  cout << "odd distance  " << p.oddDistance  << endl;

  for (size_t i = 0; i < p.oddNodes.size(); i++) {
    cout << p.oddNodes[i] << " -> ";
  }
  cout << endl;

  cout << "even distance  " << p.evenDistance << endl;
  for (size_t i = 0; i < p.evenNodes.size(); i++) {
    cout << p.evenNodes[i] << " -> ";
  }
  cout << endl;

  cout << "alternative odd distance  " << p.alternativeOddDistance  << endl;

  for (size_t i = 0; i < p.alternativeOddNodes.size(); i++) {
    cout << p.alternativeOddNodes[i] << " -> ";
  }
  cout << endl;

  cout << "alternative even distance  " << p.alternativeEvenDistance << endl;
  for (size_t i = 0; i < p.alternativeEvenNodes.size(); i++) {
    cout << p.alternativeEvenNodes[i] << " -> ";
  }

  cout << endl;
}

void loop() {
  for (int k = 1; k < roomCount+1; k++) {
    for (int i = 1; i < roomCount+1; i++) {
      for (int j = 1; j < roomCount+1; j++) {
        Path *first = &(fWarshall[i][k]);
        Path *sec = &(fWarshall[k][j]);
        Path *target = &(fWarshall[i][j]);
        /*
        cout << "i " << i << " k " << k << " j " << j << endl;
        printResultingPath(i, k);
        printResultingPath(k, j);
        */
      if (first->oddDistance + sec->oddDistance < target->evenDistance) {
        target->evenDistance = first->oddDistance + sec->oddDistance;

        target->evenNodes.clear();
        target->evenNodes.insert(target->evenNodes.end(), first->oddNodes.begin(), first->oddNodes.end());
        target->evenNodes.insert(target->evenNodes.end(), sec->oddNodes.begin(), sec->oddNodes.end());

        //cout << "Updating target as " << fWarshall[i][j] << endl;
        //printResultingPath(i, j);
      }

      if (first->evenDistance + sec->evenDistance < target->evenDistance) {
        target->evenDistance = first->evenDistance + sec->evenDistance;

        target->evenNodes.clear();
        target->evenNodes.insert(target->evenNodes.end(), first->evenNodes.begin(), first->evenNodes.end());
        target->evenNodes.insert(target->evenNodes.end(), sec->evenNodes.begin(), sec->evenNodes.end());

        //cout << "Updating target as " << fWarshall[i][j] << endl;
        //printResultingPath(i, j);
      }

      if (first->oddDistance + sec->evenDistance < target->oddDistance) {
        target->oddDistance = first->oddDistance + sec->evenDistance;

        target->oddNodes.clear();
        target->oddNodes.insert(target->oddNodes.end(), first->oddNodes.begin(), first->oddNodes.end());
        target->oddNodes.insert(target->oddNodes.end(), sec->evenNodes.begin(), sec->evenNodes.end());

        //cout << "Updating target as " << fWarshall[i][j] << endl;
        //printResultingPath(i, j);
      }

      if (sec->oddDistance + first->evenDistance < target->oddDistance) {
            target->oddDistance = sec->oddDistance + first->evenDistance;

            target->oddNodes.clear();
            target->oddNodes.insert(target->oddNodes.end(), first->evenNodes.begin(), first->evenNodes.end());
            target->oddNodes.insert(target->oddNodes.end(), sec->oddNodes.begin(), sec->oddNodes.end());

            //cout << "Updating target as " << fWarshall[i][j] << endl;
            //printResultingPath(i, j);
          }

      //printFWarshall();
      //cout << endl;
      }
    }
  }
}


bool isPathValid(vector<int> p, int ok, int ks, int sc) {
  for (size_t i = 1; i < p.size() + 1; i++) {
    if (isLocked(p[i], i)) {
      //cout << "path is not valid " << endl;
      //cout << p[i] << " is locked at " << i+1 <<endl;
      return false;
    }
  }
  int ints[] = {ok, ks, sc};
  int countOfAlternativeEven = std::count (ints, ints+3, 2);
  int countOfAlternativeOdd = std::count (ints, ints+3, 3);

  if (countOfAlternativeEven + countOfAlternativeOdd > 1)
    return false;

  return true;
}

void fillAmmoInfo() {
  int tmpNr, tmpAmo;
  int ammoRooms;
  inpFile >> ammoRooms;
  for (int i = 1; i < roomCount+1; i++) {
    ammoAmount.push_back(0);
  }
  for (int i = 0; i < ammoRooms; i++) {
    inpFile >> tmpNr;
    inpFile >> tmpAmo;
    ammoAmount[tmpNr] = tmpAmo;
  }
  /*
  cout << "ammo amount: " << endl;
  for (size_t i = 0; i < ammoAmount.size(); i++) {
    cout << ammoAmount[i] << " ";
  }
  cout << endl;
  cout << "--------------" << endl;
  */
}

void applyAmmo() {
  for (int i = 1; i < roomCount+1; i++) {
    if (ammoAmount[i] != 0) {
      for (int j = 1; j < roomCount+1; j++) {
        if (fWarshall[j][i].oddDistance != INF) {
          fWarshall[j][i].alternativeOddDistance = fWarshall[j][i].oddDistance - ammoAmount[i];
          fWarshall[j][i].alternativeOddNodes = fWarshall[j][i].oddNodes;
        }
        if (fWarshall[j][i].evenDistance != INF) {
          fWarshall[j][i].alternativeEvenDistance = fWarshall[j][i].evenDistance - ammoAmount[i];
          fWarshall[j][i].alternativeEvenNodes = fWarshall[j][i].evenNodes;
        }
      }
    }
  }
}

void updateDistances(int start, int i, int end) {
  Path *first = &(fWarshall[start][i]);
  Path *sec = &(fWarshall[i][end]);
  Path *target = &(fWarshall[start][end]);

  //cout << start << " " << i << " " << end << endl;

  //printResultingPath(start, i);
  //printResultingPath(i, end);


  if (first->alternativeOddDistance + sec->oddDistance < target->alternativeEvenDistance) {
    //cout << first->alternativeOddDistance << " + " << sec->oddDistance << " < " << target->alternativeEvenDistance << endl;

    target->alternativeEvenDistance = first->alternativeOddDistance + sec->oddDistance;

    target->alternativeEvenNodes.clear();
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), first->alternativeOddNodes.begin(), first->alternativeOddNodes.end());
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), sec->oddNodes.begin(), sec->oddNodes.end());

    //printResultingPath(start, end);
    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  if (first->alternativeEvenDistance + sec->evenDistance < target->alternativeEvenDistance) {
    //cout << first->alternativeEvenDistance << " + " << sec->evenDistance << " < " << target->alternativeEvenDistance << endl;

    target->alternativeEvenDistance = first->alternativeEvenDistance + sec->evenDistance;

    target->alternativeEvenNodes.clear();
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), first->alternativeEvenNodes.begin(), first->alternativeEvenNodes.end());
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), sec->evenNodes.begin(), sec->evenNodes.end());

    //printResultingPath(start, end);
    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  if (first->alternativeOddDistance + sec->evenDistance < target->alternativeOddDistance) {
    //cout << first->alternativeOddDistance << " + " << sec->evenDistance << " < " << target->alternativeOddDistance << endl;

    target->alternativeOddDistance = first->alternativeOddDistance + sec->evenDistance;

    target->alternativeOddNodes.clear();
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), first->alternativeOddNodes.begin(), first->alternativeOddNodes.end());
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), sec->evenNodes.begin(), sec->evenNodes.end());

    //printResultingPath(start, end);
    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  if (sec->oddDistance + first->alternativeEvenDistance < target->alternativeOddDistance) {
    //cout << first->alternativeEvenDistance << " + " << sec->oddDistance << " < " << target->alternativeOddDistance << endl;

    target->alternativeOddDistance = sec->oddDistance + first->alternativeEvenDistance;

    target->alternativeOddNodes.clear();
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), first->alternativeEvenNodes.begin(), first->alternativeEvenNodes.end());
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), sec->oddNodes.begin(), sec->oddNodes.end());

    //printResultingPath(start, end);
    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }
}

void updateDistancesForMoreAmmos(int start, int i, int end, int oldi) {
  Path *first = &(fWarshall[start][oldi]);
  Path *sec = &(fWarshall[oldi][i]);
  Path *target = &(fWarshall[start][i]);

  //cout << start << " " << i << " " << end << endl;
  if (first->alternativeOddDistance + sec->alternativeOddDistance < target->alternativeEvenDistance) {
    //cout << first->alternativeOddDistance << " + " << sec->alternativeOddDistance << " < " << target->alternativeEvenDistance << endl;

    target->alternativeEvenDistance = first->alternativeOddDistance + sec->alternativeOddDistance;

    target->alternativeEvenNodes.clear();
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), first->alternativeOddNodes.begin(), first->alternativeOddNodes.end());
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), sec->alternativeOddNodes.begin(), sec->alternativeOddNodes.end());

    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  if (first->alternativeEvenDistance + sec->alternativeEvenDistance < target->alternativeEvenDistance) {
    //cout << first->alternativeEvenDistance << " + " << sec->alternativeEvenDistance << " < " << target->alternativeEvenDistance << endl;

    target->alternativeEvenDistance = first->alternativeEvenDistance + sec->alternativeEvenDistance;

    target->alternativeEvenNodes.clear();
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), first->alternativeEvenNodes.begin(), first->alternativeEvenNodes.end());
    target->alternativeEvenNodes.insert(target->alternativeEvenNodes.end(), sec->alternativeEvenNodes.begin(), sec->alternativeEvenNodes.end());

    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  if (first->alternativeOddDistance + sec->alternativeEvenDistance < target->alternativeOddDistance) {
    //cout << first->alternativeOddDistance << " + " << sec->alternativeEvenDistance << " < " << target->alternativeOddDistance << endl;

    target->alternativeOddDistance = first->alternativeOddDistance + sec->alternativeEvenDistance;

    target->alternativeOddNodes.clear();
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), first->alternativeOddNodes.begin(), first->alternativeOddNodes.end());
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), sec->alternativeEvenNodes.begin(), sec->alternativeEvenNodes.end());

    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  if (sec->alternativeOddDistance + first->alternativeEvenDistance < target->alternativeOddDistance) {
    //cout << first->alternativeEvenDistance << " + " << sec->alternativeOddDistance << " < " << target->alternativeOddDistance << endl;

    target->alternativeOddDistance = sec->alternativeOddDistance + first->alternativeEvenDistance;

    target->alternativeOddNodes.clear();
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), first->alternativeEvenNodes.begin(), first->alternativeEvenNodes.end());
    target->alternativeOddNodes.insert(target->alternativeOddNodes.end(), sec->alternativeOddNodes.begin(), sec->alternativeOddNodes.end());

    //cout << "Updating target as " << fWarshall[start][end] << endl;
  }

  updateDistances(start, i, end);
}

void setAlternativeDistances() {
  int oldi = -1;
  for (int i = 1; i < roomCount+1; i++) {
    if (ammoAmount[i] != 0) {
      //cout << "for room number " << i << endl;
      if (oldi == -1) {
        updateDistances(1, i, key);
        updateDistances(key, i, scientist);
        updateDistances(scientist, i, chamber);
      }
      else {
        updateDistancesForMoreAmmos(1, i, key, oldi);
        updateDistancesForMoreAmmos(key, i, scientist, oldi);
        updateDistancesForMoreAmmos(scientist, i, chamber, oldi);
      }
      oldi = i;
    }
  }

}

int main() {
  inpFile.open("the3.inp");
  //inpFile.open("the3_cycle.inp");
  //inpFile.open("the3_ammo.inp");
  //inpFile.open("the3_ammo2.inp");
  //inpFile.open("the3_ammo3.inp");

  //std::ofstream ofs;
  //ofs.open("out.csv", std::ofstream::out | std::ofstream::trunc);

  inpFile >> ammo;
  inpFile >> roomCount;

  initializeFWarshall();
  inpFile >> chamber;
  inpFile >> key;
  inpFile >> scientist;

  fillLockedRoomInfo();
  fillEdgeInfo();
  fillAmmoInfo();

  //cout << "before" << endl;
  //printFWarshall();

  loop();
  //cout << endl << "after loop" << endl;
  //cout << endl;
  //printFWarshall();

  applyAmmo();
  //cout << "after applying ammo" << endl;
  //printFWarshall();
  //cout << endl;

  setAlternativeDistances();
  //cout << endl << "after setting distances" << endl;
  //printFWarshall();

  /*
  printResultingPath(1, key);
  printResultingPath(key, scientist);
  printResultingPath(scientist, chamber);
  */
  Path OK = fWarshall[1][key];
  Path KS = fWarshall[key][scientist];
  Path SC = fWarshall[scientist][chamber];

  int minDistance, minTime;
  vector<int> minNodes;
  minDistance = INF;


  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        int distance;
        vector<int> nodes;
        if (i == 0) {
          nodes.insert(nodes.end(), OK.evenNodes.begin(), OK.evenNodes.end());
          distance = OK.evenDistance;
        }
        else if (i == 1) {
          nodes.insert(nodes.end(), OK.oddNodes.begin(), OK.oddNodes.end());
          distance = OK.oddDistance;
        }
        else if (i == 2) {
          nodes.insert(nodes.end(), OK.alternativeEvenNodes.begin(), OK.alternativeEvenNodes.end());
          distance = OK.alternativeEvenDistance;
        }
        else {
          nodes.insert(nodes.end(), OK.alternativeOddNodes.begin(), OK.alternativeOddNodes.end());
          distance = OK.alternativeOddDistance;

        }

        if (j == 0) {
          nodes.insert(nodes.end(), KS.evenNodes.begin(), KS.evenNodes.end());
          distance += KS.evenDistance;
        }
        else if (j == 1) {
          nodes.insert(nodes.end(), KS.oddNodes.begin(), KS.oddNodes.end());
          distance += KS.oddDistance;
        }
        else if (j == 2) {
          nodes.insert(nodes.end(), KS.alternativeEvenNodes.begin(), KS.alternativeEvenNodes.end());
          distance += KS.alternativeEvenDistance;
        }
        else {
          nodes.insert(nodes.end(), KS.alternativeOddNodes.begin(), KS.alternativeOddNodes.end());
          distance += KS.alternativeOddDistance;
        }

        if (k == 0) {
          nodes.insert(nodes.end(), SC.evenNodes.begin(), SC.evenNodes.end());
          distance += SC.evenDistance;
        }
        else if (k == 1) {
          nodes.insert(nodes.end(), SC.oddNodes.begin(), SC.oddNodes.end());
          distance += SC.oddDistance;
        }
        else if (k == 2) {
          nodes.insert(nodes.end(), SC.alternativeEvenNodes.begin(), SC.alternativeEvenNodes.end());
          distance += SC.alternativeEvenDistance;
        }
        else {
          nodes.insert(nodes.end(), SC.alternativeOddNodes.begin(), SC.alternativeOddNodes.end());
          distance += SC.alternativeOddDistance;
        }

        /*
        for (size_t i = 0; i < nodes.size(); i++) {
          cout << nodes.at(i) << " ";
        }
        cout << endl;
        */

        if(!isPathValid(nodes,i, j, k)) {
          continue;
        }

        if (ammo - distance > ammo - minDistance) {
          minDistance = distance;
          minNodes.clear();
          minNodes.insert(minNodes.end(), nodes.begin(), nodes.end());
          minNodes.push_back(chamber);
          minTime = minNodes.size();
        }

      }
    }
  }

  ofstream outFile("the3.out");
  outFile << ammo - minDistance << endl;
  outFile << minTime << endl;

  for (size_t i = 0; i < minNodes.size(); i++) {
    outFile << minNodes.at(i) << " ";
  }
  outFile << endl;

  return 0;
}
