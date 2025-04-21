########## COMPILATION SETTINGS ####################################################################

CFLG=-O3 -Wall -std=gnu17
LIBS=-lm
CLEAN=rm -f *.out *.o *.a



########## PROGRAMS ################################################################################

00_math.o: 00_math.c EASY64.h # ------------- Parse and do math
01_String-test.o: 01_String-test.c EASY64.h # Construct and print strings



########## DEPENDENCIES ############################################################################

### Structure Toolbox ###
Structs.o: Structs.c EASY64.h
String.o: String.c EASY64.h



########## ARCHIVES & GENERAL RULES ################################################################

components.a: Structs.o String.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG)  $<



########## PROGRAM & CLEAN RULES ###################################################################


##### Program Rules #######################################################

00_math: 00_math.o components.a
	gcc $(CFLG) -o $@.out $^ $(LIBS)

01_String-test: 01_String-test.o components.a
	gcc $(CFLG) -o $@.out $^ $(LIBS)


##### Clean Rule ##########################################################

# Clean
clean:
	$(CLEAN)
