#Makefile

#OBJ = main.cpp

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

biometrieExe : imgProcessing.cpp main.cpp
	g++ $(CFLAGS) $(LIBS) -o $@ $<

#depend:
#	sed '/^#DEP/q' makefile > mk.tmp
#	c++ -M *.C >> mk.tmp
#	mv mk.tmp makefile

#DEPEDENCIES : don't remove that line

clean:
	rm -rf *.o

mrproper: clean
	rm -rf t
