.SUFFIXES:
.PHONY: x1, x2, clean

compilador:=g++
opcionesc:= -std=c++11 -pthread -Wfatal-errors
hmonsrcs:= HoareMonitor.h HoareMonitor.cpp Semaphore.h Semaphore.cpp

x1: monitor_em_exe
	./$<

x2: barrera1_sc_exe
	./$<

x3: barrera2_sc_exe
	./$<

x4: barrera2_su_exe
	./$<

x5: prodcons1_sc_exe
	./$<

x6: prodcons2_sc_exe
	./$<

x7: prodcons2_su_exe
	./$<

monitor_em_exe: monitor_em.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera1_sc_exe: barrera1_sc.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera2_sc_exe: barrera2_sc.cpp
	$(compilador) $(opcionesc)  -o $@ $<

barrera2_su_exe: barrera2_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $< HoareMonitor.cpp Semaphore.cpp

prodcons1_sc_exe: prodcons1_sc.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<

prodcons2_sc_exe: prodcons2_sc.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<

prodcons2_su_exe: prodcons2_su.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

pc_sc: Productor_consumidor_SC.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

pc_su: Productor_consumidor_SU.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

fumadores: fumadores.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

barbero: barbero.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

barbero2: barbero2.cpp $(hmonsrcs)
	$(compilador) $(opcionesc)  -o $@ $<  HoareMonitor.cpp Semaphore.cpp

clean:
	rm -f *_exe
