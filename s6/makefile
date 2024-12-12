
bin := ./bin

all: tarea13 prueba_tarea13 bloqueador

tarea13: tarea13.c
	gcc -o $(bin)/$@ $^

prueba_tarea13: prueba_tarea13.c
	gcc -o $(bin)/$@ $^

bloqueador: bloqueador.c
	gcc -o $(bin)/$@ $^

clean:
	rm -r $(bin)/*