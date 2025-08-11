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
mysterioustree()
---
quest()
quest manager
---
removefrominventory()
call this to remove something from the player inventory
---
addtoinventory()
call this to add something to the player inventory
---
determineEnemyHealRate()
returns a heal rate value based on what enemy name and level you give it
---
determineEnemyDamage()
returns a damage value based on what enemy name and level you give it
---
determineEnemyHealth()
returns a health value based on what enemy name and level you give it
---
newbattle()
battle script
---
traininggrounds()
village training grounds
---

EXPLOREABLES:
thewoods()
thevillageexploreable()

INVENTORY TYPES:
1 = weapon short-range
2 = healing potion
3 = weapon long-range
4 = material to sell
5 = tool
6 = junk

*/

#include <iostream>
#include <vector>
#include <stdlib.h>

// init player variables
double playerHealth = 10; //player health. Default is 10
double playerStrength = 7; //strength multiplier the player has towards enemies. Default is 7
double playerSpeed = 7; //speed multiplier the player has when in combat. Default is 7
double playerXP = 0; //the experience the player has, which can be used in different places
double answer = 0; //global variable for std::cin use
double maxPlayerHealth = playerHealth; //maximum player health, which is automatically set to whatever playerHealth equals
double playerCoins = 0; //player currency
double craftableCost = 0; //blacksmith craftable cost
int matSelect = 0; //material # in blacksmith
int weaponSelect = 0; //weapon type # in blacksmith
double determineBoughtWeaponStrength = 0; //use to determine how strong the weapon the player bought from the blacksmith is
double playerLuck = 1; //luck the player has for the game. can be upgraded with levels and stuff
double armorLevel = 1; //the higher this is, the more your player is protected from attacks. this shouldnt be too high, max is probably 3 because the damage dealt to player is divided by this number
std::string weaponName; //weapon name you bought at the blacksmith

//init player vectors
std::vector<std::string> playerInventory; //list of what the player has
std::vector<double> playerInventoryType; //tells computer what type of item this is >>>> 1 = weapon, 
std::vector<double> playerInventoryStrength; //tells computer what damage this item can do relative to player's strength
std::vector<std::string> playerQuests; //list of what quests the player can do
std::vector<std::string> playerExploreables = {"The Woods", "The Village"}; //list of what the player can explore
std::vector<std::string> woodsExploreables = {"Mysterious Tree", "Berry Bushes"}; //if the player chooses The Woods to explore, these are the default options
std::vector<std::string> villageExploreables = {"Blacksmith", "Bookshop", "Field", "Training Grounds", "Armorer", "Ranged Weaponsmith", "News Board", "Butcher Shop"}; //if the player chooses The Village to explore, these are the default options
std::vector<std::string> blacksmithMaterials = {"Wood", "Stone", "Copper", "Bronze", "Iron", "Steel"}; //current available materials for crafting a new weapon in the blacksmith
std::vector<double> blacksmithMatCost = {4, 12, 20, 28, 40, 58}; //material's cost for crafting a new weapon in the blacksmith
std::vector<std::string> weaponTypes = {"Axe", "Sword", "Small Blade", "Large Hammer"}; //blacksmith weapon types
std::vector<double> weaponCostMultipliers = {0.75, 1, 0.6, 1.25};
std::vector<std::string> armorLevelNames = {"None", "Bronze", "Iron", "Steel"};
std::vector<double> armorCost = {0, 25, 75, 180};
std::vector<std::string> enemiesRanged = {"Bat"};
std::vector<std::string> rangedWeaponsStock;
std::vector<int> rangedWeaponsStrength;
std::vector<double> rangedWeaponsCost;
std::vector<std::string> villageNews;

// init the player quests and other save states
bool hasFirstQuest = false;

//dev variables
bool testing = true;
std::string string1;
int int1;
double double1;

void villagenewsboard() {
    std::cout << "==== Village News Board ====\n\n";

    if (villageNews.size() == 0) {

        std::cout << "Sorry, the news board currently has nothing written on it. Maybe come back later?\n";

    } else {

        for (int i = 0; i < villageNews.size(); i++) {
            std::cout << "[ " << villageNews[i] << " ]\n";
        }

    }

    std::cout << "\n==== ====\n\n";

}

void addtoinventory(std::string name, int type, double strength) { //Call this to add something to the inventory. moved to this spot in script at 8/10/25 because of rangedweaponsmith() function
    playerInventory.push_back(name);
    playerInventoryType.push_back(type);
    playerInventoryStrength.push_back(strength);
}

bool purchase(std::string name, double cost) { //handles purchasing. moved to this spot in script at 7/25/25 (because of armorer() function)

    if (playerCoins >= cost) {
        std::cout << "\n-----------\nPurchase Successful\n-----------\n\nPurchased " << name << " for " << cost << " coins.\n";
        playerCoins = playerCoins - cost;
        std::cout << "You now have " << playerCoins << " coins.\n\n";
        return (true);
    } else {
        std::cout << "\n-----------\nPurchase Unsuccessful\n-----------\n\nYou don't have enough coins.\n";
        return (false);
    }
}

void rangedweaponsmith() {

    //init rangedweaponsmith variables
    int chosen;

    std::cout << "Welcome to the Ranged Weapon Smith. Buy ranged weapons here.\n\nWhat would you like to do?\n1) Buy ranged weapons\n2) Exit\n#";

    answer = 0;
    while (true) {
        std::cin >> answer;
        if (std::cin.fail() || (answer != 1 && answer != 2)) {
            std::cout << "Invalid input. Please choose 1 or 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (answer == 1 && rangedWeaponsStock.size() > 0) {
            std::cout << "Our stock currently has...\n";

            for (int i = 0; i < rangedWeaponsStock.size(); i++) {
                std::cout << i + 1 << ") " << rangedWeaponsStock[i] << " | Damage: " << rangedWeaponsStrength[i] << " | Cost: " << rangedWeaponsCost[i] << "\n";
            }

            std::cout << "Which one would you like? #";

            answer = 0;
            while (true) {
                std::cin >> answer;
                if (std::cin.fail() || (answer < 1 || answer > rangedWeaponsStock.size() - 1)) {
                    std::cout << "Invalid input. Please try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {

                    chosen = answer - 1;

                    { //script

                        std::cout << "You chose the " << rangedWeaponsStock[chosen] << " for " << rangedWeaponsCost[chosen] << ". Would you like to buy it?\n1) Yes\n2) No\n#";

                        answer = 0;
                        while (true) {
                            std::cin >> answer;
                            if (std::cin.fail() || (answer != 1 || answer != 2)) {
                                std::cout << "Invalid input. Please try again.\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            } else if (answer == 1) {
                                if (purchase(rangedWeaponsStock[chosen], rangedWeaponsCost[chosen]) == true) {

                                    std::cout << "Thank you for shopping, please come again!";

                                    addtoinventory(rangedWeaponsStock[chosen], 3, rangedWeaponsStrength[chosen]);

                                    { //erase weapon from the weaponsmith stock
                                        rangedWeaponsCost.erase(rangedWeaponsCost.begin() + chosen);
                                        rangedWeaponsStock.erase(rangedWeaponsStock.begin() + chosen);
                                        rangedWeaponsStrength.erase(rangedWeaponsStrength.begin() + chosen);
                                    }
                                    break;
                                } else {
                                    std::cout << "Maybe you'll get it next time...";
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                break;
            }
        } else if (answer == 1 && rangedWeaponsStock.size() == 0) {
            std::cout << "Sorry, unfortunately we do not have any weapons in stock as of right now. You can check the village news board to see when we have some in stock!\n\n";
            break;
        } else if (answer == 2) {
            break;
        }
        break;
    }
}

void armorer() {
    std::cout << "Welcome to the Armorer. Upgrade your armor here.\n\nWhat would you like to do?\n1) Buy Armor\n2) Sell Material (Coming soon)\n3) Exit\n#";

    answer = 0;
    while (true) {
        std::cin >> answer;
        if (std::cin.fail() || (answer != 1 && answer != 2 && answer != 3)) {
            std::cout << "Invalid input. Please choose 1, 2, or 3.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (answer == 1) { //buy armor
            { //armor buying process

                std::cout << "So, you want to buy armor? Sure, which level of armor would you like?\n\n";

                for (int i = 1; i < armorLevelNames.size(); i++) { //print armor choices (starts at 1 because 0th data of armorLevelNames is "None")

                    std::cout << i << ") " << armorLevelNames[i] << " | Cost: " << armorCost[i] << " coins\n";

                }

                std::cout << "#";

                answer = 0;
                while (true) {
                    std::cin >> answer;
                    if (std::cin.fail() || (answer < 0 || answer > armorLevelNames.size())) {
                        std::cout << "Invalid Input.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else {
                        int armorBuying = answer;
                        std::cout << "You chose " << armorLevelNames[answer] << " armor for " << armorCost[answer] << " coins? Would you like to proceed with this purchase?\n1) Yes\n2) No\n#";

                        answer = 0;
                        while (true) {
                            std::cin >> answer;
                            if (std::cin.fail() || (answer != 1 && answer != 2)) {
                                std::cout << "Invalid Input. Please choose 1 or 2.\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            } else if (answer == 1) {
                                if (purchase(armorLevelNames[armorBuying] + " Armor", armorCost[armorBuying]) == true) {

                                    armorLevel = armorBuying;

                                    std::cout << "Thanks for coming!\n";
                                    break;
                                } else {
                                    std::cout << "Unfortunate. Come back when you have some coins!\n";
                                    break;
                                }
                                break;
                            } else if (answer == 2) {
                                break;
                            }
                            break;
                        }
                        break;
                    }
                    break;
                }

            }
            break;
        } else if (answer == 2) { //sell material
            std::cout << "This feature is coming soon.\n";
            break;
        } else if (answer == 3) { //exit
            std::cout << "\n\nPlease come again!\n";
            break;
        }
        break;
    }
}

void completequest(int questID) { // Call this to tell the player they've completed a quest and delete that from the quest list
    std::cout << "---------------\nQuest completed\n---------------\n" << playerQuests[questID];
    playerQuests.erase(playerQuests.begin() + questID);
}

void removefrominventory(int itemlocation) { //Call this to remove something from the inventory
    playerInventory.erase(playerInventory.begin() + itemlocation);
    playerInventoryType.erase(playerInventoryType.begin() + itemlocation);
    playerInventoryStrength.erase(playerInventoryStrength.begin() + itemlocation);
}

double determineEnemyHealRate(std::string enemyName, double enemyLevel) { //This determines how much the enemy heals when they can
    if (enemyName == "Basic Goblin") {
        return(3 * enemyLevel);
    } else {
        return(0);
    }
}

double determineEnemyDamage(std::string enemyName, double enemylevel) { //has a list of enemies in the game, and will return the appropriate damage the enemy should do
    if (enemyName == "Basic Goblin") {
        return(2.5 * enemylevel);
    } else {
        return(0);
    }
}

double determineEnemyHealth(std::string enemyName, double enemyLevel) { //has a list of enemies in the game, and will return the appropriate health the enemy should have
    if (enemyName == "Basic Goblin") {
        return (10 * enemyLevel);
    } else {
        return (0);
    }
}

void newbattle(std::string battleName, std::vector<std::string> enemies, std::vector<double> enemyLevel, bool choiceOfStart) { //battle script

    double playerxpearned = 0; //total XP player has earned/lost throughout the battle
    bool forfeit = false; //if this is ever set to true, battle will be called off
    bool playerTurn = true; //determines if the player has the next turn or the enemy does
    int enemyFighting = 0; //which enemy the player is fighting (in the vector)
    double damageDealt = 0; //init of the variable that keeps track of how much damage player does

    std::cout << "\n\n----------\nNew Battle\n----------\n--" << battleName << "--\n" << "Enemies: " << enemies.size() << "\n\n";
    if (choiceOfStart == true) { // if the battle allows you to forfeit right away, it will ask if you want to.
        std::cout << "Would you like to start this battle?\n1) Yes\n2) No\n";

        answer = 0;
        while (true) {
            std::cin >> answer;
            if (std::cin.fail() || (answer != 1 && answer != 2)) {
                std::cout << "Invalid input. Please choose 1 or 2.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else if (answer == 2) {
                forfeit = true;
                break;
            }
            break;
        }
    }

    double enemyHealth = determineEnemyHealth(enemies[enemyFighting], enemyLevel[enemyFighting]); //determines enemy health
    double enemyDamage = determineEnemyDamage(enemies[enemyFighting], enemyLevel[enemyFighting]); //determines enemy damage
    double enemyHealRate = determineEnemyHealRate(enemies[enemyFighting], enemyLevel[enemyFighting]); //determines enemy heal rate

    std::cout << "You first fight a " << enemies[enemyFighting] << " who is level " << enemyLevel[enemyFighting] << ".\n\n";

    while (forfeit == false && enemyFighting < enemies.size() && playerHealth > 0) { //if the player health is over 0, there are more enemies, or if player has not forfeit, it will still run

        // init battle vars, may put into global space
        int number = 1;
        std::vector<double> availableWeaponStrengths = {};
        std::vector<int> weaponLocation = {};
        int enemyHealRecharge = 2; //if this is 3, enemy will be able to use its healing ability. Sets to 2 because it will be added by 1 soon

        enemyHealRecharge++; //adds 1 to the charge of if the enemy can heal or not, every turn

        std::cout << "\n\nCurrent Enemy: " << enemies[enemyFighting] << "\nCurrent health: " << enemyHealth << "\n\nYou:\nCurrent Health: " << playerHealth << "/" << maxPlayerHealth << "\n\n";

        if (playerTurn == true) { // Play options (if its players turn)

            std::cout << "What would you like to do?\n\n1) Use weapon\n2) Use potion\n3) Attempt dodge\n4) Attempt forfeit\n#";

            answer = 0;
            while (true) {

                std::cin >> answer;
                if (std::cin.fail()) {
                    std::cout << "Invalid input. please choose 1 to 4.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else if (answer == 1) { // Use weapon
                    std::cout << "\nPlease choose a weapon to use...\n\n";

                    for (int i = 0; i < playerInventory.size(); i++) { // will add different strengths based on inventory items to a list that you can choose from. only adds if the type of weapon = 1 (weapon)

                        if (playerInventoryType[i] == 1) {

                            std::cout << number << ") " << playerInventory[i] << ", Strength: " << playerInventoryStrength[i] << "\n";
                            availableWeaponStrengths.push_back(playerInventoryStrength[i]);
                            number++;

                        }

                    }

                    std::cout << "\nI'll use weapon ";

                    answer = 0;
                    while (true) {
                        std::cin >> answer;
                        if (std::cin.fail() || (answer < 1 && answer > availableWeaponStrengths.size())) {
                            std::cout << "Invalid input.\n";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        } else {

                            damageDealt = availableWeaponStrengths[answer - 1] * playerLuck;

                            enemyHealth = enemyHealth - damageDealt;

                            std::cout << "You did " << damageDealt << " damage!\n-----";

                            playerxpearned = playerxpearned + damageDealt / 4;

                            playerTurn = false; // when playerturn is false, it is now the enemy's turn
                            break;
                        }
                    }

                    break;
                } else if (answer == 2) { // Use potion
                    std::cout << "Choose a potion to use\n\nHealing:\n";

                    for (int i = 0; i < playerInventory.size(); i++) {

                        if (playerInventoryType[i] == 2) {

                            std::cout << number << ") " << playerInventory[i] << ", Heal Strength: " << playerInventoryStrength[i] << "\n";
                            availableWeaponStrengths.push_back(playerInventoryStrength[i]);
                            weaponLocation.push_back(i);
                            number++;

                        }
                        

                    }

                    std::cout << "\nI'll use potion ";

                        answer = 0;
                        while (true) {
                            std::cin >> answer;
                            if (std::cin.fail() || (answer < 1 && answer > availableWeaponStrengths.size())) {
                                std::cout << "Invalid input.\n";
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            } else {
                                
                                playerHealth = playerHealth + availableWeaponStrengths[answer - 1];

                                std::cout << "You used " << playerInventory[weaponLocation[answer - 1]] << " and healed for " << availableWeaponStrengths[answer - 1] << " health!";

                                if (playerHealth > maxPlayerHealth) { //this is so the player cannot go over their max player health
                                    playerHealth = maxPlayerHealth;
                                }

                                std::cout << "\nYou are now at " << playerHealth << "/" << maxPlayerHealth << " health!\n";

                                removefrominventory(weaponLocation[answer - 1]);

                                playerxpearned = playerxpearned + damageDealt / 6;

                                playerTurn = false;
                                break;
                            }
                        }

                    break;
                } else if (answer == 3) { // Attempt dodge, coming soon

                    std::cout << "This feature is coming soon.\n";

                    break;
                } else if (answer == 4) { // Attempt forfeit, coming soon

                    std::cout << "This feature is coming soon.\n";

                    break;
                }

            }

        } else { // enemy turn
            std::cout << "It is now the enemy's turn.\n Enemy is choosing what to do...\n";
            int randomEnemyOption = 1;
            bool goodEnemyOption = false;

            //enemy will only heal if HP is low and the enemy option is 50-75. same with dodge

            while (goodEnemyOption == false) { //the enemy is basically rolling a 100 sided die, and depending on what it lands on it will determine if that is a good idea based on it's health. if it isn't safe, it'll re-roll.
                srand(time(NULL));
                randomEnemyOption = rand() % 100;

                //this can be compressed all into 1 if statement, but this is for better readability
                if ((randomEnemyOption > 50 && randomEnemyOption < 75) && enemyHealth < 5 && enemyHealRecharge >= 3) { //between 50 and 75 is healing, so it will check if it is smart to heal (if health < 5) and if it is even able to heal
                    goodEnemyOption = true;
                } else if ((randomEnemyOption > 75 && randomEnemyOption < 100) && enemyHealth < 2) { //between 75 and 100 is dodging, so it will check if it is smart to dodge (if health < 2)
                    goodEnemyOption = false; //this SHOULD be true but the feature is coming soon, same with the player dodging ability. 
                } else if (randomEnemyOption > 0 && randomEnemyOption < 50) { //between 0 and 50 is damaging/battling, and this is always an available option, that is why it has no other parameter
                    goodEnemyOption = true;
                }
            }

            if (randomEnemyOption > 0 && randomEnemyOption < 50) { //damage player, 1/2 chance

                std::cout << "\nThe enemy decides to attack! It deals " << enemyDamage << " damage!";

                playerHealth = (playerHealth - enemyDamage) / armorLevel;

                std::cout << "\n\nYou are now at " << playerHealth << "/" << maxPlayerHealth << " health!\n";

            } else if (randomEnemyOption > 50 && randomEnemyOption < 75) { //heal, 1/4 chance

                enemyHealth = enemyHealth + enemyHealRate;
                enemyHealRecharge = 0; //resets the charge for healing for enemies

                std::cout << "\nThe enemy decides to heal! It heals up to " << enemyHealth << " health!\n";

            } else if (randomEnemyOption > 75 && randomEnemyOption < 100) { //dodge next attack, 1/4 chance

                //coming soon!

            }


            playerTurn = true; // sets it back to the player's turn
        }

        { //determine if player or enemy is dead

            if (playerHealth <= 0) {
                std::cout << "\n\nOh no! You died!\nYou are hospitalized, and have to pay the fee of 5 coins and 1 XP point.";

                playerXP--;
                playerCoins = playerCoins - 5;

                break;
            }
            if (enemyHealth <= 0) {

                if (enemyFighting == enemies.size()) {
                    std::cout << "\nYou defeated the " << enemies[enemyFighting] << "! That's all of the enemies for this battle, you won!";
                    std::cout << "\n\n----------\nPlayer Won\n----------\nXP earned: " << playerxpearned << "\nEnemies defeated: " << enemies.size() << "\n";

                    playerXP = playerXP + playerxpearned;

                    if (hasFirstQuest == true) {
                        completequest(0);
                    }

                } else {
                    std::cout << "\nYou defeated the " << enemies[enemyFighting] << "! Now you will fight a level " << enemyLevel[enemyFighting + 1] << " " << enemies[enemyFighting + 1];
                }

                enemyFighting++;
                enemyHealRecharge = 2;

                enemyHealth = determineEnemyHealth(enemies[enemyFighting], enemyLevel[enemyFighting]);
                enemyDamage = determineEnemyDamage(enemies[enemyFighting], enemyLevel[enemyFighting]);
                enemyHealRate = determineEnemyHealRate(enemies[enemyFighting], enemyLevel[enemyFighting]);

                playerTurn = true; //player always starts a new battle first

                break;
            }

        }

    }
}

void traininggrounds() {

    std::cout << "\n\n----------------\nTraining Grounds\n----------------\nWelcome to the training grounds! Fight makeshift enemies to train and try out new weapons or armor.\n";
    std::cout << "\nWhat should you train?\n1) Sword (short ranged)\n2) Bow (long ranged)\n3) Basic Battle\n4) Exit\n\nI will choose #";

    answer = 0;
    while (true) {

        std::cin >> answer;
        if (std::cin.fail() || (answer < 1 || answer > 4)) {
            std::cout << "Invalid input. Please select 1-4.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cout << "\n\n";
            if (answer == 4) {
                break;
            } else if (answer == 1) {
                { //code for short-ranged training
                    std::cout << "Welcome to short-ranged sword training. Here, you can train with your sword to level it up or try it out.\n";
                    //develop soon
                }
                break;
            } else if (answer == 2) {
                { //code for long-ranged training
                    std::cout << "Welcome to long-ranged bow training. Here, you can train with your bow to level it up or try it out.";
                    //develop soon
                }
                break;
            } else if (answer == 3) {
                { //code for simple battle training
                    std::cout << "Welcome to basic battle training. Here, you can fight against enemies as if it was a real battle and train your weapons.";
                    newbattle("Test Battle", {"Basic Goblin"}, {1}, true); //this is a test run. it goes name, enemy name list (which will determine enemy health etc), enemy level which multiplies enemy stats, and if the player has a choice to forfeit before the battle.
                }
                break;
            }
        }

    }

}

void quest(std::string name, std::string desc) {

    std::cout << "\n\n---------\nNew Quest\n---------\n -- " << name << " -- \n" << "'" << desc << "'\n\n";
    playerQuests.push_back(name);

}

void mysterioustree() {

    if (hasFirstQuest == false) {//if player has not recied the first quest

        std::cout << "\nYou decide to walk up to the mysterious tree. It is large, but has a green glow to it.\nYou want to touch it. Will you?\n1) Yes\n2) No\n";

        answer = 0;
        while (true) {

            std::cin >> answer;

            if (std::cin.fail() || (answer != 1 && answer != 2)) {
                std::cout << "Invalid number, please input 1 or 2.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {

                if (answer == 1) {

                std::cout << "\nYou touch it. You hear whispers about the future, and decide you need to train your skills.";
                quest("The Beginning", "Train against 1 opponent at the Training Grounds in the Village.");

                } else if (answer == 2) {

                //do if answer = 2

                }
            break;
            }
    }

    } else { //if player has recieved the first quest

    }
}



void craftweapon() {

    std::cout << "\n\nSo you want to craft a weapon? Let's talk material. Which one?\n\n"; //material select dialogue

            for (int i = 0; i < blacksmithMaterials.size(); i++) {

                std::cout << i + 1 << ") " << blacksmithMaterials[i] << " | " << blacksmithMatCost[i] << " Coins\n";

            }

            answer = 99;
            while (true) { //material select
                           // There has to be answer - 1 because the player selects 1 through however many choices there are. and vector starting is 0, not 1
                std::cin >> answer;

                if (std::cin.fail() || answer - 1 > blacksmithMaterials.size()) {
                    std::cout << "Invalid number, try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    break;
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
            while (true) { //weapon select
                
                std::cin >> answer;

                if (std::cin.fail() || answer - 1 > weaponTypes.size()) {
                    std::cout << "Invalid number, try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    break;
                }

            }

            weaponSelect = answer - 1;
            craftableCost = craftableCost * weaponCostMultipliers[answer - 1];
            std::cout << "\n\nLooks like you want a " << blacksmithMaterials[matSelect] << " " << weaponTypes[weaponSelect] << "? That will be " << craftableCost << " coins.\n";
            std::cout << "Will you pay for it?\n1) Yes\n2) No\n";

            answer = 0;
            while (true) {

                std::cin >> answer;

                if (std::cin.fail() || (answer != 1 && answer != 2)) {
                    std::cout << "Invalid number, please input 1 or 2.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    if (answer == 1) { //Check if you can buy the weapon, and add to inventory if so

                        if (playerCoins >= craftableCost) {
                        
                            weaponName = blacksmithMaterials[matSelect] + " " + weaponTypes[weaponSelect];
                            playerInventory.push_back(weaponName);
                            playerInventoryType.push_back(1);
                        
                            determineBoughtWeaponStrength = (matSelect * 1.5) + (weaponSelect * 3); // will determine how strong the weapon is and add it into the playerInventoryStrength vector
                            playerInventoryStrength.push_back(determineBoughtWeaponStrength);

                            std::cout << "Thanks for shopping here at the village blacksmith!\n";

                            purchase(weaponName, craftableCost);
                            break;

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
                    break;
                }
    }
}

void villageblacksmith() {

    std::cout << "\n\n------------------\nVillage Blacksmith\n------------------\n\nWelcome to the Blacksmith! What can I do for you?\n\n";
    std::cout << "1) Craft Weapon\n2) Buy Used\n0) Exit\n";
    answer = 3;
    while (true) { //craft, buy, or exit blacksmith

        std::cin >> answer;
        if (std::cin.fail() || (answer != 0 && answer != 1 && answer != 2)) {
            std::cout << "Invalid number, please input 1, 2, or 0.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            if (answer == 1) { // craft

                craftweapon();
                answer = 0;
                break;

            } else if (answer == 2) {

            //buy from used market. may be coming soon or may delete for better idea..

            } else if (answer == 0) {
                break;
            }
            break;
        }
    }
}

void thewoods() {
    for (int i = 0; i < woodsExploreables.size(); i++) {

        std::cout << "\n" << i + 1 << ")" << woodsExploreables[i];

    }
    std::cout << "\nYou've entered the woods, which way will you take? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (true) {
        std::cin >> answer;
        if (std::cin.fail()) {
            std::cout << "Invalid choice, try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            if (answer == 0) {
                break;
            } else if (woodsExploreables[answer - 1] == "Mysterious Tree") {
                mysterioustree();
                break;
            } else if (woodsExploreables[answer - 1] == "Berry Bushes") {

                break;
            } else {
                std::cout << "Invalid choice, try again.\n";
            }
        }
    }
}

void thevillageexploreable() {
    for (int i = 0; i < villageExploreables.size(); i++) {

        std::cout << "\n" << i + 1 << ")" << villageExploreables[i];

    }
    std::cout << "\nYou've decided to explore inside the village. Where will you go? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (true) {
        std::cin >> answer;
        if (std::cin.fail()) {
            std::cout << "Invalid answer, try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
        if (answer == 0) {
            break;
        } else if (villageExploreables[answer - 1] == "Blacksmith") {
            villageblacksmith();
            break;
        } else if (villageExploreables[answer - 1] == "Bookshop") {
            //this is where you can maybe get some knowledge/XP. read books for coins and gain XP
            break;
        } else if (villageExploreables[answer - 1] == "Field") {
            //train speed maybe?
            break;
        } else if (villageExploreables[answer - 1] == "Training Grounds") {
            traininggrounds();
            break;
        } else if (villageExploreables[answer - 1] == "Armorer") {
            armorer(); //buy or make armor
            break;
        } else if (villageExploreables[answer - 1] == "Ranged Weaponsmith") {
            rangedweaponsmith();
            break;
        } else if (villageExploreables[answer - 1] == "News Board") {
            villagenewsboard();
            break;
        } else if (villageExploreables[answer - 1] == "Butcher Shop") {

            break;
        } else {
            std::cout << "Invalid answer, try again.\n";
        }
    }
    }
}

void explore() { //exploration script

    for (int i = 0; i < playerExploreables.size(); i++) {

        std::cout << "\n" << i + 1 << ")" << playerExploreables[i];

    }
    std::cout << "\nWhere will you explore? (Type 0 to cancel explore)\nI will explore choice #";
    answer = 1;
    while (true) {
        std::cin >> answer;
        if (std::cin.fail()) {
            std::cout << "Invalid number, try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
        if (answer == 0) {
            break;
        } else if (playerExploreables[answer - 1] == "The Woods") {
            thewoods();
            break;
        } else if (playerExploreables[answer - 1] == "The Village") {
            thevillageexploreable();
            break;
        } else if (playerExploreables[answer - 1] == "The Mines") {
            //the mines where you can get ores and sell them for coins, will need pickaxe
            break;
        } else {
            std::cout << "Invalid number, try again\n";
        }
    }
    }
}

void currentquests() { //prints everything in quests

    if (playerQuests.size() == 0) {
        std::cout <<"You do not have any quests.\n";
    } else {

    std::cout << "Your current quests:\n\n";
    for (int i = 0; i <= playerQuests.size(); i++) {

        std::cout << playerQuests[i] << "\n";

    }
    std::cout << "\n";
}
}

void checkinventory() { //prints everything in inventory

    if (playerInventory.size() == 0) {
        std::cout << "You do not have anything in your inventory.\n";
    } else {

    std::cout << "Your inventory:\n\n";
    for (int i = 0; i <= playerInventory.size() - 1; i++) {

        std::cout << playerInventory[i] << ", Strength: " << playerInventoryStrength[i] << "\n";

    }
    std::cout << "\n";
}
}

void currentplayerstats() {//prints current player stats

    std::cout << "Current Player Stats:\nHealth: " << playerHealth << "/" << maxPlayerHealth << "\nStrength: " << playerStrength << "\nSpeed: " << playerSpeed << "\nTotal XP: " << playerXP << "\nCoins: " << playerCoins << "\nLuck: " << playerLuck << "\nArmor Level: " << armorLevel << "\n";

}

void game() { //actual gameplay

    //start game

    std::cout << "\n\n\n\n\n\n\n\n\n-----------\nNew Game\n-----------\n";

    currentplayerstats();
    std::cout << "These are your current player stats.\nThey will show up when you are making decisions based on player stats, or when you request it.";

    std::cout << "\n\nPlease select which you'd like to put 3 more points into:\n1) Strength\n2) Speed\n";

    answer = 0;
    while (true) {

        std::cin >> answer;

        if (std::cin.fail() || (answer != 1 && answer != 2)) {
            std::cout << "Invalid number, please input 1 or 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {

        if (answer == 1) {

            playerStrength = playerStrength + 3;

        } else if (answer == 2) {

            playerSpeed = playerSpeed + 3;

        }
        break;
    }
    }

    currentplayerstats();

    std::cout << "\n\nWould you like to have an extra 2 max health points to sacrifice 2 speed points and 2 strength points?\n1) Yes\n2) No\n";

    answer = 0;
    while (true) {

        std::cin >> answer;

        if (std::cin.fail() || (answer != 1 && answer != 2)) {
            std::cout << "Invalid number, please input 1 or 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {

        if (answer == 1) {

            playerStrength = playerStrength - 2;
            playerSpeed = playerSpeed - 2;
            maxPlayerHealth = maxPlayerHealth + 2;
            playerHealth = playerHealth + 2;

        } else if (answer == 2) {

            // do nothing

        }
        break;
    }
    }

    currentplayerstats();
    std::cout << "These are your starting stats. Press any button and hit enter to continue.";
    
    std::string tempans;
    std::cin >> tempans;

    std::cout << "\nHere is a starting weapon, a basic wooden sword.\n";

    addtoinventory("Wooden Sword", 1, 4.5); //name, type, strength
    addtoinventory("Healing Potion", 2, 2);

    //Actual game start

    std::string toDo;
    answer = 0;

    while (toDo != "Exit") {
        std::cout << "What should you do? (type 'Help' for commands)\nCurrent Coins: " << playerCoins << "\n";
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
        } else if (testing == true && toDo == "ac") {
            std::cout << "Testing - give coins, insert amount: ";
            std::cin >> answer;
            playerCoins = playerCoins + answer;
        } else if (testing == true && toDo == "addinv") {
            std::cout << "\nname ";
            std::cin >> string1;

            std::cout << "\ntype ";
            std::cin >> int1;

            std::cout << "\nstrength ";
            std::cin >> double1;

            addtoinventory(string1, int1, double1);
        } else if (testing == true && toDo == "reminv") {
            std::cout << "\ninventory # (start 0) ";
            std::cin >> int1;

            removefrominventory(int1);
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

    while (true) {

        std::cin >> answer;

        if (std::cin.fail() || (answer != 1 && answer != 2)) {
            std::cout << "Invalid number, please input 1 or 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {

            if (answer == 1) {

                //Load Game State, but for now it's coming soon
                std::cout << "This feature is coming soon, please select 'new game'\n";
                answer = 3; //sets to 3 so it  can re-ask for input

            } else if (answer == 2) {

                game();

            }
            break;
        }

    }
}