//Author: Ian Edwards - EDWIAN004
// processTag.cpp - implementation for tag processing. The sourcefile.
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <FrameSequence.h>

//Class method definitions

EDWIAN004::FrameSequence::FrameSequence() : imageSequence(NULL), PGMfilename(nullptr), x1(0), x2(0), y1(0), y2(0), fwidth(0), fheight(0){}
EDWIAN004::FrameSequence::FrameSequence(std::string PGMfilename, int x1, int x2, int y1, int y2, int fwidth, int fheight) : imageSequence(NULL), PGMfilename(PGMfilename), x1(x1), x2(x2), y1(y1), y2(y2), fwidth(fwidth), fheight(fheight){}

std::vector<unsigned char **> EDWIAN004::FrameSequence::createFrames(std::string filename, unsigned char** matrix){
	float g = 0.0;
		coord frame_coord = {x1, y1};

	if (x1 == x2){ //Deal with case where frame must just go down or up. (cant divide by zero for gradient.)
		if (y1 >= y2){ //case of starting frame below destination frame.
			for (int y = y1; y >= y2; --y){
				frame_coord.x = x1; frame_coord.y = y;
				unsigned char** frame_matrix = createMatrix(frame_coord, matrix);
				imageSequence.push_back(frame_matrix);
			}
		}
		else if (y1 < y2){
			for (int y = y1; y <= y2; y++){
				unsigned char** frame_matrix = createMatrix(frame_coord, matrix);
				imageSequence.push_back(frame_matrix);
			}
		}
	}
	else{
		g = fabs((y2-y1)/(x2-x1)); //initial gradient
		if (fabs(g) <= 1.0){
			float y = y1;
			if (x1 <= x2) {
				
				for (int x = x1; x <= x2; x++){
					//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
					frame_coord.x = x; frame_coord.y = std::round(y); //WRITE TO VECTOR.
					y += g; 
					unsigned char ** frame_matrix = new unsigned char*[fheight];
					for (int i = 0; i < fheight; i++){
						frame_matrix[i] = new unsigned char[fwidth];
						for (int j = 0; j < fwidth; j++){
							frame_matrix[i][j] = matrix[i+frame_coord.y][j+frame_coord.x];
						}
					}
					imageSequence.push_back(frame_matrix);
					g = (y2-y)/(x2-x);
				}
			}
			else if (x1 > x2){
				for (int x = x1; x >= x2; --x){
					frame_coord.x = x; frame_coord.y = std::round(y); 
					y += fabs(g); 
					unsigned char** frame_matrix = createMatrix(frame_coord, matrix);
					imageSequence.push_back(frame_matrix);
					g = (y2-y)/(x2-x);
				}
			}
		}
		else if (fabs(g) > 1.0){
			float x = x1;
			g = fabs((x2-x1)/(y2-y1));

			if (y1 <= y2) {
				for (int y = y1; y <= y2; y++){
					//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
					frame_coord.y = y; frame_coord.x = std::round(x);
					x += g; 
					unsigned char** frame_matrix = createMatrix(frame_coord, matrix);
					imageSequence.push_back(frame_matrix);
					g = fabs((x2-x)/(y2-y));
				}
			}
			else if (y1 > y2){
				for (int y = y1; y >= y2; --y){
					//find new frame_coord? Given x1 and y1 and x2 and y2. Step of 1 in x.
					frame_coord.y = y; frame_coord.x = std::round(x);
					x += g; 
					unsigned char** frame_matrix = createMatrix(frame_coord, matrix);
					imageSequence.push_back(frame_matrix);
					g = fabs((x2-x)/(y2-y));
				}
			}

		}
	}
}

void EDWIAN004::FrameSequence::writeFrames(){
	int counter = 0;

	for (int k = 0; k < imageSequence.size(); k++){
		char buffer[32]; sprintf(buffer, "%04d", counter);
		counter += 1;
		std::string seqNr(buffer);
		std::ofstream wf("./output/sequence" + seqNr + ".pgm", std::ios::out | std::ios::binary);
		if(!wf) {
			std::cout << "Cannot open file!" << std::endl;
			return 1;
		}
		wf << "P5" << std::endl << fheight << " " << fwidth << std::endl << 255 << std::endl;
		for (int i = 0; i < fheight; i ++){
			wf.write((char*)(imageSequence[k][i]), fwidth); //reinterpret_cast<char*>(imageseq..)
		}
		wf.close();
	}
};

unsigned char** EDWIAN004::FrameSequence::createMatrix(coord frame_coord, unsigned char** matrix){
	unsigned char ** frame_matrix = new unsigned char*[fheight];
	for (int i = 0; i < fheight; i++){
		frame_matrix[i] = new unsigned char[fwidth];
		for (int j = 0; j < fwidth; j++){
			frame_matrix[i][j] = matrix[i+frame_coord.y][j+frame_coord.x];
		}
	}
	return frame_matrix;
};

void setFrameSize(int fwidth, int fheight){

}
