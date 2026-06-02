CXX = g++
CXXFLAGS = -std=c++11
TARGET = terminal
MAIN = main.cpp

all: $(TARGET)

$(TARGET): $(MAIN)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN)

clean:
	rm -f $(TARGET)