from os import path
from yaml import safe_load
from argparse import ArgumentParser


ENTITIES = {}


def parse_device_cpu()


DEVICE_TYPES = {
    "cpu":
}
"cpu",


def get_micros_root_directory():
    script_path = path.dirname(path.abspath(__file__))
    return path.normpath(path.join(script_path, ".."))


def parse_description_for_arch(arch_name):
    print(f"Parsing ARCH descriptions: {arch_name}")
    arch_name = arch_name.replace("-", "/")
    arch_name = arch_name.replace(",", "/")
    micros_root = get_micros_root_directory()
    with open(path.join(micros_root, "arch", arch_name, "arch.yml"), "r") as file:
        descriptions = safe_load(file)

    for entry in descriptions:
        print(f"\n{entry}:")
        if 'type' in descriptions[entry]:
            print(f"- Type: {descriptions[entry]['type']}")

        for value in descriptions[entry]:
            print(f"- {value}")


def parse_descriptions_for_soc(soc_name):
    print(f"Parsing SoC descriptions: {soc_name}")
    soc_name = soc_name.replace("-", "/")
    soc_name = soc_name.replace(",", "/")
    micros_root = get_micros_root_directory()
    with open(path.join(micros_root, "soc", soc_name, "soc.yml"), "r") as file:
        descriptions = safe_load(file)

    for entry in descriptions:
        print(f"\n{entry}:")
        if descriptions[entry]['type'] == 'cpu':
            parse_description_for_cpu(descriptions[entry]['compatible'])
            continue

        for value in descriptions[entry]:
            print(f"- {value}")


def parse_descriptions_for_board(board_name):
    micros_root = get_micros_root_directory()
    with open(path.join(micros_root, "boards", board_name, "board.yml"), "r") as file:
        descriptions = safe_load(file)

    for entry in descriptions:
        if descriptions[entry]['type'] == 'soc':
            parse_descriptions_for_soc(descriptions[entry]['compatible'])
            continue
        for value in descriptions[entry]:
            print(f"- {value}")


def parse_args():
    parser = ArgumentParser(
        description="MicrOS device description parser - run it in MicrOS root directory")
    parser.add_argument(
        "board", type=str, help="Board to parse description for")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    parse_descriptions_for_board(args.board)
