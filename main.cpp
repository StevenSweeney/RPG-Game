/*

The Table of Contents:
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
villageblacksmith()
the village blacksmith exploreable
---
craftweapon()
weapon craft script for blacksmith
---
purchase(std::string name, double cost)
purchase script for a name and cost
---

EXPLOREABLES:
thewoods()
thevillageexploreable()

*/

#include <iostream>
#include <vector>

// init player variables
double playerHealth = 10; //player health. Default is 10
double playerStrength = 7; //strength multiplier the player has towards enemies. Default is 7
double playerSpeed = 7; //speed multiplier the player has when in combat. Default is 7
double playerXP = 0; //the experience the player has, which can be used in different places
double answer = 0; //global variable for std::cin use
double maxPlayerHealth = playerHealth; //maximum player health, which is automatically set to whatever playerHealth equals
double playerCoins = 999; //player currency /return to default after test!
double craftableCost = 0; //blacksmith craftable cost
int matSelect = 0; //material # in blacksmith
int weaponSelect = 0; //weapon type # in blacksmith
double determineBoughtWeaponStrength = 0; //use to determine how strong the weapon the player bought from the blacksmith is
std::string weaponName; //weapon name you bought at the blacksmith

//init player vectors
std::vector<std::string> playerInventory; //list of what the player has
std::vector<double> playerInventoryType; //tells computer what type of item this is >>>> 1 = weapon, 
std::vector<double> playerInventoryStrength; //tells computer what damage this item can do relative to player's strength
std::vector<std::string> playerQuests; //list of what quests the player can do
std::vector<std::string> playerExploreables = {"The Woods", "The Village"}; //list of what the player can explore
std::vector<std::string> woodsExploreables = {"Mysterious Tree", "Berry Bushes"}; //if the player chooses The Woods to explore, these are the default options
std::vector<std::string> villageExploreables = {"Blacksmith", "Bookshop", "Field", "Training Grounds"}; //if the player chooses The Village to explore, these are the default options
std::vector<std::string> blacksmithMaterials = {"Wood", "Stone", "Copper", "Bronze", "Iron", "Steel"}; //current available materials for crafting a new weapon in the blacksmith
std::vector<double> blacksmithMatCost = {4, 12, 20, 28, 40, 58}; //material's cost for crafting a new weapon in the blacksmith
std::vector<std::string> weaponTypes = {"Axe", "Sword", "Small Blade", "Large Hammer"}; //blacksmith weapon types
std::vector<double> weaponCostMultipliers = {0.75, 1, 0.6, 1.25};

// init the player quests and other save states
bool hasFirstQuest = false;

void purchase(std::string name, double cost) {

    if (playerCoins >= cost) {
        std::cout << "\n-----------\nPurchase Successful\n-----------\n\nPurchased " << name << " for " << cost << " coins.\n";
        playerCoins = playerCoins - cost;
        std::cout << "You now have " << playerCoins << " coins.";
    } else {
        std::cout << "\n-----------\nPurchase Unsuccessful\n-----------\n\nYou don't have enough coins.\n";
    }
}

void craftweapon() {

    std::cout << "\n\nSo you want to craft a weapon? Let's talk material. Which one?\n\n"; //material select dialogue

            for (int i = 0; i < blacksmithMaterials.size(); i++) {

                std::cout << i + 1 << ") " << blacksmithMaterials[i] << " | " << blacksmithMatCost[i] << " Coins\n";

            }

            answer = 99;
            while (answer - 1 > blacksmithMaterials.size()) { //material select
                                                              // There has to be answer - 1 because the player selects 1 through however many choices there are. and vector starting is 0, not 1
                std::cin >> answer;

                if (answer - 1 > blacksmithMaterials.size()) {
                    std::cout << "Invalid number, try again\n";
                }

            }

            matSelect = answer - 1;
            craftableCost = craftableCost + blacksmithMatCost[answer - 1];
            std::cout << "Total cost so far: " << craftableCost << "\n";

            std::cout << blacksmithMaterials[matSelect] << " is a great choice! What weapon would you like to build?\n\n"; //weapon select dialogue
            
            for (int i = 0; i < weaponTypes.size(); i++) {
                
                std::cout << i + 1 << ") " << weaponTypes[i] << " | Makes total cost: " << craftableCost * weaponCostMultipliers[i] << "\n";

            }

            answer = 99;
            while (answer - 1 > weaponTypes.size()) { //weapon select
                
                std::cin >> answer;

                if (answer - 1 > weaponTypes.size()) {
                    std::cout << "Invalid number, try again\n";
                }

            }

            weaponSelect = answer - 1;
            craftableCost = craftableCost * weaponCostMultipliers[answer - 1];
            std::cout << "\n\nLooks like you want a " << blacksmithMaterials[matSelect] << " " << weaponTypes[weaponSelect] << "? That will be " << craftableCost << " coins.\n";
            std::cout << "Will you pay for it?\n1) Yes\n2) No\n";

            answer = 0;
            while (answer > 2 || answer < 1) {

                std::cin >> answer;

                if (answer == 1) { //Check if you can buy the weapon, and add to inventory if so

                    if (playerCoins >= craftableCost) {
                        
                        weaponName = blacksmithMaterials[matSelect] + " " + weaponTypes[weaponSelect];
                        playerInventory.push_back(weaponName);
                        playerInventoryType.push_back(1);
                        
                        determineBoughtWeaponStrength = (matSelect * 1.5) + (weaponSelect * 3); // will determine how strong the weapon is and add it into the playerInventoryStrength vector
                        playerInventoryStrength.push_back(determineBoughtWeaponStrength);

                        std::cout << "Thanks for shopping here at the village blacksmith!\n";

                        purchase(weaponName, craftableCost);

                    } else {

                        std::cout << "You don't have enough coins to buy this... come back when you do!\n\n";
                    }

                } else if (answer == 2) { //cancel purchase

                    std::cout << "That's ok. Please come back if you decide you want it!\n\n";

                } else {

                    std::cout << "Invalid number, please input 1 or 2.\n";

                }
                //reset all variables
                matSelect = 0;
                weaponSelect = 0;
                craftableCost = 0;

            }

}

void villageblacksmith() {

    std::cout << "\n\n-----------\nVillage Blacksmith\n-----------\n\nWelcome to the Blacksmith! What can I do for you?\n\n";
    std::cout << "1) Craft Weapon\n2) Buy Used\n0) Exit\n";
    answer = 3;
    while (answer != 0) { //craft, buy, or exit blacksmith

        std::cin >> answer;

        if (answer == 1) { // craft

            craftweapon();


        } else if (answer == 2) {

            //do if answer = 2

        } else {

            std::cout << "Invalid number.\n";

        }
    }
}

void thewoods() {
    for (int i = 0; i < woodsExploreables.size(); i++) {

        std::cout << "\n" << i + 1 << ")" << woodsExploreables[i];

    }
    std::cout << "\nYou've entered the woods, which way will you take? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (answer != 0) {
        std::cin >> answer;
        if (woodsExploreables[answer - 1] == "Mysterious Tree") {

        } else if (woodsExploreables[answer - 1] == "Berry Bushes") {

        } else {
            std::cout << "Not a valid exploration place, try again\n";
        }
    }
}

void thevillageexploreable() {
    for (int i = 0; i < villageExploreables.size(); i++) {

        std::cout << "\n" << i + 1 << ")" << villageExploreables[i];

    }
    std::cout << "\nYou've decided to explore inside the village. Where will you go? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (answer != 0) {
        std::cin >> answer;
        if (answer == 0) {
            break;
        } else if (villageExploreables[answer - 1] == "Blacksmith") {
            villageblacksmith();
        } else if (villageExploreables[answer - 1] == "Bookshop") {

        } else if (villageExploreables[answer - 1] == "Field") {

        } else if (villageExploreables[answer - 1] == "Training Grounds") {

        } else {
            std::cout << "Not a valid exploration place, try again\n";
        }
    }
}

void explore() {

    for (int i = 0; i < playerExploreables.size(); i++) {

        std::cout << "\n" << i + 1 << ")" << playerExploreables[i];

    }
    std::cout << "\nWhere will you explore? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (answer != 0) {
        std::cin >> answer;
        if (playerExploreables[answer - 1] == "The Woods") {
            thewoods();
        } else if (playerExploreables[answer - 1] == "The Village") {
            thevillageexploreable();
        } else {
            std::cout << "Not a valid exploration place, try again\n";
        }
    }
}

void currentquests() { //prints everything in quests

    for (int i = 0; i <= playerQuests.size(); i++) {

        std::cout << playerQuests[i];

    }
}

void checkinventory() { //prints everything in inventory

    for (int i = 0; i <= playerInventory.size(); i++) {

        std::cout << playerInventory[i];

    }
}

void currentplayerstats() {//prints current player stats

    std::cout << "Current Player Stats:\nHealth: " << playerHealth << "/" << maxPlayerHealth << "\nStrength: " << playerStrength << "\nSpeed: " << playerSpeed << "\nTotal XP: " << playerXP << "\nCoins: " << playerCoins << "\n";

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