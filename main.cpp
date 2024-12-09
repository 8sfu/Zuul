#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "room.h"
#include "item.h"

using namespace std;

void rName(Room room, const char* newName){
  room.name = (char*)newName;
}

void iName(Item item, const char* newName){
  item.name = (char*)newName;
}

int main() {

  cout << "Welcome to Zuul" << endl;
  Room currentRoom;
  vector<Item*> inventory;

  Item shovel;
  iName(shovel,"shovel");
  Item* shovelPtr = &shovel;


  Room kitchen;
  rName(kitchen,"kitchen");
  kitchen.itemList.push_back(shovelPtr);
  currentRoom = kitchen;
  
  Room hallway;
  rName(hallway,"hallway");
  
  Room diningRoom;
  rName(diningRoom,"dining room");

  kitchen.exits["north"] = hallway;
  kitchen.exits["west"] = diningRoom;

  currentRoom.describeRoom();

  return 0;
}
