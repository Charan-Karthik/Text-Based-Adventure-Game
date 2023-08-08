// Source File Header
// CISP360-Final.cpp
// Charan Karthik, CISP 360 - Prof. Fowler
// 07/28/23

#include <cctype>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <string>

// Specification B - Additional Feature 1: Monster Class
class Monster {
public:
  std::string name;
  bool is_aggressive;
  bool is_alive;
  bool has_spawned;

  Monster(std::string monster_name, bool is_monster_aggressive = false,
          bool is_monster_alive = true, bool has_monster_spawned = false,
          std::string monster_ability = "None")
      : name(monster_name), is_aggressive(is_monster_aggressive),
        is_alive(is_monster_alive), has_spawned(has_monster_spawned),
        ability(monster_ability) {}

  std::string get_ability() const { return ability; }

  void set_ability(const std::string &monster_ability) {
    ability = monster_ability;
  }

private:
  std::string ability;
};

// Room Struct
struct Room {
  std::string room_name;
  std::string room_description;
  int room_id;
  bool occupied_code;
  // Specification B - Additional Feature 3: Additional Room Members/Fields
  char allowable_exits[4];
  bool has_item;
  bool has_been_visited;
  bool has_item_been_found;
};

// Specification B - Additional Feature 2: Collect Treasure
// Treasure Struct
struct Treasure {
  std::string name;
  bool has_been_found;
};

// Speciﬁcation C1 - Main Data Structure (Navigation Logic)
using DirectionToNewRoomMap = std::map<char, int>;
using NavigationMap = std::map<int, DirectionToNewRoomMap>;
/*
I know we didn't learn this in class, but in my head using a map was the easiest
way to organize the logic for navigating between rooms. Using my prior
programming experience in other languages, I decided to learn how to integreate
maps in C++ to solve the problem I was facing.
*/

// Function Prototypes
void ProgramGreeting();
void PrintLineBreak();
char GameMenu();
void DisplayInstructions();
char GetUserAction();
void CheckItemBox(Treasure user_treasures[], int num_treasures);
int GenerateRandomNumber(int low_range, int high_range);
void InspectRoom(int occupied_room_code, Room rooms[], bool user_looks,
                 Monster monster[], int num_monsters);
void SearchForItems(int occupied_room_code, Room rooms[], Treasure treasures[],
                    Treasure user_items[], int num_items);
bool CheckIfValidMoveDirection(Room rooms[], int occupied_room_code,
                               char user_action);
int NavigateDungeon(int occupied_room_code, const NavigationMap &navMap,
                    char user_action);

int main() {
  // Speciﬁcation C1 - Main Data Structure (Room Information Storage)
  Room ADVENTURE_ROOMS[7] = {
      // Room 1
      {"EMERALD ENTRYWAY",
       "A room shimmering in green light from the Emerald walls, floors, and "
       "ceilings. There's a mosaic of Rayquaza on the ceiling.",
       0,
       true,
       {'N', 'E', 0, 'W'},
       false,
       false,
       false},
      // Room 2
      {"SAPPHIRE SEA",
       "A room filled with Sapphire-colored water. The platform you are "
       "standing "
       "on is a statue of Kyogre. You think you see something in the water...",
       1,
       false,
       {0, 'E', 0, 0},
       true,
       false,
       false},
      // Room 3
      {"RUBY REALM",
       "A room of Ruby-colored rock with lava falling from the ceiling through "
       "the floor. Against the far wall is a statue of Groudon. There seems to "
       "be something by Groudon's foot...",
       2,
       false,
       {0, 0, 0, 'W'},
       true,
       false,
       false},
      // Room 4
      {"AMETHYST ARC",
       "A room clad in a stunning violet color with an arc of Amethyst in the "
       "center. Atop the arc is a statue of Mewtwo. Mewtwo seems to be "
       "holding something...",
       3,
       false,
       {0, 'E', 'S', 'W'},
       false,
       false,
       false},
      // Room 5
      {"STONE SANCTUARY",
       "A chilly stone room.",
       4,
       true,
       {0, 'E', 0, 0},
       false,
       false,
       false},
      // Specification B - Additional Feature 4: >5 Rooms
      // Room 6
      {"DESOLATE DESERT",
       "A scorching hot desert filled with nothing but sand.",
       5,
       true,
       {'N', 0, 0, 'W'},
       false,
       false,
       false},
      // Room 7
      {"CRYSTAL CAVE",
       "A beautiful, sparkling room with a cool breeze and relaxing scent. "
       "There's one diamond pillar in the center of the room. There seems to "
       "be something in the pillar...",
       6,
       false,
       {'N', 0, 'S', 0},
       true,
       false,
       false}};

  // Speciﬁcation C1 - Main Data Structure (Navigation Logic)
  const NavigationMap NAVIGATION_LOGIC = {{0, {{'N', 3}, {'E', 2}, {'W', 1}}},
                                          {1, {{'E', 0}}},
                                          {2, {{'W', 0}}},
                                          {3, {{'E', 5}, {'S', 0}, {'W', 4}}},
                                          {4, {{'E', 3}}},
                                          {5, {{'W', 3}, {'N', 6}}},
                                          {6, {{'S', 5}, {'N', 7}}}};

  // Specification B - Additional Feature 2: Collect Treasure
  Treasure ADVENTURE_TREASURES[3] = {{"Sapphire Ring", false},
                                     {"Ruby Necklace", false},
                                     {"Diamond Sword", false}};
  const int NUM_TREASURES = 3;
  Treasure user_item_box[3] = {
      {"temp", false}, {"temp", false}, {"temp", false}};

  Monster ADVENTURE_MONSTERS[6] = {
      Monster("NINE-TAILED FOX", true, false, false, "Tailed Beast Bomb"),
      Monster("POISONOUS SERPENT", true, false, false),
      Monster("CYBORG PANDA", false, true, false),
      Monster("WISE DRAGON", false, true, false, "Breathes Fire"),
      // Specification B - Additional Feature 5: >4 Monsters
      Monster("GIANT PUPPY", false, true, false),
      Monster("SHADOW WOLF", false, false, false, "Shadow Realm Travel")};
  const int NUM_MONSTERS = 6;

  PrintLineBreak();
  ProgramGreeting();
  PrintLineBreak();

  char user_choice;
  do {
    user_choice = GameMenu();

    switch (user_choice) {
    case 'A':
      std::cout << "\nStarting the game..." << std::endl;
      PrintLineBreak();
      break;
    case 'B':
      PrintLineBreak();
      DisplayInstructions();
      PrintLineBreak();
      break;
    case 'C':
      std::cout << "\nExiting game..." << std::endl;
      return 0;
    default:
      std::cout << "\nAn unexpected error occurred." << std::endl;
      return 1;
    }
  } while (user_choice == 'B');

  int currently_occupied_room_id = 0;
  bool has_user_looked = false;

  /*
  There are 7 rooms in the game. Each room's room_id is its spot in the array,
  so it'll always be room number - 1. If the currently_occupied_room_id is ever
  7, that means the user has exited the dungeon and completed their adventure.
  */
  while (currently_occupied_room_id != 7) {
    if (!has_user_looked) {
      InspectRoom(currently_occupied_room_id, ADVENTURE_ROOMS, false,
                  ADVENTURE_MONSTERS, NUM_MONSTERS);
    } else {
      has_user_looked = false;
    }

    char user_action = GetUserAction();

    if (user_action == 'L') {
      PrintLineBreak();
      InspectRoom(currently_occupied_room_id, ADVENTURE_ROOMS, true,
                  ADVENTURE_MONSTERS, NUM_MONSTERS);
      PrintLineBreak();
      has_user_looked = true;
      continue;
    } else if (user_action == 'D') {
      PrintLineBreak();
      DisplayInstructions();
      PrintLineBreak();
      continue;
    } else if (user_action == 'O') {
      PrintLineBreak();
      SearchForItems(currently_occupied_room_id, ADVENTURE_ROOMS,
                     ADVENTURE_TREASURES, user_item_box, NUM_TREASURES);
      PrintLineBreak();
      continue;
    } else if (user_action == 'I') {
      PrintLineBreak();
      CheckItemBox(user_item_box, NUM_TREASURES);
      PrintLineBreak();
    } else if (user_action == 'N' || user_action == 'E' || user_action == 'S' ||
               user_action == 'W') {
      bool is_valid_move = CheckIfValidMoveDirection(
          ADVENTURE_ROOMS, currently_occupied_room_id, user_action);

      if (is_valid_move) {
        PrintLineBreak();
        currently_occupied_room_id = NavigateDungeon(
            currently_occupied_room_id, NAVIGATION_LOGIC, user_action);
        PrintLineBreak();
      } else {
        PrintLineBreak();
        std::cout << "\nInvalid move." << std::endl;
        PrintLineBreak();
      }
    } else { // In theory, this should never execute because of the error
             // handling in the 'GetUserAction()' function.
      PrintLineBreak();
      std::cout << "Something unexpected happened... Restarting from last save "
                   "point...";
      PrintLineBreak();
      continue;
    }
  }

  PrintLineBreak();
  std::cout << "\nCongratulations! You have completed your adventure!"
            << std::endl;
  std::cout << "During your adventure, you found: ";
  for (const Treasure &treasure : user_item_box) {
    if (treasure.name != "temp") {
      std::cout << " " << treasure.name << " ";
    }
  }
  std::cout << "\n\nThank you for playing! See you next time!" << std::endl;
  PrintLineBreak();

  return 0;
}

void ProgramGreeting() {
  std::cout << "\n";
  time_t t = time(NULL);
  tm *timePtr = localtime(&t);

  std::cout
      << "Welcome! Are you ready to experience a cool text adventure game?\n"
      << std::endl;
  std::cout
      << "My name is Charan Karthik. I hope you enjoy my capstone assignment "
         "for CISP 360 with Professor C. Fowler in the Summer of 2023.\n"
      << std::endl;
  std::cout << "Today's date is: " << std::put_time(timePtr, "%B %d, %Y")
            << std::endl;
}

void PrintLineBreak() {
  std::cout
      << "\n----------------------------------------------------------------\n";
}

char GameMenu() {
  std::string user_input;
  char choice;

  while (true) {
    std::cout << "\nChoose an option:\n";
    std::cout << "A. Start Game\n";
    std::cout << "B. Explain Game\n";
    std::cout << "C. Exit Game\n";
    std::cout << "Enter your choice (A/B/C): ";

    std::cin.clear();
    std::getline(std::cin, user_input);
    // Speciﬁcation C3 - Input Validation
    if (user_input.length() == 1) {
      choice = std::toupper(user_input[0]);
      if (choice == 'A' || choice == 'B' || choice == 'C') {
        return choice;
      }
    }
    std::cout << "\nInvalid choice. Please select a valid option.\n";
  }
}

void DisplayInstructions() {
  std::cout
      << "\nThe game is a text-based adventure where you move through "
         "rooms, encounter monsters, and discover treasures. \n\nUse the N, "
         "S, "
         // Specification B - Additional Feature 6: More User Actions!
         "E, W keys to navigate. Press L to look around, O to search, and "
         "I to check "
         "your item box. All inputs are case-insensitive. Press D to display "
         "the instructions at any time."
      << std::endl;
}

// Speciﬁcation C3 - Detailed Look
void InspectRoom(int occupied_room_code, Room rooms[], bool user_looks,
                 Monster monsters[], int num_monsters) {
  Room room = rooms[occupied_room_code];

  if (room.has_been_visited && !user_looks) {
    std::cout << "\nYou are in the: " << room.room_name << std::endl;
  } else {
    std::cout << "\nYou have entered the: " << room.room_name << "\n"
              << room.room_description << std::endl;
    rooms[occupied_room_code].has_been_visited = true;

    if (!user_looks && room.occupied_code) {
      do {
        int monster_index = GenerateRandomNumber(0, num_monsters - 1);
        Monster monster = monsters[monster_index];

        if (!monster.has_spawned) {
          if (monster.is_alive) {
            std::cout << "\nYou have encountered a " << monster.name << "!\n";
            std::cout
                << monster.name
                << " looks at you and continues minding its own business.\n";
          } else {
            std::cout << "\nYou see the corpse of the " << monster.name
                      << "!\n";
          }
          monsters[monster_index].has_spawned = true;
          break;
        }
      } while (true);
    }
  }

  std::cout << "\nYou can go: [";
  for (const char &exit : room.allowable_exits) {
    if (exit != 0) {
      std::cout << " " << exit << " ";
    }
  }
  std::cout << "]" << std::endl;
}

char GetUserAction() {
  std::string user_input;
  char user_action;

  while (true) {
    std::cout << "\nWhat action would you like to take? ";

    std::cin.clear();
    std::getline(std::cin, user_input);
    // Speciﬁcation C3 - Input Validation
    if (user_input.length() == 1) {
      user_action = std::toupper(user_input[0]);
      if (user_action == 'N' || user_action == 'E' || user_action == 'S' ||
          user_action == 'W' || user_action == 'L' || user_action == 'O' ||
          user_action == 'I' || user_action == 'D') {
        return user_action;
      }
    }
    std::cout << "\nInvalid action. Remember: Press D to display the "
                 "instructions again.\n";
  }

  return user_action;
}

int GenerateRandomNumber(int low_range, int high_range) {
  static std::default_random_engine generator(time(nullptr));
  std::uniform_int_distribution<int> distribution(low_range, high_range);
  return distribution(generator);
}

void SearchForItems(int occupied_room_code, Room rooms[], Treasure treasures[],
                    Treasure user_treasures[], int num_items) {
  Room room = rooms[occupied_room_code];
  std::cout << "\nYou search the room..." << std::endl;

  if (room.has_item && !room.has_item_been_found) {
    Treasure found_treasure;

    do {
      int treasure_index = GenerateRandomNumber(0, num_items - 1);

      if (!treasures[treasure_index].has_been_found) {
        found_treasure = treasures[treasure_index];
        std::cout << "You found a " << found_treasure.name << "!" << std::endl;

        treasures[treasure_index].has_been_found = true;
        room.has_item_been_found = true;
        rooms[occupied_room_code].has_item_been_found = true;
      }
    } while (!room.has_item_been_found);

    do {
      int user_inventory_index = GenerateRandomNumber(0, num_items - 1);
      if (user_treasures[user_inventory_index].name == "temp") {
        user_treasures[user_inventory_index] = found_treasure;
        break;
      }
    } while (true);
  } else
    std::cout << "You found nothing." << std::endl;
}

void CheckItemBox(Treasure user_treasures[], int num_treasures) {
  std::cout << "\nYou check your item box..." << std::endl;
  std::cout << "You have:" << std::endl;
  for (int i = 0; i < num_treasures; i++) {
    if (user_treasures[i].name != "temp") {
      std::cout << " " << user_treasures[i].name << " ";
    }
  }
  std::cout << "\n";
}

bool CheckIfValidMoveDirection(Room rooms[], int occupied_room_code,
                               char user_action) {
  Room room = rooms[occupied_room_code];
  for (const char &exit : room.allowable_exits) {
    if (exit == user_action) {
      return true;
    }
  }
  return false;
}

int NavigateDungeon(int occupied_room_code, const NavigationMap &navMap,
                    char user_action) {
  int new_room_code = navMap.at(occupied_room_code).at(user_action);
  return new_room_code;
}

// Specification A - Reflection
// Word Count (from Google Docs): 395
/*
Wowza! That was one heck of an assignment - and a HUGE difference when compared
to my first C++ program and the first assignment (So.. Many.. Lines.. Of..
Code..!!). That being said though, it was definitely really fun to think through
and so, so satisfying to finish. Although I would’ve loved to replace that
current record holder and add more than 18 additional features, I’m happy to
have completed what I did in the time I had.

At the beginning of this class, I knew nothing about C++. I just knew the memes
that even printing something out to the console on C++ was a pain compared to
languages like Python and JavaScript. And for that reason, I’d kind of stayed
away from low-level programming languages. But, now, I’m very, very glad to have
taken a C++ programming course. Though my love-hate relationship with strictly
types languages still hadn’t shifted entirely to a loving relationship, I’m glad
that C++ forced me to think through each line of code I write before even
writing it to make sure that the types made sense and that the logic would
actually work and compile (this actually even helped me perform a bit better at
work!).

I’m also happy to have learned to use ChatGPT to receive code feedback. This
thought hadn’t occurred to me in the past, but now I’m using it quite frequently
(shoutout to work PRs that get merged to main directly without any feedback
comments haha). Though sometimes the AI is wonky and makes a few mistakes here
and there, for the most part, it does its job well and makes my development
faster and life easier.

Now that we’re at the tail end of the class, I wish I could’ve spent more time
with C++... An eight-week summer session just flies by so fast! Coming into this
class I wanted to learn more about topics such as memory management and
efficient programming, but looks like I’ll have to take CISP 400 for that.
Either way though, I’m glad I got to take CISP 360 and that I finally got to
learn and use C++ (this final project is arguably the most complex thing I’ve
made when learning a language for the first time - though I have made full-stack
web apps, this was a different kind of problem for me to tackle that I really
enjoyed).

 */