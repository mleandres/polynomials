CXX = g++-5
CXXFLAGS = -std=c++14
EXEC = polynomials
OBJECTS = main.o Polynomial.o
DEPENDS = ${OBJECTS:.o =.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
