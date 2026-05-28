###
### Makefile for Gerp Project
###
### Author:  Hansini Gundavarapu and Jessica Charalel

## 
## Build up your Makefile in a similar manner as for Zap. Feel free 
## to use that Makefile to guide you! Note, you do NOT need rules for
## FSTree.o and DirNode.o as those (pre-compiled) object files are 
## provided to you.
## 
## At the end, you can delete this comment!
## 

##
## Here is a special rule that removes all .o files besides the provided ones 
## (DirNode.o and FSTree.o), all temporary files (ending with ~), and 
## a.out produced by running unit_test. First, we find all .o files 
## that are not provided files, then we run rm -f on each found file 
## using the -exec find flag. Second, we delete the temporary files
## and a.out. @ is used to suppress stdout.
## 
## You do not need to modify or further comment this rule!
##
###
### Makefile for Gerp Project
###
### Author:  Jessica Charalel
## 
#########################################################
#             PROJECT 4: Gerp Makefile            #
#########################################################

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

gerp: main.o gerp.o DirNode.o FSTree.o processing.o HashTable.o
	$(CXX) $(LDFLAGS) -o gerp main.o gerp.o DirNode.o FSTree.o processing.o HashTable.o

main.o: main.cpp gerp.h HashTable.h processing.h
	$(CXX) $(CXXFLAGS) -c $<
	
processing.o: processing.cpp processing.h
	$(CXX) $(CXXFLAGS) -c $< 

HashTable.o: HashTable.cpp HashTable.h
	$(CXX) $(CXXFLAGS) -c $<

gerp.o: gerp.cpp gerp.h
	$(CXX) $(CXXFLAGS) -c $<

unit_test: unit_test_driver.o processing.o HashTable.o DirNode.o FSTree.o
	$(CXX) $(CXXFLAGS) -o a.out unit_test_driver.o processing.o HashTable.o DirNode.o FSTree.o


clean:
	@find . -type f \( \
		-name '*.o' ! -name 'FSTree.o' ! -name 'DirNode.o' \
		\) -exec rm -f {} \;
	@rm -f *~ a.out

