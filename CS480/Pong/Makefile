# Semi-Generic Makefile by Joshua Gleason

##################### ADD FILES TO THESE VARIABLES #############################

# files which are header ONLY
HEADER_FILES = objects/generic/vec.h \
							 objects/generic/mat.h \
							 objects/shapes/Poly.h

# names of files that need binaries compiled (assumes each has .h and .cc)
OBJECT_NAMES = objects/shapes/Rect \
							 objects/shapes/Circle \
							 objects/components/Paddle \
							 objects/components/Ball \
							 utilities/InitShader

######################### SETTINGS VARIABLES ###################################

LIBS				 = -lglut -lGLEW
FLAGS				 = -g
XTRA_INCLUDE = 

EXE_NAME		 = pong
DOXY_CFG		 = doxygen.cfg

CC					 = g++

BUILD_DIR    = build/
SRC_DIR		   = src/
BINARY_DIR   = bin/

#################### DO NOT MODIFY BELOW THIS POINT ############################

${EXE_NAME}: bin/${EXE_NAME}
all: doxygen ${EXE_NAME}
doxygen: doxygen/latex/refman.pdf

clean:
	rm -rf build
	@touch ${DOXY_CFG}

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

########################### BUILD DOXYGEN FILE #################################

doxygen/latex/refman.pdf: ${DOXY_CFG} ${patsubst %,${SRC_DIR}%,${HEADER_FILES}}\
		${patsubst %,${SRC_DIR}%.cc,${OBJECT_NAMES}} Makefile
	doxygen ${DOXY_CFG}
	cd doxygen/latex && make pdf
	@echo "\n\nFINISHED WRITING DOXYGEN\n\n"
