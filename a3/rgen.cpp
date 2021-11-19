#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>

using namespace std;

int rand(unsigned int k1, unsigned int k2) {

	std::ifstream urandom("/dev/urandom");

	while (true) {

		char ch = 'a';
		int k = k2 - k1;
		urandom.read(&ch, 1);
		if (0 <= (unsigned int)ch & (unsigned int)ch <= k) {
			return (unsigned int)ch + k1;
			exit(0);
		}
	}
}



class Point {
private:
	int x;
	int y;
public:
	vector<int> get_coord() {
		vector<int> coord;
		coord.push_back(x);
		coord.push_back(y);
		return coord;
	};
	int getx() {
		return this->x;
	}
	int gety() {
		return this->y;
	}
	Point(int intRange) {
		int negNum = (-1) * intRange;
		++intRange;
		this->x = rand(-intRange, intRange);
		this->y = rand(-intRange, intRange);
	};
	Point() {
	};

};




class Segment {
private:
	Point start;
	Point end;
public:
	Segment(int intRange, Point st = Point(20)) {
		bool flag = false;
		int counter = 0;
		while (!flag) {
			this->start = st;
			this->end = Point(intRange);
			if (this->start.getx() != this->end.getx() || this->start.gety() != this->end.gety()) {
				flag = true;
			}
			else {
				++counter;
				if (counter >= 25) {
					cerr << "Error: Tried producing eligible streets for " << 25 << " times." << endl;
					exit(1);
				}
			}

		}
	};
	Segment() {
	};
	vector<Point> getSegment() {
		vector<Point> segment;
		segment.push_back(this->start);
		segment.push_back(this->end);

		return segment;
	}
	vector<vector<int>> getDetails() {
		vector<vector<int>> points;
		vector <int> start = { this->start.getx(), this->start.gety() };
		vector <int> end = { this->end.getx(), this->end.gety() };
		points.push_back(start);
		points.push_back(end);
		return points;
	}
};

vector <Segment> database;

bool checkOverLap(Segment a, Segment b) {
	bool flag_a = false, flag_b = false;
	double slope1, slope2;
	//first point
	vector<Point> seg_a; seg_a = a.getSegment();
	vector<int> coord1 = seg_a[0].get_coord();
	//second point
	vector<int> coord2 = seg_a[1].get_coord();
	//cout << "First: " << (coord2[1] - coord1[1]) << "  " << (coord2[0] - coord1[0]) << endl;
	if (coord2[0] - coord1[0] == 0) {
		flag_a = true;
	}
	if (!flag_b) {
		slope1 = double(coord2[1] - coord1[1]) / (coord2[0] - coord1[0]);
	}
	//first point
	vector<Point> seg_b; seg_b = b.getSegment();
	vector<int> coord3 = seg_b[0].get_coord();
	//second point
	vector<int> coord4 = seg_b[1].get_coord();
	//cout << "Second: " << (coord4[1] - coord3[1]) << "  " << (coord4[0] - coord3[0]) << endl;
	if (coord4[0] - coord3[0] == 0) {
		flag_b = true;
	}
	if (!flag_b) {
		slope2 = double(coord4[1] - coord3[1]) / (coord4[0] - coord3[0]);
	}
	else {
		slope2 = 1000000000;
	}

	//cout << slope1 << "  " << slope2 << endl;
	if (slope1 == slope2 || (flag_a == true && flag_b == true)) {
		if ((coord1[0] >= coord3[0] && coord1[0] <= coord4[0]) || (coord1[0] >= coord4[0] && coord1[0] <= coord3[0])) {
			if ((coord1[1] >= coord3[1] && coord1[1] <= coord4[1]) || (coord1[1] >= coord4[1] && coord1[1] <= coord3[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else if ((coord3[0] >= coord1[0] && coord3[0] <= coord2[0]) || (coord3[0] >= coord2[0] && coord3[0] <= coord1[0])) {
			if ((coord3[1] >= coord1[1] && coord3[1] <= coord2[1]) || (coord3[1] >= coord2[1] && coord3[1] <= coord1[1])) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

}

string gen_random(int len) {
	string s;
	static const char alphanum[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return s;
}
vector<string> nameDatabase;
class Street {
private:
	string name;
	vector<Segment> segments;
public:
	Street(int intRange, int waitTime, int numSeg) {
		int NSe = rand(1, numSeg);
		int wT = rand(5, waitTime);
		this->name = gen_random(20);
		nameDatabase.push_back(this->name);
		int i = 0;
		int counter = 0;
		Point last;
		Segment segment;

		while (i < NSe) {
			if (counter == 26) {
				cerr << "Error: Tried producing eligible streets for " << 25 << " times." << endl;
				exit(1);
			}
			if (i == 0) {
				segment = Segment(intRange);
				last = segment.getSegment()[1];
			}
			else {
				segment = Segment(intRange, last);
				last = segment.getSegment()[1];
			}
			bool over = false;
			for (auto elem : database)
			{
				over = checkOverLap(elem, segment);
				if (over) {
					++counter;
					break;;
				}

			}
			if (!over) {
				++i;
				this->segments.push_back(segment);
				database.push_back(segment);

				sleep(wT);
				}
		}

	};
			// check whether this segment is overlapping with any of other segments generated so far.
	
	string getName()
	{
		return this->name;
	};
	vector<Segment> getSegments() {
		return this->segments;
	}
	;
};

int main (int argc, char **argv) {
	int defaultNumberOfStreets = 10;
	int NSt;
	int defaultNumberOfSegments = 5;
	int NSe;
	int waitTime = 5; // seconds
	int wT;
	int intRange = 20; //[-20, 20]
	int iR;

	std::string value;
	int option;
	while ((option = getopt(argc, argv, "s:n:l:c:")) != -1) {
		switch (option) {
		case 's': {
			value = optarg;
			defaultNumberOfStreets = atoi(value.c_str());
			if (defaultNumberOfStreets < 2) {
				std::cerr << "Error: The range of the command value is invalid!";
				exit(1);
			}
			break;
		}
		case 'n': {
			value = optarg;
			defaultNumberOfSegments = atoi(value.c_str());
			if (defaultNumberOfSegments < 1) {
				std::cerr << "Error: The range of the command value is invalid!";
				exit(1);
			}
			break;
		}
		case 'l': {
			value = optarg;
			waitTime = atoi(value.c_str());
			if (waitTime < 5) {
				std::cerr << "Error: The range of the command value is invalid!";
				exit(1);
			}
			break;
		}
		case 'c': {
			value = optarg;
			intRange = atoi(value.c_str());
			if (intRange < 1) {
				std::cerr << "Error: The range of the command value is invalid!";
				exit(1);
			}
			break;
		}
		}
	}




	while (!std::cin.eof()) {
		database.clear();
		vector<Street> streets;
		NSt = rand(2, defaultNumberOfStreets);
		for (int cc = 0; cc < nameDatabase.size(); ++cc) {
			cout << "rm " << "\"" << nameDatabase[cc] << "\"" << endl;
		}
		nameDatabase.clear();
		for (int i = 1; i <= NSt; ++i) {
			Street temp(intRange, waitTime, defaultNumberOfSegments);
			streets.push_back(temp);
		}
		for (auto elem : streets) {
			string name = elem.getName();
			vector<Segment> segments = elem.getSegments();
			cout << "add " << "\"" << name << "\"";
			int counter = 0;
			for (auto seg : segments) {
				if (counter == 0) {
					vector <vector<int>> mySeg = seg.getDetails();
					for (auto p : mySeg) {
						cout << " (" << p[0] << "," << p[1] << ")";
					}
				}
				else {
					vector <vector<int>> mySeg = seg.getDetails();
					cout << " (" << mySeg[1][0] << "," << mySeg[1][1] << ")";
				}
				++counter;
			}
			cout << endl;
		}

		cout << "gg" << endl;

	}
	return 0;
}
