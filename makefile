CXX = g++
CXXFLAGS_RELEASE = -std=c++17 -Wall -Wextra -O2 -s
CXXFLAGS_DEBUG = -std=c++17 -Wall -Wextra -O0 -g -fsanitize=address,undefined
PREFIX ?= /usr/local

SRCS = main.cpp history.cpp
HDRS = colors.hpp history.hpp

lightshell: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS_RELEASE) $(SRCS) -o lightshell

debug: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS_DEBUG) $(SRCS) -o lightshell-debug

install: lightshell
	install -Dm755 lightshell $(DESTDIR)$(PREFIX)/bin/lightshell

clean:
	rm -f lightshell lightshell-debug

.PHONY: debug install clean
