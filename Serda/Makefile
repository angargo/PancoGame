CC=@g++
CFLAGS=-Wall -O2 -std=c++11
#LFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
LFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
BNAME=serda
STATES=state_stack.o state.o troll_state.o title_state.o game_state.o
OBJS=main.o game.o media.o $(STATES)

all: $(BNAME)

run: $(BNAME)
	@./$(BNAME)

$(BNAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(BNAME) $(OBJS) $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm *o $(BNAME)
