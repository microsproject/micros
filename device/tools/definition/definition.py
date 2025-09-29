class DeviceDefinition:
    def parse_definition(self, definition: dict):
        raise NotImplementedError("Subclasses should implement this method")

    def parse(self, definition: dict):
        if 'compatible' in definition:
            self.compatible = definition['compatible']
        else:
            raise ValueError("Definition must have a 'compatible' field")
