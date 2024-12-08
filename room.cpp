#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "room.h"

using namespace std;

void Room::defineRoom(){
  cout << "name the room!" << endl;
  cin.getline(name,20);
  return;
}
