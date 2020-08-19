/* 
Cellular Automata Project
Erik Hale

/// Things to work on 
> Edges / edge cases: what happens when you get to an edge, does it register 1 or 0?
	>> (Implemented) This could take the form of adding another character to the ends of the vectors and checking them there
	>> (Implemented Instead) Create a "Loop around" variant, where the first and last element check each other 
> Color / Display: what is shown to the screen?
	>> A model that shows how long a cell has been on 
	>> A model that changes the color of an individual cell in the next generation (A color for each rule: 8 diff colors)
> Additions: Anything extra
	>> (Implemented) Adding a little box at the top left to describe the rule set of the cells

/// Fundamental Probelms
> (Fixed) Colors inputs are wrong RBG btw
*/

#include <iostream>
#include <ostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

void printLine(vector <int> vectToDisplay, ofstream& output) {
	// Print the line to a file or to another output 
	static int currentBrightness = 255;
	for (int currentSpot = 0; currentSpot < vectToDisplay.size(); currentSpot++)
		if (vectToDisplay.at(currentSpot) == 2)
			output << currentBrightness / 2 << " " << currentBrightness << " " << currentBrightness  << endl;
		else if (vectToDisplay.at(currentSpot) == 1)
			output << currentBrightness << " " << currentBrightness / 2 << " " << currentBrightness << endl;
		else
			output << 0 << " " << 0 << " " << 0 << endl;
	output << endl;
}

void printLine(vector <int> vectToDisplay, ofstream& output, int red, int blue, int green) {
	// Print the line to a file or to another output but allows you to pick a color for the pixel
	for (int currentSpot = 0; currentSpot < vectToDisplay.size(); currentSpot++)
		if (vectToDisplay.at(currentSpot) == 2)
			output << red / 2 << " " << blue / 2 << " " << green / 2 << endl;
		else if (vectToDisplay.at(currentSpot) == 1)
			output << red << " " << blue << " " << green << endl;
		else
			output << 0 << " " << 0 << " " << 0 << endl;
	output << endl;
}

void printLine(vector <int>& vectToDisplay) {
	// This function is to print the line to the console
	for (int current = 0; current < vectToDisplay.size(); current++)
		cout << vectToDisplay.at(current);
	cout << endl;
}

void printRuleSet(int width, int length, ofstream& output, int red, int blue, int green, string gen) {
	// This is to show the rule set of the Wolfram Cells by adding 4 extra lines to the file
	int lengthOfRuleSet = 33;

	if ((red > 240 && blue > 240 && green > 240) || (red < 15 && blue < 15 && green < 15)) {
		red = blue = green = 255 / 2;
	}

	if (width >= lengthOfRuleSet) {
		// if the width is less than 32, we won't have enough room to print the ruleset to the screen
		// First white line
		for (int i = 0; i < lengthOfRuleSet; i++)
			output << "255 255 255" << endl;
		for (int i = 0; i < width - lengthOfRuleSet; i++) {
			output << "0 0 0" << endl;
		}

		// The Second Line
		output << "255 255 255" << endl << red << " " << blue << " " << green << endl << red << " " << blue << " " << green << endl << red << " " << blue << " " << green << endl;
		output << "255 255 255" << endl << red << " " << blue << " " << green << endl << red << " " << blue << " " << green << endl << "0 0 0" << endl;
		output << "255 255 255" << endl << red << " " << blue << " " << green << endl << "0 0 0" << endl << red << " " << blue << " " << green << endl;
		output << "255 255 255" << endl << red << " " << blue << " " << green << endl << "0 0 0" << endl << "0 0 0" << endl;
		output << "255 255 255" << endl << "0 0 0" << endl << red << " " << blue << " " << green << endl << red << " " << blue << " " << green << endl;
		output << "255 255 255" << endl << "0 0 0" << endl << red << " " << blue << " " << green << endl << "0 0 0" << endl;
		output << "255 255 255" << endl << "0 0 0" << endl << "0 0 0" << endl << red << " " << blue << " " << green << endl;
		output << "255 255 255" << endl << "0 0 0" << endl << "0 0 0" << endl << "0 0 0" << endl << "255 255 255" << endl;
		for (int i = 0; i < width - lengthOfRuleSet; i++) {
			output << "0 0 0" << endl;
		}

		// Third line
		for (int i = 0; i < 8; i++) {
			output << "255 255 255" << endl;
			output << "0 0 0" << endl;
			if ((gen.at(i) - '0') == 1)
				output << red << " " << blue << " " << green << endl;
			else
				output << "0 0 0" << endl;
			output << "0 0 0" << endl;
		}
		output << "255 255 255" << endl;
		for (int i = 0; i < width - lengthOfRuleSet; i++) {
			output << "0 0 0" << endl;
		}

		// Last White Line
		for (int i = 0; i < lengthOfRuleSet; i++)
			output << "255 255 255" << endl;
		for (int i = 0; i < width - lengthOfRuleSet; i++) {
			output << "0 0 0" << endl;
		}
	}
}


// logic for Cellular Automata
void setIterationToZero(vector<int>& vectToChange, int desiredLenOfVect) {
	// Allocate room in the vector for an amount of elements
	// Or Reset the values of the elements of existing elements
	if (vectToChange.size() == 0)
		for (int current = 0; current < desiredLenOfVect; current++)
			vectToChange.push_back(0);
	else
		for (int current = 0; current < desiredLenOfVect; current++)
			vectToChange.at(current) = 0;
}

void setInitialConditions(vector <int>& vectToAlter) {
	int midwayPosition;
	if (vectToAlter.max_size() % 2 != 0) {
		
		// This means that the value is odd, we subtract one from the value so its even,
		// divide in 2 and then add one to the value.
		midwayPosition = (vectToAlter.size() - 1) / 2 + 1;	// set the midway postiion
		vectToAlter.at(midwayPosition) = 1;
	}
	else {
		midwayPosition = (vectToAlter.size() / 2);
		vectToAlter.at(midwayPosition) = 1;
	}
}

int checkNeighbors(int prev, int midd, int next) {
	// If the C.A. pattern requires a check of 3
	if (prev == 0 && midd == 0 && next == 0)	return 0;	// 000 - 1
	if (prev == 0 && midd == 0 && next >= 1)	return 1;	// 001 - 2
	if (prev == 0 && next == 0 && midd >= 1)	return 1;	// 010 - 4
	if (midd >= 1 && next >= 1 && prev == 0)	return 1;	// 011 - 8
	if (midd == 0 && next == 0 && prev >= 1)	return 0;	// 100 - 16
	if (prev >= 1 && next >= 1 && midd == 0)	return 1;	// 101 - 32
	if (prev >= 1 && midd >= 1 && next == 0)	return 1;	// 110 - 64
	if (prev >= 1 && midd >= 1 && next >= 1)	return 0;	// 111 - 128
	return 0;
}

int checkNeighbors(int prev, int midd, int next, string gen) {
	// Use the gen string to change the values of the nextgen by changing output
	//cout << "checkNeighbors: " << gen << endl; 
	//cout << typeid(gen.at(5)).name() << endl;
	if (prev == 0 && midd == 0 && next == 0)	return (gen.at(7) - '0');	// 000 - 1
	if (prev == 0 && midd == 0 && next >= 1)	return (gen.at(6) - '0');	// 001 - 2
	if (prev == 0 && next == 0 && midd >= 1)	return (gen.at(5) - '0');//gen.at(2) - 48;	// 010 - 4
	if (midd >= 1 && next >= 1 && prev == 0)	return (gen.at(4) - '0');//gen.at(3) - 48;	// 011 - 8
	if (midd == 0 && next == 0 && prev >= 1)	return (gen.at(3) - '0');//gen.at(4) - 48;	// 100 - 16
	if (prev >= 1 && next >= 1 && midd == 0)	return (gen.at(2) - '0');//gen.at(5) - 48;	// 101 - 32
	if (prev >= 1 && midd >= 1 && next == 0)	return (gen.at(1) - '0');//gen.at(6) - 48;	// 110 - 64
	if (prev >= 1 && midd >= 1 && next >= 1)	return (gen.at(0) - '0');//gen.at(7) - 48;	// 111 - 128
	return 0;
}

void nextGen(vector <int>& initial, vector <int>& next) {
	// find the next generation
	for (int current = 1; current < initial.size() - 1; current++) {
		next.at(current) = checkNeighbors(initial.at(current - 1), initial.at(current), initial.at(current + 1));
	}
	next.at(0) = next.at(initial.size()) = 0;
}

void nextGen(vector <int>& initial, vector <int>& next, string gen) {
	// find the next generation while also using the gen string
	
	for (int current = 1; current < initial.size() - 1; current++) {
		next.at(current) = checkNeighbors(initial.at(current - 1), initial.at(current), initial.at(current + 1), gen);
	}
	next.at(0) = checkNeighbors(initial.at(initial.size() - 1), initial.at(0), initial.at(1), gen);
	next.at(initial.size() - 1) = checkNeighbors(initial.at(initial.size() - 2), initial.at(initial.size() - 1), initial.at(0), gen);
}


// options for printing to the screen / file
void singleOutput(int generation, int width, int height, int red, int blue, int green) {
	// This will print out a single wolfram cell combination, the number is the value of the 
	// Wolfram cells we want to find

	// find the generation in a bit set, 8 bits wide 
	string genInBinary = bitset<8>(generation).to_string();
	//cout << genInBinary.at(3) << " " << genInBinary.at(1) << " " << genInBinary.at(0) << " " << endl;

	// File name
	string fileName;
	fileName.append("Generation_");
	fileName.append(to_string(generation));
	fileName.append(".ppm");
	ofstream myImage(fileName);

	// Declaring new vectors
	vector <int> currentIteration;
	vector <int> previousIteation;

	// Set iterations equal to zero
	setIterationToZero(currentIteration, width);
	setIterationToZero(previousIteation, width);
	//printLine(currentIteration);

	setInitialConditions(currentIteration);
	//printLine(currentIteration);


	if (myImage.is_open()) {
		// The first part that the program will read and show
		myImage << "P3\n" << width << " " << height << endl;
		myImage << "255\n" << endl;

		// Print ruleset before hand
		printRuleSet(width, height, myImage, red, green, blue, genInBinary);

		printLine(currentIteration, myImage, red, green, blue);
		for (int currentGen = 0; currentGen < height; currentGen++) {
			nextGen(currentIteration, previousIteation, genInBinary);
			printLine(previousIteation, myImage, red, green, blue);
			nextGen(previousIteation, currentIteration, genInBinary);
			printLine(currentIteration, myImage, red, green, blue);
			//printLine(currentIteration);
		}
		myImage.close();
	}
	else {
		cout << "Could not open the file!" << endl;
	}
}

void printRange(int startAtGen, int stopAtGen, int width, int height, int red, int blue, int green) {
	// Prints a range of Wolfram cells form a certian starting point to a specified ending point
	for (int gen = startAtGen; (gen <= stopAtGen && gen < 255); gen++) {
		singleOutput(gen, width, height, red, blue, green);
		printf(" > Gen %d finished\n", gen);
	}
	printf(" >>> Generations finished\n\n");
}

int main() {
	// Where we are running the main program
	int generation = 73;
	int width = 1000, height = 500;
	int r = 102, b = 255, g = 0;

	system("pause");
	printRange(generation, generation, width, height, r, b, g);
	return 0;
}