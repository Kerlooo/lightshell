CXX = g++
CXXFLAGS_RELEASE = -std=c++17 -O2 -s
CXXFLAGS_DEBUG = -std=c++17 -O0 -g -fsanitize=address,undefined

SRCS = main.cpp history.cpp
HDRS = colors.hpp history.hpp

lightshell: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS_RELEASE) $(SRCS) -o lightshell

debug: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS_DEBUG) $(SRCS) -o lightshell-debug

.PHONY: debug
