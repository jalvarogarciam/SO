
bin:= ./bin

all: ejprop3 ej1 tarea4 tarea5 ej7 sumador ejprop3_exec sleepy

sleepy : sleepy.c
	gcc $^ -o $(bin)/$@ 
ejprop3 : ejprop3.c
	gcc $^ -o $(bin)/$@ 
ejprop3_exec : ejprop3_exec.c
	gcc $^ -o $(bin)/$@ 
ej7: ej7.c 
	gcc $^ -o $(bin)/$@ 
ej1 : ej1.c
	gcc $^ -o $(bin)/$@
tarea4 : tarea4.c
	gcc $^ -o $(bin)/$@
tarea5 : tarea5.c
	gcc $^ -o $(bin)/$@
sumador : sumador.c
	gcc $^ -o $(bin)/$@
clean:
	rm -r bin/*