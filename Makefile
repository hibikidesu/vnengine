PREFIX		:=
CC			:= $(PREFIX)gcc
STRIP		:= $(PREFIX)strip

TARGET		:= vnengine
DEBUG		:= 1
CFLAGS		:= -Wall -std=c99 -O2 -DDEBUG=$(DEBUG)

LIBS		:= $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_mixer SDL2_image lua zlib)
SRC			:= $(shell find src -name '*.c')
OBJS		:= $(patsubst %.c,%.o,$(SRC))

.PHONY:	\
	all	\
	clean	\

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
	$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
