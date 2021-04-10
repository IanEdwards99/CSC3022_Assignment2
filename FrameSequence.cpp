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

EDWIAN004::FrameSequence::FrameSequence() : imageSequence(NULL), PGMfilename(nullptr), xstart(0), xend(0), ystart(0), yend(0), fwidth(0), fheight(0){}
EDWIAN004::FrameSequence::FrameSequence(std::string PGMfilename, int xstart, int xend, int ystart, int yend, int fwidth, int fheight) : imageSequence(NULL), PGMfilename(PGMfilename), xstart(xstart), xend(xend), ystart(ystart), yend(yend), fwidth(fwidth), fheight(fheight){}

std::vector<unsigned char **> EDWIAN004::FrameSequence::createFrames(std::string filename, unsigned char** matrix, int rows, int cols, std::string operation){
	//operations can be none, invert, reverse, revinvert.
	int x1 = 0; int x2 = 0; int y1 = 0; int y2 = 0;
	if (operation == "reverse" or operation == "revinvert"){ //Swamp origin and destination coordinates to reverse order.
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

	std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << " " <<  std::endl;
	std::cout << xstart << " " << ystart << " " << xend << " " << yend << " " <<  std::endl;
	float g = 0.0;
	coord frame_coord = {x1, y1};

	if (x1 == x2){ //Deal with case where frame must just go down or up. (cant divide by zero for gradient.)
		if (y1 >= y2){ //case of starting frame below destination frame.
			for (int y = y1; y >= y2; --y){
				frame_coord.x = x1; frame_coord.y = y;
				unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
				imageSequence.push_back(frame_matrix);
			}
		}
		else if (y1 < y2){
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
		if (fabs(g) <= 1.0){
			float y = y1;
			if (x1 <= x2) {
				if (y1 < y2){
					for (int x = x1; x <= x2; x++){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.x = x; frame_coord.y = std::round(y); //WRITE TO VECTOR.
						y += g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				else {
					for (int x = x1; x <= x2; x++){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.x = x; frame_coord.y = std::round(y); //WRITE TO VECTOR.
						y -= g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				return imageSequence;
			}
			else if (x1 > x2){
				if (y1 < y2){
					for (int x = x1; x >= x2; --x){
						frame_coord.x = x; frame_coord.y = std::round(y); 
						y += fabs(g); 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				else {
					for (int x = x1; x >= x2; --x){
						frame_coord.x = x; frame_coord.y = std::round(y); 
						y -= fabs(g); 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = (y2-y)/(x2-x);
					}
				}
				return imageSequence;
			}
		}
		else if (fabs(g) > 1.0){
			float x = x1;
			g = fabs((x2-x1)/(y2-y1));

			if (y1 <= y2) {
				if (x1 < x2){
					for (int y = y1; y <= y2; y++){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.y = y; frame_coord.x = std::round(x);
						x += g; 
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
			else if (y1 > y2){
				if (x1 < x2) {
					for (int y = y1; y >= y2; --y){
						//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
						frame_coord.y = y; frame_coord.x = std::round(x);
						x += g; 
						unsigned char** frame_matrix = createMatrix(frame_coord, matrix, rows, cols, operation);
						imageSequence.push_back(frame_matrix);
						g = fabs((x2-x)/(y2-y));
					}
				}
				else {
					for (int y = y1; y >= y2; --y){
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

		}
	}
	return imageSequence;
}

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
			wf.write((char*)(imageSequence[k][i]), fwidth); //reinterpret_cast<char*>(imageseq..)
		}
		wf.close();
	}
};

unsigned char** EDWIAN004::FrameSequence::createMatrix(coord frame_coord, unsigned char** matrix, int rows, int cols, std::string operation){
	unsigned char ** frame_matrix = new unsigned char*[fheight];

	for (int i = 0; i < fheight; i++){
		frame_matrix[i] = new unsigned char[fwidth];
		for (int j = 0; j < fwidth; j++){
			if (i+frame_coord.y >= rows or j+frame_coord.x >= cols) frame_matrix[i][j] = 0;
			else if (operation == "none" or operation == "")
				frame_matrix[i][j] = matrix[i+frame_coord.y][j+frame_coord.x];
			else if (operation == "invert")
				frame_matrix[i][j] = (255 - (float)matrix[i+frame_coord.y][j+frame_coord.x]);
		}
	}
	return frame_matrix;
};

void setFrameSize(int fwidth, int fheight){

}

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
