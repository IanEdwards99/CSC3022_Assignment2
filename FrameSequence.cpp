//Author: Ian Edwards - EDWIAN004
// processTag.cpp - implementation for tag processing. The sourcefile.
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

//Class method definitions

void readData(std::string filename){
	ifstream ifs;
	ifs.open(filename, ios::binary | ios::in)
	if (!ifs){cerr << "File open failed!" << endl;} //check if file exists
   	else{
   		
   	}
}

void writeData(std::string filename){

}

//space variable and ltrim, rtrim and trim are extra methods to trim off any unwanted characters from a string.
const std::string space = " \n\r\t\f\v";

std::vector<unsigned char **> imageSequence; // imageSequence[i][row][col] to get i'th frame's pixel.
