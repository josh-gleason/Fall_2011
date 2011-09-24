EXTRA_FLAGS=-g -Wall
OPEN_GL_LIBS=-lglut -lGLEW

COMMON_DIR=../common

FLAGS=${EXTRA_FLAGS} ${OPEN_GL_LIBS} -I${COMMON_DIR}

main: main.cc ${COMMON_DIR}/InitShader.o Makefile
	g++ -o main main.cc ${COMMON_DIR}/InitShader.o ${FLAGS}

${COMMON_DIR}/InitShader.o: Makefile ${COMMON_DIR}/InitShader.cc
	g++ -c ${COMMON_DIR}/InitShader.cc -o ${COMMON_DIR}/InitShader.o ${EXTRA_FLAGS}
