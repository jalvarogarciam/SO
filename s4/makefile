
bin:= ./bin


all: ejprop4 $(bin)/reciboSignal $(bin)/tarea9 \
	$(bin)/tarea10 $(bin)/tarea11 $(bin)/contador
$(bin)/envioSignal: envioSignal.c
	gcc $^ -o $(bin)/$@

$(bin)/reciboSignal: reciboSignal.c
	gcc $^ -o $(bin)/$@

tarea6: tarea6.c
	gcc $^ -o $(bin)/$@

tarea7: tarea7.c
	gcc $^ -o $(bin)/$@

tarea8: tarea8.c
	gcc $^ -o $(bin)/$@

esclavo: esclavo.c
	gcc $^ -o $(bin)/$@

clean:
	rm -f $(bin)/*