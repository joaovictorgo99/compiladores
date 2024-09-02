CFLAGS = -I. -g

reloc = main.o lexer.o parser.o
mybc: $(reloc)
	$(CC) -o mybc $(reloc)

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~ mybc