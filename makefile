CC = g++
LIBS =
SRCS += $(wildcard *.cpp)
ROOTPATH = ../../..

INCLUDE_PATH += 

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))

ifeq "$(CXXVERSION)" "4.6.3"
	CFLAGS += -std=c++0x
else
	CFLAGS += -std=c++11
endif

TARGET = ACITest

.PHONY:all
	all:$(TARGET)

$(TARGET):
	$(CC) -c -g $(CFLAGS) $(SRCS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf *.o
	rm -rf $(TARGET)