# Ian Edwards - my Makefile
# EDWIAN004
CC=g++         # the compiler
LIBS=-lm       # the libraries we will ref

# object files to create .exe
tags: extractor.o
	$(CC) extractor.o -o extractor.exe $(LIBS)

# Implicit pattern rule for compiling object files
%.o : %.cpp   # $< is the first dependancy and $@ is the target
	$(CC) -c $< -o $@
   
# other rules; invoked by make clean etc 

# deletes all the object code files
clean:
	@rm -f *.o
	@rm extractor.exe
   
# copies/moves the relevant binaries and/or libraries to the correct folders   
install:
	@mv extractor.exe ~/bin

