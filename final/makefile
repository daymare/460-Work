

program: bootup commands

run: program
	./run.bash

# phony commands, for better packaging
bootup: bin/init bin/login bin/sh
commands: bin/cat bin/more bin/l2u bin/grep

# standard includes
lib: ucode.c util.c

# bootup targets
bin/init: init.c lib
	./make.bash init

bin/login: login.c lib
	./make.bash login

bin/sh: sh.c lib
	./make.bash sh


# command targets
bin/cat: cat.c lib
	./make.bash cat

bin/more: more.c lib
	./make.bash more

bin/l2u: l2u.c lib
	./make.bash l2u

bin/grep: grep.c lib
	./make.bash grep


# clean
clean:
	./clean.bash

