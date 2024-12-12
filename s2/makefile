bin := ./bin

all: ejprop2 tarea3 ej3

ejprop2: ejprop2.c
	gcc -o $(bin)/$@ $^

ej3: ej3.c
	gcc -o $(bin)/$@ $^

tarea3: tarea3.c
	gcc -o $(bin)/$@ $^

clean:
	rm -r $(bin)/*