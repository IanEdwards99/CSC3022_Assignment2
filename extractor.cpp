//Author: Ian Edwards EDWIAN004
//Driver file for C++ program which loads in data from textfile, creates FrameSequences and outputs frames for video creation.
#include <iostream>   //a system header file - needed to do simple I/O
#include <fstream>
#include <sstream>
#include "FrameSequence.h"	         // our header file 
#include <string>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <cmath>

//readData handles fileIO loading in data, creating an array of pointers pointing to char values and returning this matrix as a pointer.

unsigned char **readData(std::string PGMfilename, int * rows, int * cols){
	//read file data function: (args: filename)
	std::string line;
	std::ifstream ifs;

	ifs.open(PGMfilename, std::ios::binary);
	if (!ifs){std::cerr << "File open failed!" << std::endl;} //check if file exists
   	else
	{
		//initial block reads in header info.
   		getline(ifs >> std::ws, line);
		getline(ifs >> std::ws,line);
		while (line[0] == '#'){ getline(ifs >> std::ws, line);}
		*cols = std::stoi(line.substr(0, line.find_first_of(" ")));
		*rows = std::stoi(line.substr(line.find_first_of(" ")+1, line.length() - line.find_first_of(" ")-1));
		

		unsigned char ** matrix = new unsigned char*[*rows];
		for (int i = 0; i < *rows; i++){
			matrix[i] = new unsigned char[*cols];
		}

		//read in a chunk of values from file into matrix row.
		int x = 0;
		int y = 0;
		getline(ifs, line);
		while (!ifs.eof()){
			for (int y = 0; y < *rows; y++){
				ifs.read((char*)matrix[y], *cols);
			}
		}
		ifs.close();
		return matrix;
	}
}

int main (int argc, char *argv[])
{ 
	std::string PGMfilename;
	int x1 = 0; int x2 = 0; int y1 = 0; int y2 = 0; int * rows = new int[1]; int * cols = new int[1]; //coords and matrix dimensions (passed later to class)
	int fwidth = 0; int fheight = 0;
	std::vector<std::string*> operations; //each vector position has an array of two values storing the operation name and the file to save it too.

	if (argc < 3) { std::cout << "Incorrect options inputted." << std::endl;} //basic error checking.
	else{
		PGMfilename = argv[1];

		int i = 2;

		while (i < argc){
			//std::cout<<argv[i]<<std::endl;
			if (argv[i] == std::string("-t")){
				x1 = std::stoi(argv[i+1]);
				y1 = std::stoi(argv[i+2]);
				x2 = std::stoi(argv[i+3]);
				y2 = std::stoi(argv[i+4]);
				i += 5;
			}//Read in coords
			else if (argv[i] == std::string("-s")){
				fwidth = std::stoi(argv[i+1]); //<---------------------------------------------- swap?
				fheight = std::stoi(argv[i+2]);
				i += 3;
			}//read in frame dimensions
			else if (argv[i] == std::string("-w")){
				std::string *args = new std::string[2];
				*args = argv[i+1];
				*(args+1) = argv[i+2];
				operations.push_back(args);
				i += 3;
			}//read in operations and store in a vector of 2 unit wide arrays.
		}
	}

	unsigned char ** matrix = readData(PGMfilename, rows, cols); //load up matrix.

	std::vector<unsigned char **> storedSequences[operations.size()]; //store imageSequences resulting from different operations.

	for (int i = 0; i < operations.size(); i++){ //Loop through all operations and create FrameSequences for each and output them
		EDWIAN004::FrameSequence Frames = EDWIAN004::FrameSequence(x1, x2, y1, y2, fwidth, fheight);
		Frames.createFrames(matrix, *rows, *cols, operations[i][0]);
		storedSequences[i] = Frames.getImageSequence();
		Frames.writeFrames(operations[i][1]);
	}

	if (operations.empty()){ //If no operations are inputted, have a default.
		EDWIAN004::FrameSequence Frames = EDWIAN004::FrameSequence(x1, x2, y1, y2, fwidth, fheight);
		Frames.createFrames(matrix, *rows, *cols, "none");
		storedSequences[0] = Frames.getImageSequence();
		Frames.writeFrames("default");
	}
	
	if (matrix != nullptr){ //matrix deconstructor.
		for (int k = 0; k < *rows; k++){
			if (matrix[k] != nullptr){
				delete matrix[k];
			}
		}
		delete matrix;
	}

	std::cout << "Program exited." << std::endl;
	return 0; // always return a value to calling system

}