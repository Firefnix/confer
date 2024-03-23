---
title: Command line tool (CF)
---

Command line tool
=================

Confer comes with CF, a handy tool to manage your project and run your tests. It can create projects, build and run tests, and add/rename/remove independant objects.

{% warning Optional tool %}
Keep in mind that this tool is not compulsory to use Confer, and that you can do everything this tool does by calling gcc/clang, make and editing files.
{% end %}

You can see this tool as a fast way to setup and use Confer.

## Creating a project

To create a Confer project, use the `cf create` subcommand. Here is the complete documentation of this subcommand:
```
usage: cf create [-h] [-g] [--sdl] [-l] name

positional arguments:
  name        the name of the new project

optional arguments:
  -h, --help  show this help message and exit
  -g, --git   init a git repository
  --sdl       add SDL flags
  -l, --lib   create a library project (no executable, etc.)
```

For example, to create an executable project named "myproject", enter `cf create myproject` in a terminal. This will:
* Create a `myproject` directory *
* Create `src`, `test`, `obj` and `bin` directories inside it *
* Create pre-filled C files at `src/myproject.c` and `test/test.c` *
* Create a pre-filled `Makefile` *
* If the `--git` argument is passed, initialize a Git repository *

\* *if it does not exist already*

## Objects

A Confer project is made of *objects*. An object has a name, let's say `a`, and is a pair of files : a C language file (e.g. `a.c`) and a C header file (e.g. `a.h`).

At compile time, they are compiled together using `make` into an object file (i.e. `a.o`). All the independant objects of our project can then be compiled into our main `my-project` executable file (see [running the project](/command_line_tool#running-the-project)).

{% info ⚡ Building faster %}
Building the project's parts indepdendantly that way is by far faster than recompiling everything after modifying one file.
{% end %}

CF provides commands to add, remove, and rename objects easily, but you can also edit them directly by editing the project's `Makefile`.

### Adding objects

To add an object to your project, let's say `a`, enter `cf add a`.

Here is the `run` subcommand's complete documentation:
```
usage: cf add [-h] name

positional arguments:
  name        the name of the new object (no extension)

optional arguments:
  -h, --help  show this help message and exit
```

### Renaming objects

To rename an object `a` into `b`, enter `cf mv a b`. Both `a` and `b` must be the paths from the project's root directory.

This will rename the files `a.c` and `a.h` to `b.c` and `b.h` in addition to changing the object's name in the `Makefile`.

### Removing objects

To remove an object to your project, let's say `a`, enter `cf rm a`.

This will **not** remove the object's file (here, `a.c` and `a.h`). To do this, add the `--files` to your command.

### Listing objects

To list the project's objects, use the `cf list` command. The object's names will be separated with a space and a comma.

For example, `cf list` with Confer's source prints `assertions, build, children, print` (and a newline).

## Running the project

{% warning Non-libraries only %}
A library cannot be run.
{% end %}

Entering `cf run` in a terminal will compile your main C file (e.g. `src/myproject.c`) into an executable in the `bin` directory (e.g. `bin/myproject`) and run it.

Here is the `run` subcommand's complete documentation:
```
usage: cf run [-h] [-j]

optional arguments:
  -h, --help          show this help message and exit
  -j, --just-compile  just compile, not run
```

## Testing the project

To test the project, simply enter `cf test` in a terminal. This will compile `test/test.c` (and the files it includes) into an executable binary at `bin/test`, run this file, and delete it.

{% info 👍 Tip %}
To prevent the deletion of `bin/test` after running, type `cf test --keep`.
{% end %}

Here is the `test` subcommand's complete documentation:
```
usage: cf test [-h] [-k]

optional arguments:
  -h, --help  show this help message and exit
  -k, --keep  keep bin/test after test run
```
