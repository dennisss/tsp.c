BUILD_PATH = .
OBJ_PATH = obj
CC = g++


# TODO: Eventually remove the warning suppression
CFLAGS = -w -fpermissive -s -O3 -pthread

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG
endif

ifeq ($(SPARSE),1)
	CFLAGS += -DTSP_GRAPH_IMPL=2
else
	CFLAGS += -DTSP_GRAPH_IMPL=1
endif

ifeq ($(VIEWER), 1)
	CFLAGS += -lGL -lGLEW -lglfw -DBUILD_VIEWER
endif

tsp_TARGET = tsp
tsp_SRCS = $(shell find src -name '*.c')
tsp_OBJS = $(patsubst src/%.c, obj/%.o, $(tsp_SRCS))




$(tsp_TARGET): objdir $(tsp_OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_PATH)/$(tsp_TARGET) $(tsp_OBJS)

objdir:
	mkdir -p obj
	mkdir -p obj/generate

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<



clean:
	rm -f output.tour
	rm -rf obj
	rm -f $(tsp_TARGET)
