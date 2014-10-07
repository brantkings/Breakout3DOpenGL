CC = g++ -Wall -g
OUT = breakout
LIB = -lglut -lGLU -lGL
DEPEND = glfont.o util.o entity.o camp.o ball.o racket.o hud.o game.o main.o  

all: $(DEPEND)
	$(CC) $^ -o $(OUT) $(LIB)

%.o : %.cpp %.h
	@echo " Compilando \"$@\""
	@$(CC) $< -c $(LIB) 

clean:
	rm -rf *.o 
	clear
