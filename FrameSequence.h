//Author: Ian Edwards EDWIAN004
//Basic headerfile for C++ program for processTag.cpp
#include <string>
#include <vector>
#include <cctype>
#include <cmath>

#ifndef FrameSequence_h
#define FrameSequence_h

struct coord {
	int x;
	int y;
};

namespace EDWIAN004 {

	class FrameSequence
	{
		private:
			std::vector<unsigned char **> imageSequence; // imageSequence[i][row][col] to get i'th frame's pixel.
			std::string PGMfilename;
			int x1 ; int x2; int y1;; int y2;; //start and end coordinates.
			int fwidth; int fheight; //frame width and frame height.
				
			
		public:
			FrameSequence(void) {}
			FrameSequence(std::string filename, int x1, int x2, int y1, int y2, int fwidth, int fheight){};
			~FrameSequence(void){};
			FrameSequence(const FrameSequence& p);
			FrameSequence& operator=(const FrameSequence& rhs);
			
			std::vector<unsigned char **> createFrames(std::string filename, unsigned char** matrix){};
			void writeFrames(){};

			void setFrameSize(int fwidth, int fheight){}
			int getX1(){ return x1 ;};
			int getX2(){return x2;};
			int getY1(){return y1;};
			int getY2(){return y2;};
			int getFrameWidth(){return fwidth;};
			int getFrameHeight(){return fheight;};
			std::vector<unsigned char **> getImageSequence(){return imageSequence;};
			unsigned char** createMatrix(coord frame_coord, unsigned char** matrix){};
	};
	#endif

}
