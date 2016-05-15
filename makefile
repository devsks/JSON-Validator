jvalid:	jvalid.cpp
	g++ jvalid.cpp	-o jvalid
install:
	cp jvalid /usr/local/bin
clean:
	rm -f *.o a.out core jvalid 	
