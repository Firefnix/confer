#!/usr/bin/env python3

from os import mkdir, remove, getcwd, chdir, path
import argparse, subprocess

# CHANGE THIS TO CONFER INSTALLATION DIRECTORY
confer_include = "/home/malo/Bureau/confer/src/"
confer_obj = "/home/malo/Bureau/confer/obj/confer.o"


def error(msg: str):
    print("Error: " + msg)
    exit()


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
        create_file_with_main_if_not(f"src/{args.name}.c")
    create_file_with_main_if_not("test/test.c", include_confer=True)
    if args.git:
        init_git_repo()
    create_makefile_or_fail(args)


def create_dir_if_not(dir_path):
    if not path.isdir(dir_path):
        mkdir(dir_path)


def create_file_with_main_if_not(file_path, include_confer=False):
    content = "int main(int argc, char *argv[]) {\n    return 0;\n}"
    if include_confer:
        content = "#include <confer.h>\n\n" + content
    if not path.isfile(file_path):
        with open(file_path, "x") as f:
            f.write(content)
    else:
        warning(f"file {file_path} already exists, ignoring")


def create_makefile_or_fail(args):
    if path.isfile("Makefile"):
        error(f"there is already a Makefile in {getcwd()}")
    # no objects by default
    update_makefile(args.name, [], args.sdl, args.lib)


def update_makefile(name: str, obj_list, sdl, lib):
    with open("Makefile", "w") as f:
        f.write(makefile_content(name, obj_list, sdl, lib))


def makefile_content(name: str, obj_list, sdl, lib):
    warning = "# AUTOMATICALLY GENERATED FILE. MODIFY WITH CAUTION.\n"
    flags = get_flags(sdl)
    clean = "clean:\n\trm ${TARGETS}\n"
    bin_if_lib = (
        ""
        if lib
        else f"""\nbin/{name}: src/{name}.c $(OBJ_FILES)
\t@echo "⚙ src/{name}.c $(OBJ_FILES) -> bin/{name}"
\t@$(CC) src/{name}.c $(OBJ_FILES) -o bin/{name} $(CFLAGS)"""
    )
    return (
        warning
        + f"""CC = gcc\n.PHONY : clean\n
OBJ_FILES = {' '.join([f"obj/{i}.o" for i in obj_list])}
TARGETS = {'' if lib else f'bin/{name}'} $(OBJ_FILES)\n
{flags}
{bin_if_lib}
\nobj/%.o: src/%.c
\t@echo "⚙ $< -> $@"
\t@$(CC) -I./src -c $< -o $@ $(CFLAGS)
\nbin/test: test/test.c $(OBJ_FILES)
\t@$(CC) "test/test.c" {confer_obj} $(OBJ_FILES) $(CFLAGS) -I./test -I{confer_include} -o "bin/test"\n\n"""
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
    with open("Makefile") as f:
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
        else:
            error("could not find OBJ_FILES in Makefile")


def has_sdl():
    if not path.isfile("Makefile"):
        error("no Makefile in " + getcwd())
    with open("Makefile") as f:
        return any(i.startswith("SDL2_FLAGS =") for i in f.readlines())


def is_lib():
    return not path.isfile(f"src/{get_project_name().lower()}.c")


def init_git_repo():
    if not path.isdir(".git"):
        try:
            subprocess.run(["/usr/bin/git", "init"], check=True)
        except:
            error("could not create git repository")
    else:
        warning("there is already a git repository in " + getcwd())


def test(args):
    compile_tests()
    run_tests()
    if not args.keep:
        remove("./bin/test")


def compile_tests():
    try:
        subprocess.run(["/usr/bin/make", "bin/test"], check=True)
    except:
        error("could not make bin/test")


def run_tests():
    subprocess.run(["./bin/test"])


def add_obj(args):
    l = get_obj_list()
    if args.name in l:
        print(f"{args.name} is already an object.")
    else:
        l.append(args.name)
        update_makefile(get_project_name(), l, has_sdl(), is_lib())
        create_header_file_if_not(args.name)
        create_c_file_if_not(args.name)


def create_header_file_if_not(name: str):
    if path.isfile(f"src/{name}.h"):
        return
    cap = name.replace("/", "_").upper() + "_H"
    with open(f"src/{name}.h", "w") as f:
        f.write(f"#ifndef {cap}\n#define {cap}\n\n\n\n#endif\n")


def create_c_file_if_not(name: str):
    if path.isfile(f"src/{name}.c"):
        return
    with open(f"src/{name}.c", "w") as f:
        f.write(f"#include <{name}.h>\n")


def clean(args):
    subprocess.run(["/usr/bin/make", "clean"])


def run(args):
    name = get_project_name()
    if is_lib():
        error("cannot run a library")
    try:
        subprocess.run(["/usr/bin/make", f"./bin/{name}"], check=True)
    except:
        error("cannot compile using make")
    if not args.just_compile:
        subprocess.run([f"./bin/{name}"])


def list(args):
    name = get_project_name()
    l = get_obj_list()
    if l:
        print(", ".join(i for i in l))
    else:
        print(f"No objects in {name}.\nTo add an object, use `cf add-obj <name>`")


def remove_obj(args):
    l = get_obj_list()
    if args.name not in l:
        error(f"object {args.name} does not exist")
    l.remove(args.name)
    update_makefile(get_project_name(), l, has_sdl(), is_lib())


def rename_obj(args):
    l = get_obj_list()
    if args.old_name not in l:
        error(f"object {args.old_name} does not exist")
    for i in range(len(l)):
        if l[i] == args.old_name:
            l[i] = args.new_name
    update_makefile(get_project_name(), l, has_sdl(), is_lib())


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
    parser_rm_obj.set_defaults(func=remove_obj)

    parser_mv_obj = sub_parsers.add_parser("mv", help="move/rename an object")
    help_mv_obj_name = "the name {} of the object (no extension)"
    parser_mv_obj.add_argument("old_name", help=help_mv_obj_name.format("old"))
    parser_mv_obj.add_argument("new_name", help=help_mv_obj_name.format("new"))
    parser_mv_obj.set_defaults(func=rename_obj)

    parser_list = sub_parsers.add_parser("list", help="list the objects")
    parser_list.set_defaults(func=list)

    parser_test = sub_parsers.add_parser("test", help="compile and run the tests")
    parser_test.add_argument("-k", "--keep", help="keep bin/test after test run", **b)
    parser_test.set_defaults(func=test)

    parser_run = sub_parsers.add_parser("run", help="compile and run the main file")
    parser_run.add_argument("-j", "--just-compile", help="just compile, not run", **b)
    parser_run.set_defaults(func=run)

    parser_clean = sub_parsers.add_parser("clean", help="clean all objects")
    parser_clean.set_defaults(func=clean)

    return parser.parse_args()


if __name__ == "__main__":
    main()
