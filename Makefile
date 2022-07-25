SYSCONF_LINK = g++
# CPPFLAGS     = -Wall -Wextra -Werror=narrowing -Weffc++ -Werror -std=c++17
CPPFLAGS     = 
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
TARGET  = main

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)
	$(DESTDIR)$(TARGET)
	convert $(DESTDIR)output.tga  $(DESTDIR)output.png
	convert $(DESTDIR)zbuffer.tga $(DESTDIR)zbuffer.png

$(OBJECTS): %.o: %.cpp
	$(SYSCONF_LINK) $(CPPFLAGS) -c $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f *.tga
	-rm -f *.png

