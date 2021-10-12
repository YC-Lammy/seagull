CC=go

all: clean build

build:
	$(CC) build .

.PHONY: clean
clean:
