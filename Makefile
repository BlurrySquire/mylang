CC := gcc
LD := gcc

CFLAGS := \
	-Wall -Wextra \
	-std=c99 \
	-Isrc

LDFLAGS :=

SOURCES := $(shell find src -name '*.c')
OBJECTS := $(SOURCES:src/%.c=build/%.o)

TARGET := mylang

all: $(TARGET)

clean:
	rm -rf $(TARGET) build

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
