obj = split-fusion.o mylist.o travedir.o main.o
CFLAGS = -I$(GETAROOT)/include -L$(GETAROOT)/lib

all:split-fusion tstwam perc getapy
split-fusion:$(obj)
	cc -o split-fusion $(obj) $(CFLAGS) -ldw -ldws -ldwut -lm -ldl
perc:perc.o
	cc -o perc perc.o $(CFLAGS) -ldw -ldws -ldwut -lm -ldl
.PHONY : clean
clean :
	-rm split-fusion $(obj) tstwam tstwam.o
tstwam: tstwam.o
	cc -o tstwam tstwam.o $(CFLAGS) -ldw -ldws -ldwut -lm -ldl	

getapy: getapy.c getapy.i
	swig -python getapy.i
	gcc -c -fPIC getapy.c getapy_wrap.c -I/usr/include/python2.7  $(CFLAGS)
	ld -shared getapy.o getapy_wrap.o -o _getapy.so $(CFLAGS) -ldw -ldws -ldwut -lm -ldl
	mv getapy.py ../pythonlib/getapy.py
	mv _getapy.so ../pythonlib/_getapy.so
