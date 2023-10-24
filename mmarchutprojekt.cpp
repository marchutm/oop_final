#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

struct player {
	string name;
	string position;
	int age;
	int overall;
	string nationality;
};

class csv_file {
	string fileName;
	int numColumns;
	int numLines;

	// Cache to store the counts for each file
	map<string, pair<int, int>> cache;

	// Vector to store the data from the CSV file
	vector<vector<string>> data;

public:
	//Constructor
	csv_file(const string& fileName) {
		this->fileName = fileName;
		numColumns = 0;
		numLines = 0;
		// Check if the counts are already in the cache
		if (cache.count(fileName) > 0) {
			numColumns = cache[fileName].first;
			numLines = cache[fileName].second;
		}
		else {
			// Open the file and count the number of columns and lines
			filebuf fb;
			if (fb.open(fileName, ios::in)) {
				istream is(&fb);
				string row;
				while (getline(is, row)) {
					numLines++;

					// Split the row into columns
					stringstream ss(row);
					string value;
					int currentColumn = 0;
					while (getline(ss, value, ',')) {
						currentColumn++;
					}

					// Update the number of columns if needed
					if (currentColumn > numColumns) {
						numColumns = currentColumn;
					}
				}

				// Add the counts to the cache
				cache[fileName] = make_pair(numColumns, numLines);
			}
		}

		// Resize the data vector
		data.resize(numLines);
		for (int i = 0; i < numLines; i++) {
			data[i].resize(numColumns);
		}

		// Open the file and store the data in the vector
		filebuf fb;
		if (fb.open(fileName, ios::in)) {
			istream is(&fb);
			string row;
			int currentLine = 0;
			while (getline(is, row)) {
				currentLine++;

				// Split the row into columns
				stringstream ss(row);
				string value;
				int currentColumn = 0;
				while (getline(ss, value, ',')) {
					data[currentLine - 1][currentColumn] = value;
					currentColumn++;
				}
			}
		}
	}

	// Method to return the number of columns
	int getNumColumns() const {
		return numColumns;
	}

	// Method to return the number of lines
	int getNumLines() const {
		return numLines;
	}

	// Method to read a specific cell
	string readCell(int column, int line) {
		// Check if the column and line indices are valid
		if (column < 1 || column > numColumns || line < 1 || line > numLines) {
			return "";
		}

		// Return the cell value from the data vector
		return data[line - 1][column - 1];
	}

	// Method to release the memory used by the data vector and the cache
	void cleanMemory() {
		// Clear the data vector
		for (auto& column : data) {
			column.clear();
		}
		data.clear();

		// Clear the cache
		cache.clear();
	}
};

// Loads data from a CSV file into a vector of player objects.
// Iterates through each line of the file (starting from the third line) and creates a player object for each line.
// Sets the fields of the player object by reading the corresponding cell values from the CSV file and storing them in the player object.
// Pushes the player object onto the end of the vector.
void load_data(csv_file& file, vector<player>& players) {
	for (int i = 2; i < file.getNumLines(); i++) {
		player p;
		p.name = file.readCell(2, i);
		p.position = file.readCell(62, i);
		p.age = stoi(file.readCell(3, i));
		p.overall = stoi(file.readCell(7, i));
		p.nationality = file.readCell(5, i);

		players.push_back(p);
	}
}

// Prints statistical information about the positions of players in a given vector.
// Creates a map with the position names as keys and the number of players in each position as values.
// Iterates through the players in the vector and increments the count for each position in the map.
// Sorts the positions in the map in descending order based on the number of players in each position.
// Prints the position names, the number of players in each position, and the percentage of players in each position.
void position_stats(const vector<player>& players) {
	map<string, int> positions;

	for (const auto& player : players) {
		positions[player.position]++;
	}
	int sum = 0;
	for (const auto& position : positions) {
		sum += position.second;
	}

	vector <pair<string, int>> sorted_positions(positions.begin(), positions.end());
	sort(sorted_positions.begin(), sorted_positions.end(), [](const pair<string, int>& a, const pair<string, int>& b) { return a.second > b.second; });

	for (const auto& position : sorted_positions) {
		cout << position.first << ":     " << position.second << "     " << (static_cast<float>(position.second) / sum) * 100 << "%" << endl;
	}
}

// Prints statistical information about the ages of players in a given vector.
// Creates a map with age ranges as keys and the number of players in each age range as values.
// Iterates through the players in the vector and increments the count for the appropriate age range in the map.
// Prints the age ranges, the number of players in each age range, and the percentage of players in each age range.
void age_stats(const vector<player>& players) {
	map<string, int> ages;

	for (auto& player : players) {
		if (player.age >= 16 && player.age < 20) {
			ages["16-20"]++;
			continue;
		}

		if (player.age >= 21 && player.age < 24) {
			ages["21-24"]++;
			continue;
		}

		if (player.age >= 25 && player.age < 29) {
			ages["25-28"]++;
			continue;
		}

		if (player.age >= 29 && player.age < 33) {
			ages["29-32"]++;
		}
		if (player.age >= 33) {
			ages["33+"]++;
		}
	}
	int sum = 0;
	for (const auto& age : ages) {
		sum += age.second;
	}
	for (const auto& age : ages) {
		cout << age.first << ":     " << age.second << "     " << (static_cast<float>(age.second) / sum) * 100 << "%" << endl;
	}
}

// Prints statistical information about the overall ratings of players in a given vector.
// Creates a map with overall rating ranges as keys and the number of players in each range as values.
// Iterates through the players in the vector and increments the count for the appropriate overall rating range in the map.
void overall_stats(const vector<player>& players) {
	map <string, int> overalls;

	for (auto& player : players) {
		if (player.overall >= 47 && player.overall < 58) {
			overalls["47-58"]++;
			continue;
		}
		if (player.overall >= 58 && player.overall < 63) {
			overalls["58-63"]++;
			continue;
		}
		if (player.overall >= 63 && player.overall < 67) {
			overalls["63-66"]++;
			continue;
		}
		if (player.overall >= 67 && player.overall < 73) {
			overalls["67-72"]++;
			continue;
		}
		if (player.overall >= 73 && player.overall < 80) {
			overalls["73-79"]++;
			continue;
		}
		if (player.overall >= 80 && player.overall < 84) {
			overalls["80-83"]++;
			continue;
		}
		if (player.overall >= 84 && player.overall < 95) {
			overalls["84-94"]++;
			continue;
		}
		if (player.overall >= 95) {
			overalls["95+"]++;
			continue;
		}

	}
	int sum = 0;
	for (const auto& overall : overalls) {
		sum += overall.second;
	}
	for (const auto& overall : overalls) {
		cout << overall.first << ":     " << overall.second << "     " << (static_cast<float>(overall.second) / sum) * 100 << "%" << endl;
	}
}

// Prints the names and ages of the oldest and youngest players in a given vector.
// Makes a copy of the vector and sorts it by age in ascending order.
// Finds the age of the oldest and youngest players in the sorted vector.
// Prints the names and ages of all players with the oldest or youngest age.
void first_last_age(vector<player> players) { //copying, because elems are sorted
	sort(players.begin(), players.end(), [](const player& a, const player& b) { return a.age < b.age; });
	int highest = players[0].age;
	int lowest = players[players.size() - 1].age;
	int i = 0;
	while (players[i].age == highest) {
		cout << players[i].name << " " << players[i].age << endl;
		i++;
	}
	i = players.size() - 1;
	while (players[i].age == lowest) {
		cout << players[i].name << " " << players[i].age << endl;
		i--;
	}
}

//Same thing justs overall parameter
void first_last_overall(vector<player> players) { //copying, because elems are sorted
	sort(players.begin(), players.end(), [](const player& a, const player& b) { return a.overall < b.overall; });
	int highest = players[0].overall;
	int lowest = players[players.size() - 1].overall;
	int i = 0;
	while (players[i].overall == highest) {
		cout << players[i].name << " " << players[i].overall << endl;
		i++;
	}
	i = players.size() - 1;
	while (players[i].overall == lowest) {
		cout << players[i].name << " " << players[i].overall << endl;
		i--;
	}
}

// Prints statistical information about the nationalities of players in a given vector.
// Creates a map with country names as keys and the number of players from each country as values.
// Iterates through the players in the vector and increments the count for each player's nationality in the map.
// Finds the country with the highest and lowest number of players and prints the name and count for each.
void country_stats(const vector<player>& players)
{
	map<string, int> countries;
	for (auto& player : players)
	{
		countries[player.nationality]++;
	}
	//find highest and lowest value and print them
	int highest = 0;
	string highest_country;
	int lowest = INT32_MAX;
	string lowest_country;
	for (auto& country : countries) {
		if (country.second > highest) {
			highest = country.second;
			highest_country = country.first;
		}
		if (country.second < lowest) {
			lowest = country.second;
			lowest_country = country.first;
		}
	}
	cout << highest_country << " " << highest << endl;
	cout << lowest_country << " " << lowest << endl;

}

int main() {

	//time start
	auto start = chrono::high_resolution_clock::now();
	csv_file fifa1("FIFA20_official_data.csv");
	csv_file fifa2("FIFA21_official_data.csv");
	csv_file fifa3("FIFA22_official_data.csv");

	vector<player> players1, players2, players3;

	load_data(fifa1, players1);
	load_data(fifa2, players2);
	load_data(fifa3, players3);

	fifa1.cleanMemory();
	fifa2.cleanMemory();
	fifa3.cleanMemory();

	auto stop = chrono::high_resolution_clock::now();

	cout << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " ms" << endl << endl;

	cout << "*************************" << endl << "FIFA 20:" << endl << "*************************" << endl;
	cout << "Position stats: " << endl;
	position_stats(players1);
	cout << "*************************" << endl << "Age stats: " << endl << "*************************" << endl;
	age_stats(players1);
	cout << "*************************" << endl << "Overall stats: " << endl << "*************************" << endl;
	overall_stats(players1);
	cout << "*************************" << endl << "Highest and lowest age: " << endl << "*************************" << endl;
	first_last_age(players1);
	cout << "*************************" << endl << "Highest and lowest overall: " << endl << "*************************" << endl;
	first_last_overall(players1);
	cout << "*************************" << endl << "Country stats: " << endl << "*************************" << endl;
	country_stats(players1);
	cout << endl;

	cout << "*************************" << endl << "FIFA 21:" << endl << "*************************" << endl;
	cout << "Position stats: " << endl;
	position_stats(players2);
	cout << "*************************" << endl << "Age stats: " << endl << "*************************" << endl;
	age_stats(players2);
	cout << "*************************" << endl << "Overall stats: " << endl << "*************************" << endl;
	overall_stats(players2);
	cout << "*************************" << endl << "Highest and lowest age: " << endl << "*************************" << endl;
	first_last_age(players2);
	cout << "*************************" << endl << "Highest and lowest overall: " << endl << "*************************" << endl;
	first_last_overall(players2);
	cout << "*************************" << endl << "Country stats: " << endl << "*************************" << endl;
	cout << endl;
	country_stats(players2);

	cout << "*************************" << endl << "FIFA 22:" << endl << "*************************" << endl;
	cout << "Position stats: " << endl;
	position_stats(players3);
	cout << "*************************" << endl << "Age stats: " << endl << "*************************" << endl;
	age_stats(players3);
	cout << "*************************" << endl << "Overall stats: " << endl << "*************************" << endl;
	overall_stats(players3);
	cout << "*************************" << endl << "Highest and lowest age: " << endl << "*************************" << endl;
	first_last_age(players3);
	cout << "*************************" << endl << "Highest and lowest overall: " << endl << "*************************" << endl;
	first_last_overall(players3);
	cout << "*************************" << endl << "Country stats: " << endl << "*************************" << endl;
	country_stats(players3);
}