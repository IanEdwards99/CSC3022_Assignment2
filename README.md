# CSC3022F_Assignment2
# Author: Ian Edwards, EDWIAN004
Description:

Program notes:

pnmtopng -size '800 800 0' input.pnm >output.png

ffmpeg -r 60 -f image2 -s 1920x1080 -i pic%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p test.mp4

specify "pic" name.



