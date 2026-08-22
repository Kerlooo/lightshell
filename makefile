CXX = g++
CXXFLAGS_RELEASE = -std=c++17 -O2 -s
CXXFLAGS_DEBUG = -std=c++17 -O0 -g -fsanitize=address,undefined

lightshell: main.cpp
	$(CXX) $(CXXFLAGS_RELEASE) main.cpp -o lightshell

debug: main.cpp
	$(CXX) $(CXXFLAGS_DEBUG) main.cpp -o lightshell-debug

.PHONY: debug