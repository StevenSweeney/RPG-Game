/*

Table of Contents:
Use the find feature (CMD/CTRL+F) to find different parts of the game code. Hopefully all important functions and code will be here

---
currentplayerstats()
if called, will print all current player stats
---
game()
actual gameplay portion of the game. calls different gameplay portions like explore mode
---
main()
starting screen, where you select load game or new game
---
explore()
when the player wants to 'Explore', this script will run. it prints all of the available ways to explore
---
currentquests()
prints all of the current quests the player has
---
checkinventory()
prints all of the current things the player has in their inventory
---

EXPLOREABLES:
thewoods()
thevillageexploreable()

*/

#include <iostream>
#include <vector>

// init player variables, inventory, etc.
double playerHealth = 10; //player health. Default is 10
double playerStrength = 7; //strength multiplier the player has towards enemies. Default is 7
double playerSpeed = 7; //speed multiplier the player has when in combat. Default is 7
double playerXP = 0; //the experience the player has, which can be used in different places
double answer = 0; //global variable for std::cin use
double maxPlayerHealth = playerHealth; //maximum player health, which is automatically set to whatever playerHealth equals
std::vector<std::string> playerInventory; //list of what the player has
std::vector<std::string> playerQuests; //list of what quests the player can do
std::vector<std::string> playerExploreables = {"The Woods", "The Village"}; //list of what the player can explore
std::vector<std::string> woodsExploreables = {"Mysterious Tree", "Berry Bushes"}; //if the player chooses The Woods to explore, these are the options
std::vector<std::string> villageExploreables = {"Blacksmith", "Bookshop", "Field", "Training Grounds"}; //if the player chooses The Village to explore, these are the options

// init the player quests and other save states
bool hasFirstQuest = false;

void thewoods() {
    for (int i = 1; i <= woodsExploreables.size(); i++) {

        std::cout << "\n" << i << ")" << woodsExploreables[i];

    }
    std::cout << "\nYou've entered the woods, which way will you take? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (answer != 0) {
        std::cin >> answer;
        if (woodsExploreables[answer] == "Mysterious Tree") {

        } else if (woodsExploreables[answer] == "Berry Bushes") {

        } else {
            std::cout << "Not a valid exploration place, try again\n";
        }
    }
}

void thevillageexploreable() {
    for (int i = 1; i <= villageExploreables.size(); i++) {

        std::cout << "\n" << i << ")" << villageExploreables[i];

    }
    std::cout << "\nYou've decided to explore inside the village. Where will you go? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (answer != 0) {
        std::cin >> answer;
        if (villageExploreables[answer] == "Blacksmith") {

        } else if (villageExploreables[answer] == "Bookshop") {

        } else if (villageExploreables[answer] == "Field") {

        } else if (villageExploreables[answer] == "Training Grounds") {

        } else {
            std::cout << "Not a valid exploration place, try again\n";
        }
    }
}

void explore() {

    for (int i = 1; i <= playerExploreables.size(); i++) {

        std::cout << "\n" << i << ")" << playerExploreables[i];

    }
    std::cout << "\nWhere will you explore? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (answer != 0) {
        std::cin >> answer;
        if (playerExploreables[answer] == "The Woods") {
            thewoods();
        } else if (playerExploreables[answer] == "The Village") {
            thevillageexploreable();
        } else {
            std::cout << "Not a valid exploration place, try again\n";
        }
    }
}

void currentquests() { //prints everything in quests

    for (int i = 1; i <= playerQuests.size(); i++) {

        std::cout << playerQuests[i];

    }
}

void checkinventory() { //prints everything in inventory

    for (int i = 1; i <= playerInventory.size(); i++) {

        std::cout << playerInventory[i];

    }
}

void currentplayerstats() {//prints current player stats

    std::cout << "Current Player Stats:\nHealth: " << playerHealth << "/" << maxPlayerHealth << "\nStrength: " << playerStrength << "\nSpeed: " << playerSpeed << "\nTotal XP: " << playerXP << "\n";

}

void game() { //actual gameplay

    //start game

    std::cout << "\n\n\n\n\n\n\n\n\n-----------\nNew Game\n-----------\n";

    currentplayerstats();
    std::cout << "These are your current player stats.\nThey will show up when you are making decisions based on player stats, or when you request it.";

    std::cout << "\n\nPlease select which you'd like to put 3 more points into:\n1) Strength\n2) Speed\n";

    answer = 0;
    while (answer > 2 || answer < 1) {

        std::cin >> answer;

        if (answer == 1) {

            playerStrength = playerStrength + 3;

        } else if (answer == 2) {

            playerSpeed = playerSpeed + 3;

        } else {

            std::cout << "Invalid number, please input 1 or 2.\n";

        }
    }

    currentplayerstats();

    std::cout << "\n\nWould you like to have an extra 2 max health points to sacrifice 2 speed points and 2 strength points?\n1) Yes\n2) No\n";

    answer = 0;
    while (answer > 2 || answer < 1) {

        std::cin >> answer;

        if (answer == 1) {

            playerStrength = playerStrength - 2;
            playerSpeed = playerSpeed - 2;
            maxPlayerHealth = maxPlayerHealth + 2;
            playerHealth = playerHealth + 2;

        } else if (answer == 2) {

            // do nothing

        } else {

            std::cout << "Invalid number, please input 1 or 2.\n";

        }
    }

    currentplayerstats();
    std::cout << "These are your starting stats. Press any button and hit enter to continue.";
    
    std::string tempans;
    std::cin >> tempans;

    //Actual game start

    std::string toDo;

    while (toDo != "Exit") {
        std::cout << "What should you do? (type 'Help' for commands)\n";
        std::cin >> toDo;
        std::cout << "\nYou Chose: " << toDo << "\n\n";

        if (toDo == "Help") {
            std::cout << "Commands:\nHelp\nExplore\nMyStats\nQuests\nInventory\n";
        } else if (toDo == "Explore") {
            explore();
        } else if (toDo == "MyStats") {
            currentplayerstats();
        } else if (toDo == "Quests") {
            currentquests();
        } else if (toDo == "Inventory") {
            checkinventory();
        } else {
            std::cout << "Invalid. Try again...";
        }
        std::cout << "\n";
    }
}

int main() { //starting screen

    int answer = 0;

    std::cout << "RPG Game\n\n";
    std::cout << "1) Load Game\n2) New Game\n";

    while (answer > 2 || answer < 1) {

        std::cin >> answer;

        if (answer == 1) {

            //Load Game State, but for now it's coming soon
            std::cout << "This feature is coming soon, please select 'new game'\n";
            answer = 3; //sets to 3 so it  can re-ask for input

        } else if (answer == 2) {

            game();

        } else {

            std::cout << "Invalid number, please input 1 or 2.\n";

        }
    }
}