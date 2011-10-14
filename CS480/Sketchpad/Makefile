# Semi-Generic Makefile by Joshua Gleason

##################### ADD FILES TO THESE VARIABLES #############################

# files which are header ONLY
HEADER_FILES = objects/generic/vec.h \
							 objects/generic/mat.h

# names of files that need binaries compiled (assumes each has .h and .cc)
OBJECT_NAMES = objects/shapes/Shape \
							 utilities/InitShader

######################### SETTINGS VARIABLES ###################################

LIBS				 = -lGLU -lglut -lGLEW
FLAGS				 = -g
XTRA_INCLUDE = 

EXE_NAME		 = sketchpad

CC					 = g++

BUILD_DIR    = build/
SRC_DIR		   = src/
BINARY_DIR   = bin/

#################### DO NOT MODIFY BELOW THIS POINT ############################

all: ${EXE_NAME}
${EXE_NAME}: bin/${EXE_NAME}

clean:
	rm -rf build

.PHONY:all doxygen ${EXE_NAME} clean

HEADER_FILES += ${patsubst %,%.h,${OBJECT_NAMES}}
OBJECT_FILES = ${OBJECT_NAMES} ${EXE_NAME}
OBJECTS = ${patsubst %,${BUILD_DIR}%.o,${OBJECT_FILES}}

INCLUDE_DIRS = ${sort ${patsubst %,-I${SRC_DIR}%,${dir ${HEADER_FILES}}} \
							 ${patsubst %,-I${SRC_DIR}%,${dir ${OBJECT_NAMES}}}} \
							 ${XTRA_INCLUDE}

########################### BUILD EXECUTABLE ###################################

${BINARY_DIR}${EXE_NAME}: ${OBJECTS} ${patsubst %,${SRC_DIR}%,${HEADER_FILES}} \
		Makefile
	@mkdir -p ${BINARY_DIR}
	${CC} -o $@ ${OBJECTS} ${FLAGS} ${INCLUDE_DIRS} ${LIBS}

######################## BUILD THE OBJECT FILES ################################

${OBJECTS}:${BUILD_DIR}%.o:${SRC_DIR}%.cc \
		${patsubst %,${SRC_DIR}%,${HEADER_FILES}} Makefile
	@mkdir -p ${dir $@}
	${CC} -c $< -o $@ ${FLAGS} ${INCLUDE_DIRS}
