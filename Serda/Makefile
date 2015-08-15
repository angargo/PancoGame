CC=@g++
CFLAGS=-Wall -O2 -std=c++11
#LFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
LFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
BNAME=serda
STATES=state_stack.o state.o troll_state.o

all: $(BNAME)

run: $(BNAME)
	@./$(BNAME)

$(BNAME): main.o game.o $(STATES)
	$(CC) $(CFLAGS) -o $(BNAME) main.o game.o $(STATES) $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm *o $(BNAME)
