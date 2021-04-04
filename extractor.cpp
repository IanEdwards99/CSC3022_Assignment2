//Author: Ian Edwards EDWIAN004
//Driver file for C++ program which calls the processTag sourcefile functions.
#include <iostream>   //a system header file - needed to do simple I/O
#include "processTag.h"	         // our header file - contains decl of fib()
#include <string>
#include <cstdlib>

int main (void)
{
	using namespace std;  
	vector<tagStruct> vect;

	cout << "c: Clear display.\nr: Read and process tag file\np: Print all tags\nd: Write tags\nl: List tags\nq: Quit\n\nEnter an option (r,p,d,l) or q to quit, and press return..." << std::endl;
	string input;
	cin >> input; //can also use input = cin.get(); reads next character.

	while (input != "q") { //quit if q is entered.
		if (input == "c"){system("clear");} //clear if c is entered
		else if (input == "r"){	//read in a file's tags and details into the vector declared previously.
			std::cout << "Enter a filename..." << std::endl;
			string file;
			std::cin >> file;
			vect = EDWIAN004::stackRead(file);
		}
		else if (input == "p"){ //print all the tags in the vector read in by "r"
			std::cout << "Printing tags..." << std::endl;
			EDWIAN004::displayVect(vect);
		}
		else if (input == "d"){ //output vector contents to a textfile.
			std::cout << "Enter filename to write to:" << std::endl;
			std::cin >> input;
			EDWIAN004::writeFile(vect, input);
		}
		else if (input == "l") { //Find a tag name in the vector
			std::cout << "Enter a tag name to output:" << std::endl;
			std::string tagName;
			std::cin >> tagName;
			EDWIAN004::outputTag(vect, tagName);
		}
		else std::cout << "Incorrect option selected. Please choose an option from below:\n" << endl;

		cout << "c: Clear display.\nr: Read and process tag file\np: Print all tags\nd: Write tags\nl: List tags\nq: Quit\n\nEnter an option (r,p,d,l) or q to quit, and press return..." << std::endl;
		cin >>input;
	}
	std::cout << "Program exited." << std::endl;
	return 0; // always return a value to calling system

}
