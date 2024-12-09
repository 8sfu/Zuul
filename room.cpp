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

void Room::describeRoom(){
  cout << "You are in the " << name << "." << endl;
  

  for(Item* item : itemList){
    cout << "There is a " << item->name << " here." << endl;
  }
  
  for(map<const char*,Room>::iterator it = exits.begin(); it != exits.end(); ++it){
    cout << it->second << endl;
  }

  return;
}
