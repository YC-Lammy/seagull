
CC=gcc

CFLAGS += `pkg-config --cflags gtk+-3.0 webkit2gtk-4.0`
LDFLAGS += `pkg-config --libs gtk+-3.0 webkit2gtk-4.0`

all: clean browser

browser:
	$(CC) $(CFLAGS) browser.c $(OBJECTS) -o browser $(LDFLAGS)

.PHONY: clean
clean:
	rm -f browser $(OBJECTS)
