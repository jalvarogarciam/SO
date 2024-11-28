
bin:= ./bin


all: $(bin)/envioSignal $(bin)/reciboSignal $(bin)/tarea9 \
	$(bin)/tarea10 $(bin)/tarea11 $(bin)/contador
$(bin)/envioSignal: envioSignal.c
	gcc $^ -o $@

$(bin)/reciboSignal: reciboSignal.c
	gcc $^ -o $@

$(bin)/tarea9: tarea9.c
	gcc $^ -o $@

$(bin)/tarea10: tarea10.c
	gcc $^ -o $@

$(bin)/tarea11: tarea11.c
	gcc $^ -o $@

$(bin)/contador: contador.c
	gcc $^ -o $@

clean:
	rm -f $(bin)/*