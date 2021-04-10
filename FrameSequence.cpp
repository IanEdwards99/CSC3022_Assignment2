//Author: Ian Edwards - EDWIAN004
// processTag.cpp - implementation for tag processing. The sourcefile.
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include "FrameSequence.h"
#include <exception>

//Class method definitions
//Constructors:
EDWIAN004::FrameSequence::FrameSequence() : imageSequence(NULL), xstart(0), xend(0), ystart(0), yend(0), fwidth(0), fheight(0){}
EDWIAN004::FrameSequence::FrameSequence(int xstart, int xend, int ystart, int yend, int fwidth, int fheight) : imageSequence(NULL), xstart(xstart), xend(xend), ystart(ystart), yend(yend), fwidth(fwidth), fheight(fheight){}

//This method checks for different cases such as if x1 < x2 or x1 > x2, and loops accordingly over the respective variable so that the gradient is maintained as less/equal to 1,
//so that when one increases a variable by the gradient, it moves only by 1 pixel. Other cases are accounted for such as when one is moving up rather than down across a gradient, then a variable is increased or decreased by g.
//Lastly, different operations are carried out depending on the given operation.
std::vector<unsigned char **> EDWIAN004::FrameSequence::createFrames(unsigned char** matrix, int rows, int cols, std::string operation){
	//operations can be none, invert, reverse, revinvert.
	int x1 = 0; int x2 = 0; int y1 = 0; int y2 = 0;
	if (operation == "reverse" or operation == "revinvert"){ //Swap origin and destination coordinates to reverse order.
		x1 = xend;
		x2 = xstart;
		y1 = yend;
		y2 = ystart;
	}
	else{
		x1 = xstart;
		x2 = xend;
		y1 = ystart;
		y2 = yend;
	}
	if (operation == "revinvert") operation = "invert"; //order is already reversed previously, continue like normal just invert pixels.

	float g = 0.0; //declare gradient variable.
	coord frame_coord = {x1, y1}; //starting coordinates for frame

	if (x1 == x2){ //Deal with case where frame must just go down or up. (cant divide by zero for gradient.)
		if (y1 >= y2){ //case of starting frame below destination frame.
			for (int y = y1; y >= y2; --y){
				frame_coord.x = x1; frame_coord.y = y;
				unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
				imageSequence.push_back(frame_matrix); //add frame to the vector.
			}
		}
		else if (y1 < y2){ //case of starting above destination frame.
			for (int y = y1; y <= y2; y++){
				frame_coord.x = x1; frame_coord.y = y;
				unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
				imageSequence.push_back(frame_matrix);
			}
		}
		return imageSequence;
	}
	else{
		g = fabs((y2-y1)/(x2-x1)); //initial gradient
		if (fabs(g) <= 1.0){ //not too steep, so will move one pixel at a time.
			float y = y1;
			if (x1 <= x2) { //destination point to the right of the starting point.
				if (y1 < y2){ //start y value is above end y value, so y += g not y -= g;
					for (int x = x1; x <= x2; x++){ //Loop across x values.
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.x = x; frame_coord.y = std::round(y);
						y += g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x); //recalculate gradient.
					}
				}
				else {
					for (int x = x1; x <= x2; x++){
						//y values need to decrease to move up to destination y.
						frame_coord.x = x; frame_coord.y = std::round(y);
						y -= g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				return imageSequence;
			}
			else if (x1 > x2){ //starting to the right of the end point.
				if (y1 < y2){ //Above the end point?
					for (int x = x1; x >= x2; --x){ //move left.
						frame_coord.x = x; frame_coord.y = std::round(y); 
						y += fabs(g); //thus increase y. move down.
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				else {
					for (int x = x1; x >= x2; --x){
						frame_coord.x = x; frame_coord.y = std::round(y); 
						y -= fabs(g); //below end point, move up.
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				return imageSequence;
			}
		}
		else if (fabs(g) > 1.0){ //Too steep to move one pixel at a time, loop in y direction instead.
			float x = x1;
			g = fabs((x2-x1)/(y2-y1)); //recalculate g inverted.

			if (y1 <= y2) { //start point above end point?
				if (x1 < x2){ //start x left of end x?
					for (int y = y1; y <= y2; y++){ //Loop through y values.
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.y = y; frame_coord.x = std::round(x);
						x += g; //increase x to go right.
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = fabs((x2-x)/(y2-y));
					}
				}
				else {
					for (int y = y1; y <= y2; y++){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.y = y; frame_coord.x = std::round(x);
						x -= g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = fabs((x2-x)/(y2-y));
					}
				}

				return imageSequence;
			}
			else if (y1 > y2){ //case of being below end point.
				if (x1 < x2) { //left of end point
					for (int y = y1; y >= y2; --y){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.y = y; frame_coord.x = std::round(x);
						x += g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = fabs((x2-x)/(y2-y));
					}
				}
				else { //right of end point.
					for (int y = y1; y >= y2; --y){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.y = y; frame_coord.x = std::round(x);
						x -= g;  //decrease to go leftwards.
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = fabs((x2-x)/(y2-y));
					}

				}
				return imageSequence;
			}

		}
	}
	return imageSequence;
}
//write out frames in imageSequence vector to specified file.
void EDWIAN004::FrameSequence::writeFrames(std::string filename){
	int counter = 0;

	for (int k = 0; k < imageSequence.size(); k++){
		char buffer[32]; sprintf(buffer, "%04d", counter);
		counter += 1;
		std::string seqNr(buffer);
		std::ofstream wf("./output/" + filename + seqNr + ".pgm", std::ios::out | std::ios::binary);
		if(!wf) {
			std::cout << "Cannot open file!" << std::endl;
		}
		wf << "P5" << std::endl << fheight << " " << fwidth << std::endl << 255 << std::endl;
		for (int i = 0; i < fheight; i ++){
			wf.write((char*)(imageSequence[k][i]), fwidth); //write out each row in frame_matrix in question from imageSequence.
		}
		wf.close();
	}
};
//method to prepare frame_matrix array of pointers and load the respective value from the image/matrix lying underneath, into the frame_matrix array.
unsigned char** EDWIAN004::FrameSequence::createMatrix(coord frame_coord, unsigned char** matrix, int rows, int cols, std::string operation){
	unsigned char ** frame_matrix = new unsigned char*[fheight];

	for (int i = 0; i < fheight; i++){
		frame_matrix[i] = new unsigned char[fwidth];
		for (int j = 0; j < fwidth; j++){
			if (i+frame_coord.y >= rows or j+frame_coord.x >= cols) frame_matrix[i][j] = 0;
			else if ((operation == "none" or operation == "" or operation == "reverse") and operation != "invert" and operation != "revinvert") //go normal if incorrect operation code is given.
				frame_matrix[i][j] = matrix[i+frame_coord.y][j+frame_coord.x];
			else if (operation == "invert")
				frame_matrix[i][j] = (255 - (float)matrix[i+frame_coord.y][j+frame_coord.x]);
		}
	}
	return frame_matrix;
};

//Deconstructor loops through nested pointer arrays to delete each pointer.
EDWIAN004::FrameSequence::~FrameSequence()
{
	for (int k = 0; k < imageSequence.size(); k++){
		if (this->imageSequence[k] != nullptr){
			for (int i=0; i < fheight; i++){
				if (this->imageSequence[k][i] != nullptr){
					delete this->imageSequence[k][i];
				};
			};
			delete this->imageSequence[k];
		};
	};
}
