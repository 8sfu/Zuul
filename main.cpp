#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <iomanip>
#include "room.h"
#include "item.h"
#include "cat.h"
#include "progression.h"

using namespace std;

Room* newRoom(const char* newName, vector<Room*>* roomList){ //Create a new room, name it, and return its pointer
  Room* roomPtr = new Room();
  roomPtr->name = (char*)newName;
  roomList->push_back(roomPtr);
  return roomPtr;
}

Item* newItem(const char* newName, double newWeight){ //Create a new cat, name it, and return its pointer
  Item* itemPtr = new Item();
  itemPtr->name = (char*)newName;
  itemPtr->weight = newWeight;
  return itemPtr;
} 


Cat* newCat(const char* newName, double newWeight){ //Create a new item, name it, and return its pointer
  Cat* catPtr = new Cat();
  catPtr->name = (char*)newName;
  catPtr->weight = newWeight;
  return catPtr;
}

void help(vector<Item*>* inventory){ //Print out all possible commands
  cout << "h - See commands list." << endl;
  cout << "q - Quit game." << endl;
  cout << "r - Restart game." << endl;
  cout << "y - Yes." << endl;
  cout << "n - No." << endl;
  cout << "d - Describe the current room." << endl;
  cout << "e - List current inventory." << endl;
  cout << "i - Progression hint." << endl;
  cout << "go - Access movement prompt." << endl;
  cout << "grab - Access item pickup prompt." << endl;
  cout << "drop - Access item drop prompt." << endl;
  for(Item* item : *inventory) {
    if(!strcmp(item->name,(char*)"wrench")){
      cout << "w - Fix with wrench" << endl;
    }
    if (!strcmp(item->name,(char*)"cat food")){
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

void listInv(vector<Item*>* inventory,double* totalWeight){ //Print out item and weight info of the inventory
  if(inventory->size()){
    cout << "You have " << endl;
    for(Item* item : *inventory){
      cout << "   " << item->name << " (Weight:" << item->weight << "kg)";
    }
    cout << ".   " << endl << endl;
    cout << "Your inventory is " << *totalWeight << "kg. It is " << setprecision(2) << *totalWeight/.8 << "% full." << endl << endl;
  } else {
    cout << "There is nothing in your inventory." << endl << endl;
  }
}


Room* move(Room* currentRoom, const char* moveName, Progression* progression){ //Move to an adjacent room
  for(pair<const char*, Room*> exit : currentRoom->exits){
    if(!strcmp(exit.second->name,moveName)){
      if(strcmp(moveName,(char*)"elevator")){
	exit.second->describeRoom();
	return exit.second;
      } else {
	if(progression->elevatorFix){
	  exit.second->describeRoom();
	  return exit.second;
	} else {
	  cout << "The elevator hasn't been fixed yet. ";
	}
      }
    }
  }
  cout << "You can't go there." << endl << endl;
  return currentRoom;
}

void grab(Room* currentRoom, const char* itemName, vector<Item*>* inventory, double* invWeight, Progression* progression){ //Grab items, with special conditions for specific items
  vector<Item*>::iterator it;
  for(it = currentRoom->itemList.begin(); it != currentRoom->itemList.end(); ++it){
    if(!strcmp((*it)->name,itemName)){ //checking if the item is in the room
      if(progression->hasPack || inventory->size() == 0){	
	if((*it)->weight + *invWeight <= 80){ //checking that inventory can accomodate item.
	  if(strcmp(itemName,"freddy the maine coon")) { //different pickup conditions for the cat
	    cout << "You have grabbed " << itemName << " (Weight: " << (*it)->weight << "kg)." << endl << endl;
	    if(!strcmp(itemName,"wrench")){
	      progression->hasWrench = true;
	    }
	    if(!strcmp(itemName,"backpack")){
	      cout << "You have a backpack now. Your inventory size has increased from one item with 80kg weight to infinite items with 80kg combined weight." << endl << endl;
	      progression->hasPack = true;
	    } if(!strcmp(itemName,"cat food")){
	      progression->hasCatFood = true;
	      inventory->push_back(*it);
	    } else {
	      inventory->push_back(*it);
	    } //Add item weight to inventory
	    *invWeight += (*it)->weight;
	    currentRoom->itemList.erase(it);
	    --it;
	    return;  
	  } else { //if the item is the cat and it can be picked up
	    if(progression->fedCat){ //check if it has been fed
	      cout << "You have picked up freddy the maine coon." << endl << endl;
	      inventory->push_back(*it);
	      *invWeight += (*it)->weight;
	      progression->hasCat = true;
	      return;
	    } else { //Grab fail explanations
	      cout << "You haven't fed freddy the maine coon. You can't pick him up without contracting rabies from his scratches. Feed him some cat food from the kitchen to appease him." << endl << endl;
	      return;
	    }
	  }  
	} else {
	  cout << "You can't carry that much combined weight." << endl;
	  return;
	}
      } else {
	  cout << "You need a backpack to carry more than one item." << endl << endl;
	  return;
	}
      }
    }
    cout << "That item isn't here." << endl << endl;
    return;
}

void drop(Room* currentRoom, const char* itemName, vector<Item*>* inventory, double* invWeight){
  vector<Item*>::iterator it; //Reverse of grab, applies to everything except backpack, hence the greater simplicity
  for(it = inventory->begin(); it != inventory->end(); ++it){
    if(!strcmp((*it)->name,itemName)){
      if(strcmp((*it)->name,"backpack")){
	  cout << "You have dropped " << itemName << " (Weight: " <<(*it)->weight << "kg)." << endl << endl;
	  *invWeight -= (*it)->weight;
	  currentRoom->putItem((*it));
	  inventory->erase(it);
	  --it;
	  return;
	} else {
	  cout << "You can't drop that. That would make coding this too complicated. :)" << endl << endl;
	  return;
      }
    }
  }
  cout << "You can't drop that." << endl << endl;
  return;
}

void useTool(vector<Item*>* inventory, Progression* progression, Room* currentRoom){
  vector<Item*>::iterator it;
  for(it = inventory->begin(); it != inventory->end(); ++it){ //Fix elevator
    if(!strcmp((*it)->name,"wrench") && !strcmp(currentRoom->name,"foyer")){
      cout << "You have fixed the elevator." << endl << endl;
      progression->elevatorFix = true;
      return;
    } else if (!strcmp((*it)->name,"cat food") && !strcmp(currentRoom->name,"bedroom (1)")) { //Feed cat
      cout << "You have fed the cat." << endl << endl;
      progression->fedCat = true;
      inventory->erase(it);
      ++it;
      return;
    }
  }
  return;
}

void help(Progression* progression){ //Hints based on current stage of progression
  if(progression->fedCat){ 
    cout << "Bring freddy with you, and grab food and guns on your way to the bunker." << endl << endl;
  } else if(progression->hasCatFood){
    cout << "Check out the bedrooms." << endl;
  } else if(progression->elevatorFix){
    cout << "Grab some cat food from the kitchen." << endl << endl;
  } else if(progression->hasWrench){
    cout << "Use the wrench to fix the elevator in the foyer." << endl << endl;
  } else if(progression->hasPack){
    cout << "Fetch the wrench from the supply closet." << endl << endl;
  } else {
    cout << "Grab a backpack from the office to simplify inventory management for the rest of the game." << endl << endl;
  }
}

bool endGame(vector<Item*>* inventory){ //Return true or false depending on relevant satisfied conditions
  bool notHasCat = true;
  bool notHasGun = true;
  bool notHasRice = true;
  bool notHasBean = true;
  for(Item* item : *inventory){
    if(cmp(item->name,"freddy the maine coon")){
      notHasCat = !notHasCat;
    } else if (cmp(item->name,"chekhov's gun")){
      notHasGun = !notHasGun;
    } else if (cmp(item->name,"bag of rice")){
      notHasRice = !notHasRice;
    } else if(cmp(item->name,"bag of beans")){
      notHasBean = !notHasBean;
    }
  }
  if(notHasCat){
    cout << "You didn't bring freddy the beloved maine coon and consequently died of grief." << endl;
  } else if (notHasRice || notHasBean) {
    cout << "You didn't bring enough food, and starved." << endl;
  } else if (notHasGun) {
    cout << "You didn't bring your gun, and after your food ran out, you were forced to live out the remainder of your weeks in a state of starvation." << endl;
  }
  bool done = !(notHasCat + notHasGun + notHasRice + notHasBean); //boolean arithmetic to check that all win conditions
  return done;
}

int main() {
  cout << endl << "Welcome to Zuul" << endl << endl;
  vector<Item*>* inventory = new vector<Item*>();
  double* invWeight = new double(0);

  Item* wrench = newItem("wrench",0.86);  //Item setup
  Item* backpack = newItem("backpack",0.5);
  Cat* cat = newCat("freddy the maine coon",7.2);
  Item* catFood = newItem("cat food",0.25);
  Item* debug = newItem("debug",99.999999999999999999999999999);
  Item* gun = newItem("chekhov's gun",5);
  Item* rice = newItem("bag of rice",20);
  Item* beans = newItem("bag of beans",15);
  
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
  hallway->setExit("east",kitchen);
  hallway->setExit("east",porch);
  hallway->setExit("east",foyer);
  porch->setExit("west",hallway);
  porch->setExit("south",kitchen);
  porch->setExit("south",livingRoom);
  porch->setExit("south",master);
  kitchen->setExit("west",hallway);
  kitchen->setExit("north",porch);
  kitchen->setExit("west",pantry);
  kitchen->setExit("east",livingRoom);
  kitchen->setExit("south",foyer);
  pantry->setExit("west",kitchen);
  livingRoom->setExit("north",porch);
  livingRoom->setExit("west",kitchen);
  livingRoom->setExit("south",foyer);
  foyer->setExit("west",hallway);
  foyer->setExit("north",kitchen);
  foyer->setExit("south",laundryRoom);
  foyer->setExit("south",elevator);
  foyer->setExit("north",livingRoom);
  foyer->setExit("south",diningRoom);
  foyer->setExit("south",office);
  foyer->setExit("east",master);
  diningRoom->setExit("north",foyer);
  office->setExit("north",foyer);
  master->setExit("south",foyer);
  master->setExit("west",foyer);
  master->setExit("south",masterBath);
  master->setExit("south",masterCloset);
  masterBath->setExit("north",master);
  masterCloset->setExit("north",master);
  
  supplyCloset->putItem(wrench); //Item setup in rooms
  supplyCloset->putItem(gun);
  office->putItem(backpack);
  bedroom1->putItem(cat);
  // foyer->putItem(debug);
  kitchen->putItem(catFood);
  pantry->putItem(rice);
  pantry->putItem(beans);
  
  
  bool running = true;
  bool restart = false;
  char* input = new char[40];
  cout << "Enter h for a list of commands." << endl;
  
  do { //Main loop
    cout << endl;
    restart = false;
    Room* currentRoom = foyer;
    currentRoom->describeRoom();
    Progression* progression = new Progression();
    while(running == true && restart == false){
      cin.getline(input,40);
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
	cout << endl;
      } else if (cmp(input,"d")) {
	currentRoom->describeRoom();
      } else if (cmp(input,"e")) {
	listInv(inventory, invWeight);
      } else if (cmp(input,"go")) {
	cout << "Where do you want to go?" << endl;
	cin.getline(input,40);
	currentRoom = move(currentRoom,input,progression);
      } else if (cmp(input,"grab")) {
	cout << "What do you want to grab?" << endl;
	cin.getline(input,40);
	grab(currentRoom,input,inventory,invWeight,progression);
      } else if (cmp(input,"drop")) {
	cout << "What do you want to drop?" << endl;
	cin.getline(input,40);
	drop(currentRoom,input,inventory,invWeight);
      } else if (cmp(input,"i")) {
	help(progression);
      } else if (cmp(input,"w")) {
	useTool(inventory,progression,currentRoom);
      }
      if(currentRoom == elevator){
	if(endGame(inventory)){
	  cout << "You WIN! Thank you for playing!" << endl << endl;
	} else {
	  cout << "You LOSE! Thank you for playing!" << endl << endl;
	}
	running = false;
      }
    }
  } while(restart == true);
  cout << endl;
  return 0;
}

