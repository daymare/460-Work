

program: bootup commands

run: program
	./run.bash

# phony commands, for better packaging
bootup: bin/init bin/login bin/sh
commands: bin/cat bin/more


# bootup targets
bin/init: init.c
	./make.bash init

bin/login: login.c
	./make.bash login

bin/sh: sh.c
	./make.bash sh


# command targets
bin/cat: cat.c
	./make.bash cat

bin/more: more.c
	./make.bash more


# clean
clean:
	./clean.bash

