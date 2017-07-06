CXX = g++
CXXFLAGS = -Wall -MMD
EXEC = ss5k
OBJECTS = main.o textdisplay.o level0.o level1.o level2.o board.o cell.o window.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
