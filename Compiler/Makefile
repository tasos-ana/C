all: compile avm_comp

compile: syn.c lex.c
	gcc -o compiler lex.c syn.c manage.c quad.c scopeSpaces.c symbolTable.c expr.c dataStructs.c instruction.c generator.c fileWriter.c c_main.c

compile_gdb: syn.c lex.c
	gcc -g lex.c syn.c manage.c quad.c scopeSpaces.c symbolTable.c expr.c dataStructs.c instruction.c generator.c fileWriter.c c_main.c

lex.c: lex.l
	flex -olex.c lex.l

syn.c: syn.y
	bison --yacc -v --defines --output=syn.c syn.y

avm_comp:
	gcc -o avm lex.c syn.c manage.c quad.c scopeSpaces.c symbolTable.c expr.c dataStructs.c instruction.c generator.c fileReader.c avm.c avmBranches.c avmFuncs.c avmHashtable.c avmOps.c avmTables.c execute.c avmLibfuncs.c -lm

avm_gdb:
	gcc -g lex.c syn.c manage.c quad.c scopeSpaces.c symbolTable.c expr.c dataStructs.c instruction.c generator.c fileReader.c avm.c avmBranches.c avmFuncs.c avmHashtable.c avmOps.c avmTables.c execute.c avmLibfuncs.c -lm
