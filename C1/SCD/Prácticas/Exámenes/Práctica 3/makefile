.SUFFIXES:
.PHONY:    pc,pc2,pcm,fp,f,fi,fc,clean

compilador := mpicxx -std=c++11
flagsc     := -g -Wall

pc: prodcons_exe
	mpirun -np  3 ./$<

pc2: prodcons2_exe
	mpirun -np  3 ./$<

pcm: prodcons2-mu_exe
	mpirun -np 10 --oversubscribe ./$<

pcml: prodcons2-mu-LIFO_exe
	mpirun -np 10 --oversubscribe ./$<

fp: filosofos-plantilla_exe
	mpirun -np 10 --oversubscribe ./$<

fi: filosofos-interb_exe
	mpirun -np 10 --oversubscribe ./$<

f: filosofos_exe
	mpirun -np 10 --oversubscribe ./$<

fc: filosofos-cam_exe
	mpirun -np 11 --oversubscribe ./$<

fcp: filosofos-probe_exe
	mpirun -np 11 --oversubscribe ./$<

fci: filosofos-iprobe_exe
	mpirun -np 11 --oversubscribe ./$<

camarero: camarero4_exe
	mpirun -np 11 --oversubscribe ./$<

%_exe: %.cpp
	$(compilador) $(flagsc) -o $@ $<

clean:
	rm -rf *_exe *.dSYM
