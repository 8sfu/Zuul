#ifndef ROOM_H
#define ROOM_H
#include <cstring>
#include <vector>
#include <map>
#include "item.h"

using namespace std;

class Room {
 public:
  char* name;
  vector<Item*> itemList;
  multimap<const char*, Room*> exits;
  //vector of item pointers
  //  map<int, Room> exits;
  void setName(const char* setName);
  void putItem(Item* item);
  void setExit(const char* direction, Room* exitRoom);
  void describeRoom();
};

#endif
