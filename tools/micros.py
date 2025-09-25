from argparse import ArgumentParser
from os import path
from shutil import rmtree
from subprocess import run


BOARDS = [
    "lm/lm3s6965evb",
    "st/stm32vldiscovery"
]

QEMU_BOARDS = {
    "lm/lm3s6965evb": "lm3s6965evb",
    "st/stm32vldiscovery": "stm32vldiscovery"
}

SAMPLES = [
    "hello_world",
    "init_functions"
]


def get_microsproject_path():
    script_path = path.dirname(path.abspath(__file__))
    return path.normpath(path.join(script_path, ".."))


def parse_args():
    parser = ArgumentParser(
        description="MicrOS project management tool - run it in MicrOS root directory")

    subparser = parser.add_subparsers(dest="command", required=True)
    build_parser = subparser.add_parser(
        "build", help="Build specified sample for specified board")
    build_parser.add_argument(
        "board", choices=BOARDS,  help="Board to build for")
    build_parser.add_argument(
        "sample", choices=SAMPLES, help="Sample to build")
    build_parser.add_argument(
        "-t", "--type", choices=["Debug", "Release"], default="Debug", help="Build type")
    build_parser.add_argument(
        "-c", "--clean", action="store_true", help="Clean build directory before building")
    build_parser.add_argument(
        "-r", "--run", action="store_true", help="Run the built sample in QEMU")
    build_parser.add_argument(
        "-p", "--path", type=str, default='build', help="Path to the built sample")

    return parser.parse_args()


def main():
    args = parse_args()
    if args.command == "build":
        build_dir = path.join(get_microsproject_path(), args.path)
        if args.clean and path.exists(build_dir):
            rmtree(build_dir)

        cmake_cmd = [
            "cmake",
            "-S", get_microsproject_path(),
            "-B", build_dir,
            f"-DMICROS_BOARD={args.board}",
            f"-DMICROS_SAMPLE={args.sample}",
            f"-DCMAKE_BUILD_TYPE={args.type}",
            "-GNinja"
        ]
        run(cmake_cmd, check=True)

        run(["cmake", "--build", build_dir], check=True)

        if args.run:
            elf_path = path.join(
                build_dir, "samples", args.sample, f"{args.sample}.elf")
            qemu_cmd = [
                "qemu-system-arm",
                "-M", QEMU_BOARDS[args.board],
                "-nographic",
                "-kernel", elf_path
            ]
            run(qemu_cmd, check=True)


if __name__ == "__main__":
    main()
