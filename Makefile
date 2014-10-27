CXX=g++ -g
XXFLAGS=-Wall -MMD
EXEC=cc3k
OBJECTS=cc3k.o floor.o textdisplay.o character.o cell.o enemy.o item.o affect.o player.o
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${XXFLAGS} ${OBJECTS} -o ${EXEC} -lncurses
-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC}
