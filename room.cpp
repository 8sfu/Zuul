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

void Room::initItem(const char* itemName){
  Item item;
  item.name = (char*)itemName;
  cout << itemName << endl;
  Item* iptr = &item;
  cout << iptr << endl;
  cout << iptr->name << endl;
  itemList.push_back(iptr);
  return;
}
