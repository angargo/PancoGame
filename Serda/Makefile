CC=@g++
CFLAGS=-Wall -O2 -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system
BNAME=serda

all: $(BNAME)

run: $(BNAME)
	@./$(BNAME)

$(BNAME): main.o
	$(CC) $(CFLAGS) -o $(BNAME) main.o 
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm *o $(BNAME)
