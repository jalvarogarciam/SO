
bin:= ./bin

all: ejprop3 ej1 tarea4 tarea5 ej7

ejprop3 : ejprop3.c
	gcc $^ -o $(bin)/$@ 
ej7: ej7.c 
	gcc $^ -o $(bin)/$@ 
ej1 : ej1.c
	gcc $^ -o $(bin)/$@
tarea4 : tarea4.c
	gcc $^ -o $(bin)/$@
tarea5 : tarea5.c
	gcc $^ -o $(bin)/$@
clean:
	rm -r bin/*