
BUILD_PATH = .
OBJ_PATH = obj
CC = g++
CFLAGS = -w -fpermissive -std=c++0x -O3 #-lGL -lGLEW #-Iext -DVERSION=$(VERSION) # -pthread


tsp_TARGET = tsp
tsp_SRCS = $(shell find src -name '*.c')
tsp_OBJS = $(patsubst src/%.c, obj/%.o, $(tsp_SRCS))


$(tsp_TARGET): objdir $(tsp_OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$(tsp_TARGET) $(tsp_SRCS)

objdir:
	mkdir -p obj

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf obj
	rm -f $(tsp_TARGET)
