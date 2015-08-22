ifneq (strip $(which clang++),)
  CC=@clang++
else
  CC=@g++
endif

CFLAGS=-Wall -O2 -std=c++11
#LFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
LFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
BNAME=serda
STATES=state_stack.o state.o troll_state.o title_state.o game_state.o
OBJS=main.o game.o media.o entity.o world.o world_manager.o $(STATES)

all: $(BNAME)

run: $(BNAME)
	@./$(BNAME)

$(BNAME): objects
	$(CC) $(CFLAGS) -o $(BNAME) $(OBJS) $(LFLAGS)

objects: $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm *o $(BNAME)
