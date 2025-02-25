#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <windows.h>
using namespace std;

struct Games {
    string name, developerCompany;
    int releaseYear, copiesSold;
    double price;
};

void parseCSV(string line, Games &game) {
    stringstream ss(line);
    string temp;

    getline(ss, game.name, ',');
    getline(ss, temp, ',');
    game.releaseYear = stoi(temp);
    getline(ss, temp, ',');
    game.price = stod(temp);
    getline(ss, temp, ',');
    game.copiesSold = stoi(temp);
    getline(ss, game.developerCompany, ',');
}

void readCSV(string fileName, vector<Games> &games) {
    ifstream fin(fileName);
    if (fin) {
        string line;
        getline(fin, line);
        while (getline(fin, line)) {
            Games game;
            parseCSV(line, game);
            games.push_back(game);
        }
        fin.close();
    } else {
        cout << "Error: Unable to open file " << fileName << endl;
    }
}

void displayGames(const vector<Games> &games) {
    if (games.empty()) {
        cout << "No games found :(\n";
        return;
    }

    cout << left << fixed << setprecision(2);
    cout << setw(35) << "Game Name"
            << setw(8) << "Year"
            << setw(12) << "Price"
            << setw(15) << "Copies Sold"
            << "Developer Company\n";

    cout << string(80, '-') << endl;
    SetConsoleOutputCP(1252);
    for (const auto &game: games) {
        cout << setw(35) << game.name.substr(0, 32)
                << setw(8) << game.releaseYear
                << (char) 128 <<
                setw(11) << game.price
                << setw(15) << (to_string(game.copiesSold) + "m")
                << game.developerCompany << endl;
    }
    cout << endl;
}

int searchByName(const vector<Games> &games, const string &name) {
    for (size_t i = 0; i < games.size(); ++i) {
        if (games[i].name == name) return i;
    }
    return -1;
}

map<string, int> countByDeveloper(const vector<Games> &games) {
    map<string, int> counts;
    for (const auto &game: games) {
        counts[game.developerCompany]++;
    }
    return counts;
}

void displayByDeveloper(const vector<Games> &games, const string &developer) {
    vector<Games> results;
    for (const auto &game: games) {
        if (game.developerCompany == developer) {
            results.push_back(game);
        }
    }
    if (results.empty()) {
        cout << "No games from " << developer << endl;
    } else {
        displayGames(results);
    }
}

int calculateSalesStats(const vector<Games> &games, Games &maxGame, Games &minGame) {
    if (games.empty()) return -1;

    int maxSales = games[0].copiesSold;
    int minSales = games[0].copiesSold;
    int total = 0;
    maxGame = minGame = games[0];

    for (const auto &game: games) {
        total += game.copiesSold;
        if (game.copiesSold > maxSales) {
            maxSales = game.copiesSold;
            maxGame = game;
        }
        if (game.copiesSold < minSales) {
            minSales = game.copiesSold;
            minGame = game;
        }
    }
    return total / games.size();
}

vector<Games> searchPartialMatch(const vector<Games> &games, const string &text) {
    vector<Games> matches;
    for (const auto &game: games) {
        if (game.name.find(text) != string::npos) {
            matches.push_back(game);
        }
    }
    return matches;
}

void displaySortedByPrice(vector<Games> games) {
    sort(games.begin(), games.end(), [](const Games &a, const Games &b) {
        return a.price > b.price;
    });
    displayGames(games);
}

void showMenu() {
    cout << "\n* * * * * GAME DATABASE MENU * * * * *\n"
            << "1. Display all games\n"
            << "2. Search game by name\n"
            << "3. Show game count by developer\n"
            << "4. Filter games by developer\n"
            << "5. Show sales statistics\n"
            << "6. Search games by partial name\n"
            << "7. Display games sorted by price\n"
            << "8. Exit\n"
            << "Enter your choice (1-8): ";
}

int main() {
    vector<Games> games;
    readCSV("games_data.csv", games);

    int choice;
    do {
        showMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                cout << "\nAll Games:\n";
                displayGames(games);
                break;
            }
            case 2: {
                string name;
                cout << "Enter game name to search: ";
                getline(cin, name);
                int index = searchByName(games, name);
                if (index != -1) {
                    cout << "\nGame Found:\n";
                    displayGames({games[index]});
                } else {
                    cout << "Game not found!\n";
                }
                break;
            }
            case 3: {
                auto counts = countByDeveloper(games);
                cout << "\nGames per Developer:\n";
                cout << left << setw(25) << "Developer" << "Game Count\n";
                cout << string(35, '-') << endl;
                for (const auto &entry: counts) {
                    cout << setw(25) << entry.first << entry.second << endl;
                }
                cout << endl;
                break;
            }
            case 4: {
                string developer;
                cout << "Enter developer name: ";
                getline(cin, developer);
                cout << "\nGames From " << developer << ":\n";
                displayByDeveloper(games, developer);
                break;
            }
            case 5: {
                Games maxGame, minGame;
                int avg = calculateSalesStats(games, maxGame, minGame);
                if (avg != -1) {
                    cout << "\nSales Statistics:\n"
                            << "Average copies sold: " << avg << "M\n"
                            << "Highest seller: " << maxGame.name << " (" << maxGame.copiesSold << "M)\n"
                            << "Lowest seller: " << minGame.name << " (" << minGame.copiesSold << "M)\n";
                } else {
                    cout << "No sales data available\n";
                }
                break;
            }
            case 6: {
                string text;
                cout << "Enter search text: ";
                getline(cin, text);
                auto matches = searchPartialMatch(games, text);
                cout << "\nSearch results:\n";
                if (!matches.empty()) {
                    displayGames(matches);
                } else {
                    cout << "No results found\n";
                }
                break;
            }
            case 7: {
                cout << "\nGames Sorted by Price (Descending Order):\n";
                displaySortedByPrice(games);
                break;
            }
            case 8:
                cout << "Finishing execution\n";
                break;
            default:
                cout << "Invalid option :( Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}