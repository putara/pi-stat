CPP = g++
LINK = gcc
CPPFLAGS = -O3 -Wall -fno-exceptions
LINKFLAGS =
LIBS =
BINDIR = ./bin
OBJDIR = ./obj
TARGET = $(BINDIR)/pi-stat
TARGET_OBJS = $(OBJDIR)/main.o

all: $(OBJDIR) $(BINDIR) $(TARGET)

clean: cleanobj
	-rm -r $(TARGET)
	-rm -r $(OBJDIR) $(BINDIR)

cleanobj:
	-rm -f $(TARGET_OBJS)

$(OBJDIR)/%.o: %.cpp
	$(CPP) -c -o $@ $< $(CPPFLAGS)

$(TARGET) : $(TARGET_OBJS)
	$(LINK) -o $@ $(LINKFLAGS) $(LIBS) $(TARGET_OBJS)

$(OBJDIR):
	test -d $(OBJDIR) || mkdir $(OBJDIR)

$(BINDIR):
	test -d $(BINDIR) || mkdir $(BINDIR)

main.cpp: ssd1306.hpp helpers.hpp render.hpp config.hpp types.hpp lcd.hpp font.h
