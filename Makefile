obj = split-fusion.o mylist.o travedir.o main.o
CFLAGS = -I$(GETAROOT)/include -L$(GETAROOT)/lib

all:split-fusion tstwam perc
split-fusion:$(obj)
	cc -o split-fusion $(obj) $(CFLAGS) -ldw -ldws -ldwut -lm -ldl
perc:perc.o
	cc -o perc perc.o $(CFLAGS) -ldw -ldws -ldwut -lm -ldl
.PHONY : clean
clean :
	-rm split-fusion $(obj) tstwam tstwam.o
tstwam: tstwam.o
	cc -o tstwam tstwam.o $(CFLAGS) -ldw -ldws -ldwut -lm -ldl	
