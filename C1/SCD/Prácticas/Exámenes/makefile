.SUFFIXES:
.PHONY: pc
.PRECIOUS: %_exe

compilador:=g++

all: pc fu

pc: prodcons_exe
	./$<

pcm: prodcons_mutex_exe
	./$<

fu: fumadores_exe
	./$<

fusm: fumadores_sin_monitor_exe
	./$<

sum: fumadores_suministrador_exe
	./$<

est: estancos2_exe
	./$<

%_exe: %.cpp
		$(compilador) -std=c++11 -pthread -o $@ $< Semaphore.cpp -g

clean:
	rm -f *_exe
