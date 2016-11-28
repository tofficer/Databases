#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

using namespace std;

static vector<string> get_contents(ifstream& infile) {
	vector<string> lines;
	string line;
	for (int i = 0; getline(infile, line); ++i) {
		lines.push_back(line);
	}

	return lines;
}

int main(int argc, char* argv[]) {
	ifstream infile;
	infile.open(argv[1]);
	vector<string> lines = get_contents(infile);
	infile.close();

	//unordered map that takes in first col string as key and has a float array as a value
	//first element in array stores count of second col, second element stores sum of second col, 
	//third element stores if a third col nonempty value has been seen, fourth stores the max of the third col
	unordered_map< string, array<float, 4> > map;
	int num_buckets = lines.size()/0.75;
	map.reserve(num_buckets);

	for (int i = 0; i < lines.size(); ++i) {
		string line = lines.at(i);
		vector<string> row;
		string col_data;
		stringstream ss(line);
		while (getline(ss, col_data, ',')) {
			row.push_back(col_data);
		}
		
		string key = row[0];
		float f_col = (row[1].empty()) ? 0 : stof(row[1]);
		float s_col = (row[2].empty()) ? 0 : stof(row[2]);
		
		array<float, 4> value = {0, f_col, 0, s_col};

		if (map.find(key) == map.end()) {
			if (!row[1].empty()) value.at(0) = 1;
			if (!row[2].empty()) value.at(2) = 1;
		}
		else {
			value = map.at(key);
			if (!row[1].empty()) {
				value.at(0) += 1;
				value.at(1) += f_col;
			}
			if (!row[2].empty()) {
				value.at(3) = max(value.at(3), s_col);
			}
		}
		map[key] = value;
	}

	ofstream outfile;
	outfile.open("output.dat");

	//write to output.dat
	for ( const auto &pair : map ) {
		outfile << pair.first << '\t';
		array<float, 4> value = pair.second;
		if (value.at(0) > 0) outfile << value.at(1)/value.at(0) << '\t';
		else outfile << "NULL" << '\t';
		if (value.at(2) > 0) outfile << value.at(3) << '\n';
		else outfile << "NULL" << '\n';
	}
   	outfile << endl;
   	outfile.close();

   	return 0;
}
