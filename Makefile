CC = gcc
CFLAGS = -Wall
LIBS = -lGL -lGLU -lglut

OUT = build
NAME = fractal_landscapes

SRC = src
SRCS := $(shell find $(SRC) -name '*.c')

all: $(OUT)
	$(CC) -o $(OUT)/$(NAME) $(SRCS) $(CFLAGS) $(LIBS)

$(OUT):
	mkdir $(OUT)

clean:
	rm -rf $(OUT)
