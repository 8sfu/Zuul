#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "room.h"
#include "item.h"

using namespace std;

int main() {

  cout << "Welcome to Zuul" << endl;
  Room currentRoom;

  Room kitchen;
  kitchen.setName("kitchen");
  cout << "The room is named: " << kitchen.name << endl;
  
  kitchen.initItem("shovel");
  for(Item* iptr : kitchen.itemList){
    // tool->name = (char*)"spork";
    cout << iptr << endl;
    cout << "There is a " << iptr->name << " in the " << kitchen.name << endl;
  }

  currentRoom = kitchen;






  return 0;
}
