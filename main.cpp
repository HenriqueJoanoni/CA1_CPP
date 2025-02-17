#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/** GAMES */
struct Games {
    string name, developerCompany;
    int releaseYear, copiesSold;
    double price;
};

/** PARSE A SINGLE LINE FROM THE CSV AND POPULATE THE GAMES STRUCT */
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

int main() {
    vector<Games> games;
    readCSV("games_data.csv", games);
    for (const auto &game: games) {
        cout << "Name: " << game.name << ", "
                << "Release Year: " << game.releaseYear << ", "
                << "Price: " << game.price << ", "
                << "Copies Sold: " << game.copiesSold << ", "
                << "Developer Company: " << game.developerCompany << endl;
    }
    return 0;
}
