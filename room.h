#ifndef ROOM_H
#define ROOM_H
#include <cstring>
#include <vector>
#include <map>

using namespace std;

class Room {
 public:
  char* name;
  //vector of item pointers
  //  map<int, Room> exits;
  void defineRoom();
};

#endif
