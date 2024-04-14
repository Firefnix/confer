#!/usr/bin/env python3

import sys
from os import mkdir, remove, getcwd, chdir, path
from glob import glob
import argparse
import subprocess

# CHANGE THIS TO CONFER INSTALLATION DIRECTORY
confer_include = "/home/malo/Bureau/confer/src/"
confer_obj = "/home/malo/Bureau/confer/obj/confer.o"

test_main = "test/test.c"
test_bin = "./bin/test"


def error(msg: str):
    print("Error: " + msg)
    sys.exit()


def warning(msg: str):
    print("Warning: " + msg)


def main():
    args = get_args()
    args.func(args)


def get_project_name():
    return getcwd().split("/")[-1]


def create(args):
    print(f"Creating project '{args.name}'")
    create_dir_if_not(args.name)
    chdir(args.name)
    for i in ["src", "test", "obj", "bin"]:
        create_dir_if_not(i)
    if not args.lib:
        create_file_with_main_if_not(c_file(args.name))
    create_file_with_main_if_not(test_main, include_confer=True)
    if args.git:
        init_git_repo()
    create_makefile_or_fail(args)


def create_dir_if_not(dir_path):
    if not path.isdir(dir_path):
        mkdir(dir_path)


def create_file_with_main_if_not(file_path, include_confer=False):
    content = "int main(int argc, char *argv[]) {\n\n    return 0;\n}"
    if include_confer:
        content = "#include <confer.h>\n\nint main(int argc, char *argv[]) {\n    cfInit();\n\n    cfPrintCallTree();\n    return cfReturnCode();\n}"
    if not path.isfile(file_path):
        with open(file_path, "x", encoding="utf8") as f:
            f.write(content)
    else:
        warning(f"file {file_path} already exists, ignoring")


def create_makefile_or_fail(args):
    if path.isfile("Makefile"):
        error(f"there is already a Makefile in {getcwd()}")
    # no objects by default
    update_makefile(args.name, [], args.sdl, args.lib)


def update_makefile(name: str, obj_list, sdl, lib):
    with open("Makefile", "w", encoding="utf8") as f:
        f.write(makefile_content(name, obj_list, sdl, lib))


def makefile_content(name: str, obj_list, sdl: bool, lib: bool):
    warning = "# AUTOMATICALLY GENERATED FILE. MODIFY WITH CAUTION.\n"
    flags = get_flags(sdl)
    clean = "clean:\n\trm ${TARGETS}\n"
    bin_or_obj_if_lib = (
        f"""obj/{name}.o: $(OBJ_FILES)\n\t@echo \"📦 $(OBJ_FILES) -> $@\"
\t@ld -r $(OBJ_FILES) -o $@"""
        if lib
        else f"""\nbin/{name}: {c_file(name)} $(OBJ_FILES)
\t@echo "⚙ {c_file(name)} $(OBJ_FILES) -> bin/{name}"
\t@$(CC) {c_file(name)} $(OBJ_FILES) -o bin/{name} $(CFLAGS)"""
    )
    return (
        warning
        + f"""CC = gcc\n.PHONY : clean\n
OBJ_FILES = {' '.join(map(o_file, obj_list))}
TARGETS = {'' if lib else f'bin/{name}'} $(OBJ_FILES)\n
{flags}
{bin_or_obj_if_lib}
\nobj/%.o: src/%.c
\t@echo "🔨 $< -> $@"
\t@$(CC) -I./src -c $< -o $@ $(CFLAGS)
\n{test_bin}: {test_main} $(OBJ_FILES)
\t@$(CC) "{test_main}" {confer_obj} $(OBJ_FILES) $(CFLAGS) -I./test -I{confer_include} -o "{test_bin}"\n\n"""
        + clean
    )


def get_flags(sdl: bool):
    flags = "CC_OPTIONS = -pedantic\nCC_INCLUDE = -I./src\nWARN = -Wall -Wextra\n"
    if sdl:
        flags += "SDL2_FLAGS = $(shell sdl2-config --cflags --libs)\nCFLAGS = $(CC_INCLUDE) $(CC_OPTIONS) $(SDL2_FLAGS) $(WARN)"
    else:
        flags += "CFLAGS = $(CC_INCLUDE) $(CC_OPTIONS) $(WARN)"
    return flags


def get_obj_list():
    if not path.isfile("Makefile"):
        error("no Makefile in " + getcwd())
    with open("Makefile", encoding="utf8") as f:
        for i in f.readlines():
            if i.startswith("OBJ_FILES ="):
                l, r = i[12:].split(), []
                for j in l:
                    if not j:
                        continue
                    if j.startswith("obj/") and j.endswith(".o"):
                        r.append(j[4:-2])
                    else:
                        warning("ignoring " + j)
                return r
        error("could not find OBJ_FILES in Makefile")


def has_sdl():
    if not path.isfile("Makefile"):
        error("no Makefile in " + getcwd())
    with open("Makefile", encoding="utf8") as f:
        return any(i.startswith("SDL2_FLAGS =") for i in f.readlines())


def is_lib():
    return not path.isfile(c_file(get_project_name().lower()))


def init_git_repo():
    if not path.isdir(".git"):
        try:
            subprocess.run(["/usr/bin/git", "init"], check=True)
        except subprocess.CalledProcessError:
            error("could not create git repository")
    else:
        warning("there is already a git repository in " + getcwd())


def test(args):
    compile_tests()
    run_tests()
    if not args.keep:
        remove(test_bin)


def compile_tests():
    try:
        subprocess.run(["/usr/bin/make", test_bin], check=True)
    except subprocess.CalledProcessError:
        error(f"could not make {test_bin}")


def run_tests():
    subprocess.run([test_bin], check=False)


def add_obj(args):
    l = get_obj_list()
    if args.name in l:
        print(f"{args.name} is already an object.")
    else:
        l.append(args.name)
        update_makefile(get_project_name(), l, has_sdl(), is_lib())
        create_header_file_if_not(args.name)
        create_c_file_if_not(args.name)


def h_file(name: str) -> str:
    return f"src/{name}.h"


def c_file(name: str) -> str:
    return f"src/{name}.c"


def o_file(name: str) -> str:
    return f"obj/{name}.o"


def create_header_file_if_not(name: str):
    if path.isfile(h_file(name)):
        return
    cap = name.replace("/", "_").upper() + "_H"
    with open(h_file(name), "w", encoding="utf8") as f:
        f.write(f"#ifndef {cap}\n#define {cap}\n\n\n\n#endif\n")


def create_c_file_if_not(name: str):
    if path.isfile(c_file(name)):
        return
    with open(c_file(name), "w", encoding="utf8") as f:
        f.write(f"#include <{name}.h>\n")


def clean(_):
    subprocess.run(["/usr/bin/make", "clean"], check=False)


def run(args):
    name = get_project_name()
    if is_lib():
        error("cannot run a library")
    try:
        subprocess.run(["/usr/bin/make", f"./bin/{name}"], check=True)
    except subprocess.CalledProcessError:
        error("cannot compile using make")
    if not args.just_compile:
        subprocess.run([f"./bin/{name}"], check=False)


def print_config(name: str, obj_list, sdl: bool, lib: bool, newline: bool = False):
    status = "🧪 executable"
    if lib:
        status = "🧪 library"
    if sdl:
        status += ", sdl"
    if newline:
        print("\n".join(obj_list))
        return
    m = max(*map(len, obj_list), 8, len(name))
    print(f"{name}".ljust(m), status)
    if not obj_list:
        print("No objects found.\nTo add an object, use `cf add <name>`")
    for i in obj_list:
        if path.isfile(o_file(i)):
            print(i.ljust(m), "🔨 built")
        else:
            print(i.ljust(m), "🚧 not built")
    if is_makefile_synchronized():
        print("Makefile".ljust(m), "🌀 synchronized")
    else:
        print("Makefile".ljust(m), "❓ not synchronized")


def list_objects(args):
    newline = hasattr(args, "newline") and args.newline
    print_config(
        get_project_name(), get_obj_list(), has_sdl(), is_lib(), newline=newline
    )


def remove_with_confirm(file: str):
    confirm = input(f"Permanently remove {file} ? [y/N] ")
    if not confirm.lower() == "n":
        remove(file)


def remove_obj(args):
    l = get_obj_list()
    if args.name not in l:
        error(f"object {args.name} does not exist")
    l.remove(args.name)
    if args.files:
        for f in [h_file, c_file]:
            file = f(args.name)
            if path.isfile(file):
                remove_with_confirm(file)
            else:
                warning(f"No such source code file: {file}")
    update_makefile(get_project_name(), l, has_sdl(), is_lib())


def rename_obj(args):
    l = get_obj_list()
    if args.old_name not in l:
        error(f"object {args.old_name} does not exist")
    for i in range(len(l)):
        if l[i] == args.old_name:
            l[i] = args.new_name
    update_makefile(get_project_name(), l, has_sdl(), is_lib())


def is_makefile_synchronized():
    if not path.isfile("Makefile"):
        error("no Makefile in " + getcwd())
    guessed_content = makefile_content(
        get_project_name(), get_obj_list(), has_sdl(), is_lib()
    )
    with open("Makefile", encoding="utf8") as f:
        return f.read() == guessed_content


def guess_obj_list():
    trim = lambda s: s[4:-2]
    h_list = map(trim, glob("src/*.h"))
    c_list = map(trim, glob("src/*.c"))
    return [i for i in h_list if i in c_list]


def sync(args):
    if not path.isfile("Makefile"):
        error("no Makefile in " + getcwd())
    name = get_project_name()
    obj_list = guess_obj_list()
    if args.sdl and args.no_sdl:
        error("cannot be SDL and no-SDL")
    if args.lib and args.exec:
        error("cannot be a library and an executable")
    sdl = has_sdl()
    if args.sdl:
        sdl = True
    if args.no_sdl:
        sdl = False
    lib = is_lib()
    if args.lib:
        lib = True
    if args.exec:
        lib = False
    if not is_lib() and lib:
        remove_with_confirm(c_file(name))
    if not args.dry_run:
        content = makefile_content(name, obj_list, sdl, lib)
        with open("Makefile", "w", encoding="utf8") as f:
            f.write(content)
        list_objects(args)
    else:
        print_config(name, obj_list, sdl, lib)


def get_args():
    b = {"action": "store_true"}
    parser = argparse.ArgumentParser(prog="cf", description="cf description")
    sub_parsers = parser.add_subparsers()

    parser_init = sub_parsers.add_parser("create", help="create a new project")
    parser_init.add_argument("name", help="the name of the new project")
    parser_init.add_argument("-g", "--git", help="init a git repository", **b)
    parser_init.add_argument("--sdl", help="add SDL flags", **b)
    parser_init.add_argument(
        "-l", "--lib", help="create a library project (no executable, etc.)", **b
    )
    parser_init.set_defaults(func=create)

    parser_add_obj = sub_parsers.add_parser("add", help="add a new object")
    help_obj_name = "the name of the new object (no extension)"
    parser_add_obj.add_argument("name", help=help_obj_name)
    parser_add_obj.set_defaults(func=add_obj)

    parser_rm_obj = sub_parsers.add_parser("rm", help="remove an object")
    parser_rm_obj.add_argument("name", help=help_obj_name)
    parser_rm_obj.add_argument("-f", "--files", help="remove files from disk", **b)
    parser_rm_obj.set_defaults(func=remove_obj)

    parser_mv_obj = sub_parsers.add_parser("mv", help="move/rename an object")
    help_mv_obj_name = "the name {} of the object (no extension)"
    parser_mv_obj.add_argument("old_name", help=help_mv_obj_name.format("old"))
    parser_mv_obj.add_argument("new_name", help=help_mv_obj_name.format("new"))
    parser_mv_obj.set_defaults(func=rename_obj)

    parser_list = sub_parsers.add_parser("list", help="list the objects")
    parser_list.add_argument(
        "-n", "--newline", help="print 1 object name per line", **b
    )
    parser_list.set_defaults(func=list_objects)

    parser_test = sub_parsers.add_parser("test", help="compile and run the tests")
    parser_test.add_argument(
        "-k", "--keep", help=f"keep {test_bin} after test run", **b
    )
    parser_test.set_defaults(func=test)

    parser_run = sub_parsers.add_parser("run", help="compile and run the main file")
    parser_run.add_argument("-j", "--just-compile", help="just compile, not run", **b)
    parser_run.set_defaults(func=run)

    parser_clean = sub_parsers.add_parser("clean", help="clean all built objects")
    parser_clean.set_defaults(func=clean)

    parser_sync = sub_parsers.add_parser("sync", help="syncronize the Makefile")
    parser_sync.add_argument("--dry-run", help="print the content of the", **b)
    parser_sync.add_argument("--sdl", help="enable SDL", **b)
    parser_sync.add_argument("--no-sdl", help="disable SDL", **b)
    parser_sync.add_argument("-l", "--lib", help="set project as library", **b)
    parser_sync.add_argument("-e", "--exec", help="set project as an executable", **b)
    parser_sync.set_defaults(func=sync)

    return parser.parse_args()


if __name__ == "__main__":
    main()
