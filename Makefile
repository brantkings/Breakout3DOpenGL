CC = g++ -Wall -g
OUT = breakout
LIB = -lglut -lGLU -lGL
DEPEND = glfont.o util.o entidade.o fase.o bola.o raquete.o hud.o jogo.o main.o  

all: $(DEPEND)
	$(CC) $^ -o $(OUT) $(LIB)

%.o : %.cpp %.h
	@echo " Compilando \"$@\""
	@$(CC) $< -c $(LIB) 

clear:
	rm -rf *.o 
	clear
