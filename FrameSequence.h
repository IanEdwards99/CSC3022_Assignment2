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
			int xstart ; int xend; int ystart;; int yend;; //start and end coordinates.
			int fwidth; int fheight; //frame width and frame height.
				
			
		public:
			FrameSequence();
			FrameSequence(std::string filename, int xstart, int xend, int ystart, int yend,int fwidth, int fheight);
			~FrameSequence(void);
			FrameSequence(const FrameSequence& p);
			FrameSequence& operator=(const FrameSequence& rhs);
			
			std::vector<unsigned char **> createFrames(std::string filename, unsigned char** matrix, int rows, int cols, std::string operation);
			void writeFrames(std::string filename);

			void setFrameSize(int fwidth, int fheight);
			int getX1(){ return xstart ;};
			int getX2(){return xend;};
			int getY1(){return ystart;};
			int getY2(){return yend;};
			int getFrameWidth(){return fwidth;};
			int getFrameHeight(){return fheight;};
			std::vector<unsigned char **> getImageSequence(){return imageSequence;};
			unsigned char** createMatrix(coord frame_coord, unsigned char** matrix, int rows, int cols, std::string operation);
	};
	#endif

}
