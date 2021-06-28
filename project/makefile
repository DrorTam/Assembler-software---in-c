all: main.o SysConfig.o RegisterManager.o LexicalAnalyzerList.o Tables.o TablesContainer.o FileOutput.o Parser.o Decoder.o
	gcc -Wall -ansi -pedantic -o assembler main.o SysConfig.o RegisterManager.o LexicalAnalyzerList.o Tables.o TablesContainer.o FileOutput.o Parser.o Decoder.o

main.o: main.c SysConfig.h SysConfig.h RegisterManager.h Decoder.h TablesContainer.h FileOutput.h
	gcc -Wall -ansi -pedantic -c main.c

SysConfig.o: SysConfig.c SysConfig.h
	gcc -Wall -ansi -pedantic -c SysConfig.c

RegisterManager.o: RegisterManager.c RegisterManager.h SysConfig.h
	gcc -Wall -ansi -pedantic -c RegisterManager.c

LexicalAnalyzerList.o: LexicalAnalyzerList.c LexicalAnalyzerList.h SysConfig.h
	gcc -Wall -ansi -pedantic -c LexicalAnalyzerList.c

Tables.o: Tables.c Tables.h SysConfig.h
	gcc -Wall -ansi -pedantic -c Tables.c

TablesContainer.o: TablesContainer.c Tables.h
	gcc -Wall -ansi -pedantic -c TablesContainer.c

FileOutput.o: FileOutput.c SysConfig.h TablesContainer.h
	gcc -Wall -ansi -pedantic -c FileOutput.c

Parser.o: Parser.c Parser.h SysConfig.h RegisterManager.h LexicalAnalyzerList.h
	gcc -Wall -ansi -pedantic -c Parser.c

Decoder.o: Decoder.c Decoder.h SysConfig.h RegisterManager.h Parser.h TablesContainer.h Tables.h
	gcc -Wall -ansi -pedantic -c Decoder.c

clean:
	rm *.o
