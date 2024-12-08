#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "room.h"

using namespace std;

int main() {

  cout << "Welcome to Zuul" << endl;
  Room kitchen;
  kitchen.defineRoom();
  cout << "The room is named: " << kitchen.name << endl;
  return 0;
}
