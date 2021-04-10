//Author: Ian Edwards EDWIAN004
//headerfile for FrameSequence class
#include <string>
#include <vector>
#include <cctype>
#include <cmath>

#ifndef FrameSequence_h
#define FrameSequence_h

struct coord { //struct to store coords.
	int x;
	int y;
};

namespace EDWIAN004 {

	class FrameSequence
	{
		private: //private fields of FrameSequence class storing an imageSequence of frames, the coords and the frame dimensions.
			std::vector<unsigned char **> imageSequence; // imageSequence[i][row][col] to get i'th frame's pixel.
			int xstart ; int xend; int ystart;; int yend;; //start and end coordinates.
			int fwidth; int fheight; //frame width and frame height.
				
			
		public: //All the methods of the class
			FrameSequence(); //Default constructor
			FrameSequence(int xstart, int xend, int ystart, int yend,int fwidth, int fheight); //Custom constructor
			~FrameSequence(void); //Deconstructor
			// FrameSequence(const FrameSequence& p); //not needed for this assignment.
			// FrameSequence& operator=(const FrameSequence& rhs);
			
			std::vector<unsigned char **> createFrames(unsigned char** matrix, int rows, int cols, std::string operation); //method to createFrames in imageSequence variable. Does not need to be loaded into a variable but can be.
			void writeFrames(std::string filename); //write frames out from imageSequence into a given named sequence.

			//Setter/Mutator methods
			void setFrameSize(int fwidth, int fheight){this->fwidth = fwidth; this->fheight = fheight;}; 
			void setCoords(int x1, int x2, int y1, int y2) {xstart= x1; xend = x2; ystart = y1; yend = y2;};
			void setImageSeq(std::vector<unsigned char **> someSeq){imageSequence = someSeq;};

			//Accessor/get methods
			int getX1(){ return xstart ;};
			int getX2(){return xend;};
			int getY1(){return ystart;};
			int getY2(){return yend;};
			int getFrameWidth(){return fwidth;};
			int getFrameHeight(){return fheight;};
			std::vector<unsigned char **> getImageSequence(){return imageSequence;};

			//Auxiliary method to create the frame_matrix per frame.
			unsigned char** createMatrix(coord frame_coord, unsigned char** matrix, int rows, int cols, std::string operation);
	};
	#endif

}
