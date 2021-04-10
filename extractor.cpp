//Author: Ian Edwards EDWIAN004
//Driver file for C++ program which calls the processTag sourcefile functions.
#include <iostream>   //a system header file - needed to do simple I/O
#include <fstream>
#include <sstream>
#include "FrameSequence.h"	         // our header file 
#include <string>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <cmath>

const std::string space = " \n\r\t\f\v";

unsigned char **readData(std::string PGMfilename, int * rows, int * cols){
	//read file data function: (args: filename)
	std::string line;
	std::ifstream ifs;

	ifs.open(PGMfilename, std::ios::binary);
	if (!ifs){std::cerr << "File open failed!" << std::endl;} //check if file exists
   	else
	{
   		getline(ifs >> std::ws, line);
		getline(ifs >> std::ws,line);
		while (line[0] == '#'){ getline(ifs >> std::ws, line);}
		*cols = std::stoi(line.substr(0, line.find_first_of(" ")));
		*rows = std::stoi(line.substr(line.find_first_of(" ")+1, line.length() - line.find_first_of(" ")-1));
		std::cout << *cols << std::endl;
		std::cout << *rows << std::endl;
		

		unsigned char ** matrix = new unsigned char*[*rows];
		for (int i = 0; i < *rows; i++){
			matrix[i] = new unsigned char[*cols];
		}

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
	int x1 = 0; int x2 = 0; int y1 = 0; int y2 = 0; int * rows = new int[1]; int * cols = new int[1];
	int fwidth = 0; int fheight = 0;
	std::vector<std::string*> operations; //each vector position has an array of two values storing the operation name and the file to save it too.

	if (argc < 3) { std::cout << "Incorrect options inputted." << std::endl;}
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
			}
			else if (argv[i] == std::string("-s")){
				fwidth = std::stoi(argv[i+1]); //<---------------------------------------------- swap?
				fheight = std::stoi(argv[i+2]);
				i += 3;
			}
			else if (argv[i] == std::string("-w")){
				std::string *args = new std::string[2];
				*args = argv[i+1];
				*(args+1) = argv[i+2];
				operations.push_back(args);
				i += 3;
			}
		}
	}

	std::cout << PGMfilename << std::endl;
	std::cout << x1 << " " << y1 << std::endl;
	std::cout << x2 << " " << y2 << std::endl;
	std::cout << "frame dimensions: " << fwidth << " x " << fheight << std::endl;

	for (int i = 0; i < operations.size(); i++){
		std::cout << operations[i][0] << std::endl;
		std::cout << operations[i][1] << std::endl;
	}

	unsigned char ** matrix = readData(PGMfilename, rows, cols);

	std::vector<unsigned char **> storedSequences[operations.size()];

	for (int i = 0; i < operations.size(); i++){
		EDWIAN004::FrameSequence Frames = EDWIAN004::FrameSequence(PGMfilename, x1, x2, y1, y2, fwidth, fheight);
		Frames.createFrames(PGMfilename, matrix, *rows, *cols, operations[i][0]);
		storedSequences[i] = Frames.getImageSequence();
		Frames.writeFrames(operations[i][1]);
	}
	
	



	std::cout << "Program exited." << std::endl;
	return 0; // always return a value to calling system

}