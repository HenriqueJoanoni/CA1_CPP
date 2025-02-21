#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

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

/** FUNCTION 1 */
void displayGames(const vector<Games>& games) {
    for (const auto& game : games) {
        cout << "Name: " << game.name << "\n"
             << "  Year: " << game.releaseYear
             << ", Price: $" << game.price
             << ", Copies Sold: " << game.copiesSold
             << "M\n  Developer: " << game.developerCompany << "\n\n";
    }
}

/** FUNCTION 2 */
int searchByName(const vector<Games>& games, const string& name) {
    for (size_t i = 0; i < games.size(); ++i) {
        if (games[i].name == name) return i;
    }
    return -1;
}

/** FUNCTION 3 */
map<string, int> countByDeveloper(const vector<Games>& games) {
    map<string, int> counts;
    for (const auto& game : games) {
        counts[game.developerCompany]++;
    }
    return counts;
}

/** FUNCTION 4 */
void displayByDeveloper(const vector<Games>& games, const string& developer) {
    vector<Games> results;
    for (const auto& game : games) {
        if (game.developerCompany == developer) {
            results.push_back(game);
        }
    }
    if (results.empty()) {
        cout << "No games found from developer: " << developer << endl;
    } else {
        displayGames(results);
    }
}

/** FUNCTION 5 */
int calculateSalesStats(const vector<Games>& games, Games& maxGame, Games& minGame) {
    if (games.empty()) return -1;

    int maxSales = games[0].copiesSold;
    int minSales = games[0].copiesSold;
    int total = 0;
    maxGame = minGame = games[0];

    for (const auto& game : games) {
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

/** FUNCTION 6 */
vector<Games> searchPartialMatch(const vector<Games>& games, const string& text) {
    vector<Games> matches;
    for (auto it = games.begin(); it != games.end(); ++it) {
        if (it->name.find(text) != string::npos) {
            matches.push_back(*it);
        }
    }
    return matches;
}

/** FUNCTION 7 */
void displaySortedByPrice(vector<Games> games) {
    sort(games.begin(), games.end(), [](const Games& a, const Games& b) {
        return a.price > b.price;
    });
    displayGames(games);
}

int main() {
    vector<Games> games;
    readCSV("games_data.csv", games);

    /** DISPLAY ALL GAMES */
    cout << "* * * All Games * * *\n";
    displayGames(games);

    /** SEARCH FOR A GAME */
    // string searchTerm = "Call of Duty: Modern Warfare II";
    // int index = searchByName(games, searchTerm);
    // cout << "* * * Search Results * * *\n";
    // if (index != -1) {
    //     cout << "Game found at index " << index << ":\n";
    //     cout << games[index].name << " (" << games[index].releaseYear << ")\n";
    // } else {
    //     cout << "Game not found: " << searchTerm << "\n";
    // }

    /** COUNT BY DEVELOPER */
    // cout << "\n* * * Developer Counts * * *\n";
    // auto devCounts = countByDeveloper(games);
    // for (const auto& entry : devCounts) {
    //     cout << entry.first << ": " << entry.second << " games\n";
    // }

    /** FILTER BY DEVELOPER */
    // cout << "\n* * * Games by Nintendo * * *\n";
    // displayByDeveloper(games, "Nintendo");

    /** SALES STATISTICS */
    // Games maxGame, minGame;
    // int avg = calculateSalesStats(games, maxGame, minGame);
    // cout << "\n* * * Sales Statistics * * *\n";
    // cout << "Average copies sold: " << avg << "M\n"
    //      << "Best Seller: " << maxGame.name << " (" << maxGame.copiesSold << "M)\n"
    //      << "Lowest Seller: " << minGame.name << " (" << minGame.copiesSold << "M)\n";

    /** PARTIAL MATCH SEARCH */
    // cout << "\n* * * Games with 'The' in title * * *\n";
    // auto textMatches = searchPartialMatch(games, "The");
    // displayGames(textMatches);

    /** SORT BY PRICE */
    // cout << "\n* * * Games Sorted by Price * * *\n";
    // displaySortedByPrice(games);

    return 0;
}