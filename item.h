#ifndef ITEM_H
#define ITEM_H
#include <cstring>

using namespace std;

class Item {
 public:
  char* name = new char[20];
  double weight;
};

#endif
