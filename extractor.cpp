//Author: Ian Edwards EDWIAN004
//Driver file for C++ program which calls the processTag sourcefile functions.
#include <iostream>   //a system header file - needed to do simple I/O
#include <fstream>
#include <sstream>
//#include "FrameSequence.h"	         // our header file - contains decl of fib()
#include <string>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <cmath>

struct coord {
	int x;
	int y;
};

int main (int argc, char *argv[])
{ 
	std::vector<unsigned char **> imageSequence; // imageSequence[i][row][col] to get i'th frame's pixel.
	std::string PGMfilename;
	int x1 = 0; int x2 = 0; int y1 = 0; int y2 = 0;
	int fwidth = 0; int fheight = 0; int Twidth = 0; int Theight = 0;
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
				fheight = std::stoi(argv[i+1]); //<---------------------------------------------- swap?
				fwidth = std::stoi(argv[i+2]);
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

	//read file data function: (args: filename)
	std::string line;
	std::ifstream ifs;

	ifs.open(PGMfilename, std::ios::binary | std::ios::in);
	if (!ifs){std::cerr << "File open failed!" << std::endl;} //check if file exists
   	else
	{
   		getline(ifs, line);
		getline(ifs,line);
		while (line[0] == '#'){ getline(ifs, line);}
		int rows = std::stoi(line.substr(0, line.find_first_of(" ")));
		int cols = std::stoi(line.substr(line.find_first_of(" ")+1));
		std::cout << rows << std::endl;
		std::cout << cols << std::endl;

		unsigned char ** matrix = new unsigned char*[rows];
		for (int i = 0; i < rows; i++){
			matrix[i] = new unsigned char[cols];
		}
		getline(ifs, line);

		while (!ifs.eof()){
			int y = 0;
			for (int y = 0; y < rows; y++){
				ifs.read((char*)matrix[y], cols); //read ws?
			}
		}
		ifs.close();

		//write data out to vector, frame by frame.

		float g = 0.0;
		
		//float x = x1;
		coord frame_coord = {x1, y1};

		if (x1 == x2){ //Deal with case where frame must just go down or up. (cant divide by zero for gradient.)
			if (y1 >= y2){ //case of starting frame below destination frame.
				for (int y = y1; y >= y2; --y){
					frame_coord.x = x1; frame_coord.y = y;
					//std::cout << frame_coord.x << " " << frame_coord.y << std::endl;
				}
			}
			else if (y1 < y2){
				for (int y = y1; y <= y2; y++){
					frame_coord.x = x1; frame_coord.y = y;
					//std::cout << frame_coord.x << " " << frame_coord.y << std::endl;
				}
			}
		}
		else{
			g = fabs((y2-y1)/(x2-x1)); //initial gradient
			// std::cout << y2 << std::endl;
			// std::cout << y1 << std::endl;
			// std::cout << x2 << std::endl;
			// std::cout << x1 << std::endl;
			if (fabs(g) <= 1.0){
				float y = y1;
				if (x1 <= x2) {
					// std::cout << g << std::endl;
					// std::cout << "testing..." << std::endl;
					
					for (int x = x1; x <= x2; x++){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						y += g; frame_coord.x = x; frame_coord.y = std::round(y); //WRITE TO VECTOR.

						unsigned char ** frame_matrix = new unsigned char*[fheight];
						for (int i = 0; i < fheight; i++){
							frame_matrix[i] = new unsigned char[fwidth];
							for (int j = 0; j < fwidth; j++){
								frame_matrix[i][j] = matrix[frame_coord.y][frame_coord.x];
							}
						}
						imageSequence.push_back(frame_matrix);
						

						g = (y2-y)/(x2-x);
						//std::cout << frame_coord.x << " " << frame_coord.y << std::endl;
					}
					// std::cout << (float)imageSequence[0][0][0] << std::endl;
					// std::cout << (float)matrix[0][11] << std::endl;

					int counter = 0;
					char buffer[256]; sprintf(buffer, "%04d", counter);
					std::string seqNr(buffer);
					std::ofstream wf("./output/sequence" + seqNr, std::ios::out | std::ios::binary);
					if(!wf) {
						std::cout << "Cannot open file!" << std::endl;
						return 1;
					}
					wf << "P5" << std::endl << fheight << " " << fwidth << std::endl << 255 << std::endl;
					// for (int k = 0; k < imageSequence.size(); k ++) {
						for (int i = 0; i < fheight; i ++){
							wf.write((char*)(imageSequence[0][i]), sizeof(imageSequence[0][i])); //reinterpret_cast<char*>(imageseq..)
						}
					// }
					wf.close();


				}
				else if (x1 > x2){
					for (int x = x1; x >= x2; --x){
						y += fabs(g); frame_coord.x = x; frame_coord.y = std::round(y);
						g = (y2-y)/(x2-x);
						//std::cout << frame_coord.x << " " << frame_coord.y << std::endl;
					}
				}
			}
			else if (fabs(g) > 1.0){
				float x = x1;
				g = fabs((x2-x1)/(y2-y1));

				if (y1 <= y2) {
					for (int y = y1; y <= y2; y++){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						x += g; frame_coord.y = y; frame_coord.x = std::round(x);
						g = fabs((x2-x)/(y2-y));
						//std::cout << frame_coord.x << " " << frame_coord.y << std::endl;
					}
				}
				else if (y1 > y2){
					for (int y = y1; y >= y2; --y){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						x += g; frame_coord.y = y; frame_coord.x = std::round(x);
						g = fabs((x2-x)/(y2-y));
						//std::cout << frame_coord.x << " " << frame_coord.y << std::endl;
					}
				}

			}
		}




		// for (int i = 0; i < rows; i++){
		// 	for (int j = 0; j < cols; j++){
		// 		std::cout << (float)matrix[i][j] << " ";
		// 	}
		// 	std::cout << std::endl;
		// }
   	}

	std::cout << "Program exited." << std::endl;
	return 0; // always return a value to calling system

}
