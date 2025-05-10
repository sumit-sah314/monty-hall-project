/**
 * @file monty_hall.cpp
 * @brief Implements Monty Hall simulation, gameplay logic, and visual output.
 *
 * Contains logic for user interaction, statistical simulations,
 * ASCII door rendering, and persistent logging of results.
 */

 #include "monty_hall.h"
 #include <iostream>
 #include <vector>
 #include <algorithm>
 #include <cstdlib>
 #include <ctime>
 #include <fstream>
 #include <iomanip>
 #include <cstring>
 
 using namespace std;
 
 /**
  * @brief Constructor that sets number of doors and seeds random generator.
  */
 MontyHall::MontyHall(int numberOfDoors) {
     doors = (numberOfDoors < 3) ? 3 : numberOfDoors;
     srand(static_cast<unsigned int>(time(0))); // seed only once
 }
 
 /**
  * @brief Runs a single simulation of the Monty Hall game internally.
  *
  * Randomly selects the prize and player’s initial choice.
  * Simulates Monty revealing one goat door, and the player switching if enabled.
  * @return True if the player ends up choosing the prize door.
  */
 bool MontyHall::playSingleSimulation() {
     prizeDoor = rand() % doors;
     userChoice = rand() % doors;
 
     vector<int> revealCandidates;
     for (int i = 0; i < doors; ++i) {
         if (i != prizeDoor && i != userChoice)
             revealCandidates.push_back(i);
     }
 
     random_shuffle(revealCandidates.begin(), revealCandidates.end());
     hostOpens = revealCandidates[0];
 
     if (switchChoice) {
         for (int i = 0; i < doors; ++i) {
             if (i != userChoice && i != hostOpens) {
                 userChoice = i;
                 break;
             }
         }
     }
 
     return userChoice == prizeDoor;
 }
 
 /**
  * @brief Runs a number of simulation rounds with and without switching.
  *
  * This method prints out statistics comparing both strategies.
  * @param simulations The number of trials to run.
  */
 void MontyHall::runSimulation(int simulations) {
     int switchWins = 0, stayWins = 0;
 
     switchChoice = true;
     for (int i = 0; i < simulations; ++i) {
         if (playSingleSimulation()) switchWins++;
     }
 
     switchChoice = false;
     for (int i = 0; i < simulations; ++i) {
         if (playSingleSimulation()) stayWins++;
     }
 
     cout << "\n=== Monty Hall Simulation Results ===\n";
     cout << "Number of Doors: " << doors << "\n";
     cout << "Simulations per Strategy: " << simulations << "\n\n";
     cout << "Switched  -> Wins: " << switchWins << ", Losses: " << (simulations - switchWins) << "\n";
     cout << "Stayed    -> Wins: " << stayWins << ", Losses: " << (simulations - stayWins) << "\n";
 }
 
 /**
  * @brief Runs the interactive version of the game.
  *
  * Handles player input, door reveals, switching decision,
  * final result display, and logs the game outcome.
  */
 void MontyHall::runInteractiveGame() {
     prizeDoor = rand() % doors;
 
     printIntroDiagram(doors);
     cout << "Pick a door (1 to " << doors << "): ";
     cin >> userChoice;
     userChoice--;
 
     vector<int> revealCandidates;
     for (int i = 0; i < doors; ++i)
         if (i != prizeDoor && i != userChoice)
             revealCandidates.push_back(i);
     random_shuffle(revealCandidates.begin(), revealCandidates.end());
 
     vector<int> revealedGoats(revealCandidates.begin(), revealCandidates.begin() + doors - 2);
 
     cout << "\nMonty opens " << revealedGoats.size() << " goat doors:\n";
     displayDoors(doors, revealedGoats, userChoice);
 
     cout << "Do you want to switch your choice? (y/n): ";
     char response;
     cin >> response;
     switchChoice = (response == 'y' || response == 'Y');
 
     if (switchChoice) {
         for (int i = 0; i < doors; ++i) {
             if (find(revealedGoats.begin(), revealedGoats.end(), i) == revealedGoats.end() && i != userChoice) {
                 userChoice = i;
                 break;
             }
         }
     }
 
     cout << "\nFinal Reveal:\n";
     displayDoors(doors, {}, userChoice, prizeDoor, true);
 
     bool won = (userChoice == prizeDoor);
     if (won)
         cout << ":) You WON the car!\n";
     else
         cout << ":( You got a goat. The car was behind door " << (prizeDoor + 1) << ".\n";
 
     logGameResult("Interactive", doors, switchChoice, won);
 }
 
 /**
  * @brief Prints an intro diagram and game description.
  */
 void printIntroDiagram(int numDoors) {
     cout << "\n   ============================\n";
     cout << "       MONTY HALL - " << numDoors << " DOORS\n";
     cout << "   ============================\n\n";
 
     cout << "    ";
     for (int i = 0; i < numDoors; ++i)
         cout << "[" << (i + 1) << "]     ";
     cout << "\n   ";
     for (int i = 0; i < numDoors; ++i)
         cout << "+-----+ ";
     cout << "\n   ";
     for (int i = 0; i < numDoors; ++i)
         cout << "| ??? | ";
     cout << "\n   ";
     for (int i = 0; i < numDoors; ++i)
         cout << "+-----+ ";
     cout << "\n";
 
     cout << "\n   - One door hides a CAR\n";
     cout << "   - The others hide GOATS\n";
     cout << "   - Monty will reveal all goat doors except one\n";
 }
 
 /**
  * @brief Shows visual state of all doors with optional prize/goat reveal.
  */
 void displayDoors(int numDoors, vector<int> revealedGoats, int userPick, int prize, bool final) {
     cout << "\n   Doors:\n   ";
     for (int i = 0; i < numDoors; ++i)
         cout << "[" << (i + 1) << "]     ";
     cout << "\n   ";
     for (int i = 0; i < numDoors; ++i)
         cout << "+-----+ ";
     cout << "\n   ";
     for (int i = 0; i < numDoors; ++i) {
         if (final) {
             if (i == prize) cout << "| CAR | ";
             else cout << "|GOAT | ";
         } else if (find(revealedGoats.begin(), revealedGoats.end(), i) != revealedGoats.end()) {
             cout << "|GOAT | ";
         } else {
             cout << "| ??? | ";
         }
     }
     cout << "\n   ";
     for (int i = 0; i < numDoors; ++i)
         cout << "+-----+ ";
     cout << "\n";
 }
 
 /**
  * @brief Logs the result of a game or simulation to a local file.
  */
 void logGameResult(const string& mode, int doors, bool switched, bool won) {
     ofstream file("game_stats.txt", ios::app);
     if (!file) return;
 
     time_t now = time(0);
     char* dt = ctime(&now);
     dt[strcspn(dt, "\n")] = 0;
 
     file << "[" << dt << "] Mode: " << mode
          << ", Doors: " << doors
          << ", Switched: " << (switched ? "Yes" : "No")
          << ", Result: " << (won ? "Win" : "Loss") << "\n";
 
     file.close();
 }
 
 /**
  * @brief Displays all logged game history from file.
  */
 void viewGameStats() {
     ifstream file("game_stats.txt");
     if (!file) {
         cout << "\nNo statistics recorded yet.\n";
         return;
     }
 
     cout << "\n=== Game History ===\n";
     string line;
     while (getline(file, line)) {
         cout << line << "\n";
     }
     file.close();
 }
 
