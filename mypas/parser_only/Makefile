CFLAGS = -I. -g

reloc = main.o lexer.o parser.o keywords.o symtab.o

mypas: $(reloc)
	$(CC) -o mypas $(reloc)

clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~ mypas

