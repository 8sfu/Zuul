#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <iomanip>
#include "room.h"
#include "item.h"
#include "progression.h"

using namespace std;

Room* newRoom(const char* newName, vector<Room*>* roomList){ //Create a new room, name it, and return its pointer
  Room* roomPtr = new Room();
  roomPtr->name = (char*)newName;
  roomList->push_back(roomPtr);
  return roomPtr;
}

Item* newItem(const char* newName, double newWeight){ //Create a new item, name it, and return its pointer
  Item* itemPtr = new Item();
  itemPtr->name = (char*)newName;
  itemPtr->weight = newWeight;
  return itemPtr;
} 


void help(vector<Item*>* inventory){ //Print out all possible commands
  cout <<"h - See commands list." << endl;
  cout << "q - Quit game." << endl;
  cout << "r - Restart game." << endl;
  cout << "d - Describe the current room." << endl;
  cout << "e - List current inventory." << endl;
  cout << "i - Progression hint." << endl;
  cout << "go - Access movement prompt." << endl;
  cout << "grab - Access item pickup prompt." << endl;
  cout << "drop - Access item drop prompt." << endl;
  for(Item* item : *inventory){
    if(!strcmp(item->name,(char*)"wrench")){
      cout << "w - Fix with wrench" << endl;
    } else if (!strcmp(item->name,(char*)"cat food")){
      cout << "w - Feed the cat" << endl;
    }
  }
  cout << endl;
  return;
}

bool cmp(char* input, const char* check){ //Simplified string comparison function
  if(!strcmp(input,check)){
    return true; 
  } else {
    return false;
  }
}

void listInv(vector<Item*>* inventory,double* totalWeight){ //NEED TO PRINT TOTAL WEIGHT
  if(inventory->size()){
    cout << "You have " << endl;
    for(Item* item : *inventory){
      cout << "   " << item->name << " (Weight:" << item->weight << "kg)";
    }
    cout << ".   " << endl;
    cout << "Your inventory is " << *totalWeight << "kg. It is " << setprecision(2) << *totalWeight/.8 << "% full." << endl << endl;
  } else {
    cout << "There is nothing in your inventory." << endl << endl;
  }
}


Room* move(Room* currentRoom, const char* moveName){
  for(pair<const char*, Room*> exit : currentRoom->exits){
    if(!strcmp(exit.second->name,moveName)){
      exit.second->describeRoom();
      return exit.second;
    }
  }
  cout << "You can't go there." << endl << endl;
  return currentRoom;
}

void grab(Room* currentRoom, const char* itemName, vector<Item*>* inventory, double* invWeight, Progression* progression){
  vector<Item*>::iterator it;
  for(it = currentRoom->itemList.begin(); it != currentRoom->itemList.end(); ++it){
    if(!strcmp((*it)->name,itemName)){
      cout << "You have grabbed " << itemName << " (Weight: " << (*it)->weight << "kg)." << endl << endl;
      if(!strcmp(itemName,"wrench")){
	  progression->hasWrench = true;
	}
      *invWeight += (*it)->weight;
      inventory->push_back(*it);
      currentRoom->itemList.erase(it);
      --it;
      return;
    }
  }
  cout << "You can't pick that up." << endl << endl;
  return;
}

void drop(Room* currentRoom, const char* itemName, vector<Item*>* inventory, double* invWeight){
  vector<Item*>::iterator it;
  for(it = inventory->begin(); it != inventory->end(); ++it){
    if(!strcmp((*it)->name,itemName)){
      cout << "You have dropped " << itemName << " (Weight: " <<(*it)->weight << "kg)." << endl << endl;
      *invWeight -= (*it)->weight;
      currentRoom->putItem((*it));
      inventory->erase(it);
      --it;
      return;
    }
  }
  cout << "You can't drop that." << endl << endl;
  return;
}

void help(Progression* progression){
  if(progression->hasWrench){
    cout << "Take the elevator to the bunker." << endl << endl;
  } else {
    cout << "Fetch the wrench from the supply closet." << endl << endl;
  }
}

int main() {
  cout << endl << "Welcome to Zuul" << endl << endl;
  vector<Item*>* inventory = new vector<Item*>();
  double* invWeight = new double(0);

  Item* wrench = newItem("wrench",0.5);  //Item setup
  Item* backpack = newItem("backpack",1.5);
  Item* cat = newItem("cat",3.5);
  Item* debug = newItem("debug",30);
  
  vector<Room*>* roomList = new vector<Room*>(); //Room setup (and naming)
  Room* bedroom1 = newRoom("bedroom (1)",roomList);
  Room* bedroom2 = newRoom("bedroom (2)",roomList);
  Room* bedroom3 = newRoom("bedroom (3)",roomList);
  Room* restroom1 = newRoom("small restroom",roomList);
  Room* restroom2 = newRoom("restroom",roomList);
  Room* theater = newRoom("home theater",roomList);
  Room* garage = newRoom("garage",roomList);
  Room* supplyCloset = newRoom("supply closet",roomList);
  Room* laundryRoom = newRoom("laundry room",roomList);
  Room* hallway = newRoom("hallway",roomList);
  Room* porch = newRoom("porch",roomList);
  Room* kitchen = newRoom("kitchen",roomList); 
  Room* pantry = newRoom("pantry",roomList);
  Room* livingRoom = newRoom("living room",roomList);
  Room* foyer = newRoom("foyer",roomList);
  Room* elevator = newRoom("elevator",roomList);
  Room* diningRoom = newRoom("dining room",roomList);
  Room* office = newRoom("office",roomList);
  Room* master = newRoom("master bedroom",roomList);
  Room* masterBath = newRoom("bathroom",roomList);
  Room* masterCloset = newRoom("closet",roomList);

  bedroom1->setExit("south",hallway); //Room exit setup
  bedroom2->setExit("east",hallway);
  bedroom3->setExit("east",hallway);
  restroom1->setExit("west",hallway);
  restroom2->setExit("east",hallway);
  theater->setExit("east",hallway);
  garage->setExit("north",hallway);
  garage->setExit("west",supplyCloset);
  garage->setExit("east",laundryRoom);
  supplyCloset->setExit("east",garage);
  laundryRoom->setExit("west",garage);
  laundryRoom->setExit("north",foyer);
  hallway->setExit("north",bedroom1);
  hallway->setExit("west",bedroom2);
  hallway->setExit("west",bedroom3);
  hallway->setExit("east",restroom1);
  hallway->setExit("west",restroom2);
  hallway->setExit("west",theater);
  hallway->setExit("south",garage);
  hallway->setExit("west",porch);
  hallway->setExit("west",foyer);
  porch->setExit("east",hallway);
  porch->setExit("south",kitchen);
  porch->setExit("south",livingRoom);
  kitchen->setExit("north",porch);
  kitchen->setExit("east",pantry);
  kitchen->setExit("west",livingRoom);
  kitchen->setExit("south",foyer);
  pantry->setExit("west",kitchen);
  livingRoom->setExit("north",porch);
  livingRoom->setExit("east",kitchen);
  livingRoom->setExit("south",foyer);
  foyer->setExit("east",hallway);
  foyer->setExit("north",kitchen);
  foyer->setExit("south",laundryRoom);
  foyer->setExit("south",elevator);
  foyer->setExit("north",livingRoom);
  foyer->setExit("south",diningRoom);
  foyer->setExit("west",master);
  diningRoom->setExit("north",foyer);
  office->setExit("north",foyer);
  master->setExit("east",foyer);
  master->setExit("south",masterBath);
  master->setExit("south",masterCloset);
  masterBath->setExit("north",master);
  masterCloset->setExit("north",master);
  
  supplyCloset->putItem(wrench);
  office->putItem(backpack);
  bedroom1->putItem(cat);
  foyer->putItem(debug);
  
  bool running = true;
  bool restart = false;
  char* input = new char[20];
  cout << "Enter h for a list of commands." << endl;
  
  do {
    cout << endl;
    restart = false;
    Room* currentRoom = foyer;
    currentRoom->describeRoom();
    Progression* progression = new Progression();
    while(running == true && restart == false){
      cin.getline(input,20);
      if(cmp(input,"h")){
	help(inventory);
      } else if (cmp(input,"q")) {
	running = false;
      } else if (cmp(input,"r")) {
	cout << "Are you sure?" << endl;
	cin.getline(input,20);
	if(cmp(input,"y")){
	  restart = true;
	}
      } else if (cmp(input,"d")) {
	currentRoom->describeRoom();
      } else if (cmp(input,"e")) {
	listInv(inventory, invWeight);
      } else if (cmp(input,"go")) {
	cout << "Where do you want to go?" << endl;
	cin.getline(input,20);
	currentRoom = move(currentRoom,input);
      } else if (cmp(input,"grab")) {
	cout << "What do you want to grab?" << endl;
	cin.getline(input,20);
	grab(currentRoom,input,inventory,invWeight,progression);
      } else if (cmp(input,"drop")) {
	cout << "What do you want to drop?" << endl;
	cin.getline(input,20);
	drop(currentRoom,input,inventory,invWeight);
      } else if (cmp(input,"i")) {
	help(progression);
      }
      if(currentRoom == elevator){
	running = false;
	cout << "You win!" << endl;
      }
    }
  } while(restart == true);
  cout << endl;
  return 0;
}

