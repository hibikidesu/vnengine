PREFIX		:=
CC			:= $(PREFIX)gcc
STRIP		:= $(PREFIX)strip

TARGET		:= vnengine
DEBUG		:= 1
CFLAGS		:= -Wall -std=c99 -O2 -DDEBUG=$(DEBUG)
ifeq ($(DEBUG), 1)
CFLAGS += -ggdb3
endif

LIBS		:= $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_mixer SDL2_image luajit zlib)
SRC			:= $(shell find src -name '*.c')
OBJS		:= $(patsubst %.c,%.o,$(SRC))

.PHONY:	\
	all	\
	clean	\

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
ifeq ($(DEBUG), 0)
	$(STRIP) $@
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
