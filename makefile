CXX = g++
CXXFLAGS = -Wall -g -std=c++11

SRCS = butterfly-main.cpp butterfly.cpp
OBJS = $(SRCS:.cpp=.o)

all: butterfly-output

butterfly-output: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o butterfly-output

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) butterfly-output
