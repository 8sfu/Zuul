#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "room.h"

using namespace std;

void Room::setName(const char* setName){
  name = (char*)setName;
  return;
}

void Room::putItem(Item* item){
  itemList.push_back(item);
  return;
}

void Room::setExit(const char* direction, Room* exitRoom){
  exits.insert(pair<const char*, Room*>(direction, exitRoom));
  return;
}


void Room::describeRoom(){
  cout << "You are in the " << name << "." << endl;
  
  for(Item* item : itemList){
    cout << "There is a " << item->name << " (weight: " << item->weight << "kg) here." << endl;
  }

  for(pair<const char*, Room*> exit : exits) {
    cout << "There is a " << exit.second->name << " to the " << exit.first << "." << endl;
  }
  cout << endl;
  return;
}

