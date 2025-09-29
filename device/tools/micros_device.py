# type - where to look at
# compatible - which description to use
# name - optional, for printing
# key - id

from argparse import ArgumentParser
from os import path

from yaml import safe_load

DESCRIPTION_TYPES = ["board", "soc", "arch"]

DESCRIPTION_DETAILS = {}

DESCRIPTION = {}

DESCRIPTION_DEFINES = {}


MICROS_ROOT = path.normpath(path.join(
    path.dirname(path.abspath(__file__)), "..", ".."))


def parse_args():
    parser = ArgumentParser(description="Parse micros device descriptions")
    parser.add_argument("name", type=str,
                        help="Description name, e.g. lm,lm3s6965evb")
    parser.add_argument("-t", "--type", type=str, choices=DESCRIPTION_TYPES,
                        default=DESCRIPTION_TYPES[0], help="Type of description to parse")
    return parser.parse_args()


def load_description(name, description_type) -> dict:
    name = name.replace(",", "/")
    with open(path.join(MICROS_ROOT, description_type, name, f"{description_type}.yml"), "r") as file:
        return safe_load(file)


def load_device_definition(name, compatible) -> dict:
    name = name.replace(",", "/")
    compatible = compatible.replace(",", "/")
    with open(path.join(MICROS_ROOT, "device", name, compatible, "device.yml"), "r") as file:
        return safe_load(file)


def parse_device(id: str, definition: dict):
    compatible = definition['compatible']
    device_definition = load_device_definition(definition['type'], compatible)

    compatible_string = compatible.replace(
        ",", "_").upper().replace("-", "_").replace(".", "_")

    description_define = "DESCRIPTION_DEFINE_"+compatible_string+"_"+id.upper()

    return_definition = {}

    for key, value in device_definition.items():
        if value.count(',') < 1:
            raise Exception(
                f"Invalid definition for key '{key}' in system-timer '{id}' definition")
        value = value+",none" if value.count(",") == 1 else value
        required, definition_type, default_value = value.split(",")
        if required == "required" and key not in definition:
            raise Exception(
                f"Missing required key '{key}' in system-timer '{id}' definition")
        if key not in definition:
            if default_value == "none":
                return_definition[key] = None
            elif definition_type == "number":
                return_definition[key] = int(default_value, 0)
            elif definition_type == "hex":
                return_definition[key] = f"0x{int(default_value, 16):x}"
            elif definition_type == "string":
                return_definition[key] = default_value
            elif definition_type == "boolean":
                if default_value.lower() in ["true", "1", "yes"]:
                    return_definition[key] = 1
                else:
                    return_definition[key] = 0
            else:
                raise Exception(
                    f"Unknown definition type '{definition_type}' for key '{key}' in system-timer '{id}' definition")
        else:
            if definition_type == "number":
                if type(definition[key]) != int:
                    return_definition[key] = int(definition[key], 0)
            elif definition_type == "hex":
                if type(definition[key]) != int:
                    return_definition[key] = f"0x{int(definition[key], 16):x}"
                else:
                    return_definition[key] = f"0x{definition[key]:x}"
            elif definition_type == "string":
                return_definition[key] = str(definition[key])
            elif definition_type == "boolean":
                if isinstance(definition[key], bool):
                    return_definition[key] = int(definition[key])
                elif str(definition[key]).lower() in ["true", "1", "yes"]:
                    return_definition[key] = 1
                elif str(definition[key]).lower() in ["false", "0", "no"]:
                    return_definition[key] = 0
                else:
                    raise Exception(
                        f"Invalid boolean value '{definition[key]}' for key '{key}' in system-timer '{id}' definition")
            else:
                raise Exception(
                    f"Unknown definition type '{definition_type}' for key '{key}' in system-timer '{id}' definition")
    return description_define, return_definition


def parse_board(id: str, definition: dict):
    cpu_description = load_description(
        definition['compatible'], 'board')
    parse_description(id,
                      cpu_description, 'board')


def parse_soc(id: str, definition: dict):
    cpu_description = load_description(
        definition['compatible'], 'soc')
    parse_description(id,
                      cpu_description, 'soc')


def parse_arch(id: str, definition: dict):
    cpu_description = load_description(
        definition['compatible'], 'arch')
    parse_description(id,
                      cpu_description, 'arch')


def parse_generic_device(id: str, definition: dict):
    parse_device_define, definition = parse_device(id, definition)
    description_define = parse_device_define

    # check if at least one type and compatible is enabled

    for key in definition:
        DESCRIPTION_DEFINES[description_define +
                            "_" + key.upper()] = definition[key]


DESCRIPTION_PARSERS = {
    "board": parse_board,
    "soc": parse_soc,
    "arch": parse_arch,
    "system-timer": parse_generic_device,
    "interrupt-controller": parse_generic_device,
    "cpu-controller": parse_generic_device,
    "memory-protection": parse_generic_device,
}


def parse_description(id: str, description: dict, type="board"):
    DESCRIPTION_DETAILS[type] = id
    for entry in description:
        for parser_type in DESCRIPTION_PARSERS:
            if parser_type == description[entry]['type']:
                print(
                    f"Parsing {description[entry]['type']} - {entry} {description[entry]['compatible']}")
                DESCRIPTION_PARSERS[parser_type](entry, description[entry])
                continue


if __name__ == "__main__":
    args = parse_args()
    description = load_description(args.name, args.type)
    parse_description(args.name, description, args.type)
    print(DESCRIPTION_DETAILS)

    for define in DESCRIPTION_DEFINES:
        print(define, str(DESCRIPTION_DEFINES[define]))
