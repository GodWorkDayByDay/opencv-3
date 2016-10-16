PROG = Cartoonifier_Desktop
OBJS = main_desktop.o cartoon.o ImageUtils_0.7.o
#DEBUG = -g
INCLUDE_OPENCV = `pkg-config opencv --cflags`
LINK_OPENCV = `pkg-config opencv --libs`

CFLAGS = -Wall -c $(DEBUG) $(INCLUDE_OPENCV) -I$(INCLUDE_PATH)
CXXFLAGS = $(CFLAGS)
LDFLAGS = -Wall $(DEBUG) $(LINK_OPENCV)
CC = gcc
CXX = g++
RM = rm -f

all:	$(PROG)

$(PROG):	$(OBJS) $(HEADERS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(PROG)


run:
	./$(PROG)


# Add a phony target for "clean", in case there is actually a file named "clean"!
# Also note that there is a hyphen in front of "rm", which says to ignore errors,
# otherwise it would stop once it can't find a file.
.PHONY:	clean
clean:
	-$(RM) $(OBJS)
