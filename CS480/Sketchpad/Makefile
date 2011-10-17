# Semi-Generic Makefile by Joshua Gleason
##################### ADD FILES TO THESE VARIABLES #############################
default: all

# files which are header ONLY
HEADER_FILES = objects/generic/vec.h \
							 objects/generic/mat.h

# names of files that need binaries compiled (assumes each has .h and .cc)
OBJECT_NAMES = objects/shapes/Shape \
               objects/shapes/Rect \
							 utilities/InitShader

######################### SETTINGS VARIABLES ###################################

EXE_NAME		 = sketchpad

THIRD_PARTY_DIR = ./3rdParty

LIBS				 = -lGLU -lglut -lGLEW ${THIRD_PARTY_DIR}/triangle/lib/libdel.a
FLAGS				 = -g
XTRA_INCLUDE = -I${THIRD_PARTY_DIR}/triangle/include/

CC					 = g++

BUILD_DIR    = build/
SRC_DIR		   = src/
BINARY_DIR   = bin/

######################### THIRD PARTY LIBRARIES ################################

ALL_EXTRA    		= triangle
PHONY_EXTRA  		= triangle

## add any third party build commands here
triangle:
	cd ${THIRD_PARTY_DIR}/triangle/ && make

#################### DO NOT MODIFY BELOW THIS POINT ############################
all: ${ALL_EXTRA} ${EXE_NAME}

${EXE_NAME}: bin/${EXE_NAME}

clean:
	rm -rf build

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

.PHONY:all ${EXE_NAME} clean doxygen ${PHONY_EXTRA}

