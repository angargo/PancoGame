CC=@g++
CFLAGS=-Wall -O2 -std=c++11
#LFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
LFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
BNAME=serda

all: $(BNAME)

run: $(BNAME)
	@./$(BNAME)

$(BNAME): main.o game.o state_stack.o state.o
	$(CC) $(CFLAGS) -o $(BNAME) main.o game.o state_stack.o state.o $(LFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm *o $(BNAME)
