# type - where to look at
# compatible - which description to use
# name - optional, for printing
# key - id

from argparse import ArgumentParser
from os import path

from yaml import safe_load
import os

DESCRIPTION_TYPES = ["board", "soc", "arch"]

DESCRIPTION_DETAILS = {}

DESCRIPTION = {}

DESCRIPTION_DEFINES = {}

COMPATIBLE_PATHS = set()

CMAKE_DEFINITIONS = {}


MICROS_ROOT = path.normpath(path.join(
    path.dirname(path.abspath(__file__)), "..", ".."))


def parse_args():
    parser = ArgumentParser(description="Parse micros device descriptions")
    parser.add_argument("output_path", type=str, default=None,
                        help="Output path to write the define files to")
    parser.add_argument("name", type=str,
                        help="Description name, e.g. lm,lm3s6965evb")
    parser.add_argument("-t", "--type", type=str, choices=DESCRIPTION_TYPES,
                        default=DESCRIPTION_TYPES[0], help="Type of description to parse")
    parser.add_argument("--micros-root", type=str, default=MICROS_ROOT,
                        help="Path to micros root directory")
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
    print(
        f"Parsing device {id} {definition['type']} {definition['compatible']}")
    compatible = definition['compatible']
    device_definition = load_device_definition(definition['type'], compatible)

    type_string = definition['type'].replace(
        ",", "_").upper().replace("-", "_").replace(".", "_")

    description_define = "MICROS_DEVICE_DESCRIPTION_"+type_string+"_"+id.upper()

    return_definition = {'compatible':  compatible, 'type': definition['type']}

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
            elif definition_type == "string" or definition_type == "label":
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
            elif definition_type == "string" or definition_type == "label":
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
    if return_definition['enabled'] not in [1, True]:
        return None, None
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

    if parse_device_define is None:
        return

    COMPATIBLE_PATHS.add('device/' +
                         definition['type'] + "/" + definition['compatible'].replace(",", "/"))

    for key in definition:
        DESCRIPTION_DEFINES[parse_device_define +
                            "_" + key.upper()] = definition[key]


DESCRIPTION_PARSERS = {
    "board": parse_board,
    "soc": parse_soc,
    "arch": parse_arch,
}


def parse_description(id: str, description: dict, type="board"):
    for entry in description:
        was_parsed = False
        for parser_type in DESCRIPTION_PARSERS:
            if parser_type == description[entry]['type']:
                print(
                    f"Parsing {description[entry]['type']} - {entry} {description[entry]['compatible']}")
                if description[entry]['type'] == 'arch':
                    if 'arch' in DESCRIPTION_DETAILS:
                        raise Exception(
                            f"Multiple arch definitions found: {entry} and {DESCRIPTION_DETAILS['arch']}")
                    DESCRIPTION_DETAILS['arch'] = description[entry]
                if description[entry]['type'] == 'soc':
                    if 'soc' in DESCRIPTION_DETAILS:
                        raise Exception(
                            f"Multiple soc definitions found: {entry} and {DESCRIPTION_DETAILS['soc']}")
                    DESCRIPTION_DETAILS['soc'] = description[entry]
                DESCRIPTION_PARSERS[parser_type](entry, description[entry])
                was_parsed = True
                continue
        if not was_parsed:
            parse_generic_device(entry, description[entry])


def generate_cmake_file(output_path: str):
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    with open(output_path, "w") as file:
        file.write("# This file is auto-generated. Do not edit.\n\n")
        file.write("message(STATUS \"MicrOS device configuration\")\n\n")
        for key, value in CMAKE_DEFINITIONS.items():
            file.write(f'set({key} "{value}")\n')
        file.write("\n")
        for define in DESCRIPTION_DEFINES:
            value = DESCRIPTION_DEFINES[define]
            if isinstance(value, str):
                value = f'"{value}"'
            file.write(f"set({define} {value})\n")


if __name__ == "__main__":
    args = parse_args()
    description = load_description(args.name, args.type)
    parse_description(args.name, description, args.type)
    print(DESCRIPTION_DETAILS)

    for define in DESCRIPTION_DEFINES:
        print(define, str(DESCRIPTION_DEFINES[define]))

    print("\n/* Compatible paths */")
    for path in COMPATIBLE_PATHS:
        print(f"  {path}")

    architecture = DESCRIPTION_DETAILS["arch"]["compatible"] if "arch" in DESCRIPTION_DETAILS else "unknown"
    CMAKE_DEFINITIONS["MICROS_ARCH"] = architecture.replace(",", "/")
    soc = DESCRIPTION_DETAILS["soc"]["compatible"] if "soc" in DESCRIPTION_DETAILS else "unknown"
    CMAKE_DEFINITIONS["MICROS_SOC"] = soc.replace(",", "/")
    CMAKE_DEFINITIONS["MICROS_BOARD"] = args.name
    generate_cmake_file(os.path.join(
        args.output_path, "micros_generated.cmake"))
    print(
        f"\nCMake definitions written to {os.path.join(args.output_path, 'micros_generated.cmake')}")
