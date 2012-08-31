include config

CFLAGS := $(CFLAGS) `sdl-config --cflags` -fpermissive

LIBS = `sdl-config --libs` -lSDL_mixer
OBJECTS = src/world.o src/audio.o src/score.o src/bonus.o src/shot.o src/sprite.o src/ship.o src/alien.o src/main.o
BINARY = falsoyd

src/$(BINARY): src/config.h $(OBJECTS)
	g++ $(TARGET) $(OBJECTS) -o src/$(BINARY) $(LIBS)	

%.o: %.cpp
	g++ $(SYM) $(TARGET) $(CFLAGS) -c -o $@ $<

src/config.h: config
	echo '#define PREFIX "$(PREFIX)"' > src/config.h

install:
	mkdir -p $(PREFIX)
	mkdir -p $(BIN_PREFIX)
	cp src/$(BINARY) $(BIN_PREFIX)
	cp -a ./gfx ./sounds $(PREFIX)

uninstall:
	rm -rf $(PREFIX)
	rm -f $(BIN_PREFIX)/$(BINARY)

clean:
	rm -f src/*.o
	rm -f src/$(BINARY)
