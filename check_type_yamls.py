import os
import yaml

def enumerate_yaml_files(directory):
    """Recursively enumerate all YAML files in the given directory."""
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith(".yaml") or filename.endswith(".yml"):
                yield os.path.join(root, filename)

folder_path = "yamls/us"

def is_models(element):
    """Check if the element is a gfx or vtx type."""
    return element.get("type").lower() in ["gfx", "vtx", "mk64:course_vtx", "blob"]

def is_textures(element):
    """Check if the element is a texture type."""
    return element.get("type").lower() == "texture"

def is_other(element):
    """Check if the element is of any other type."""
    return element.get("type").lower() not in ["gfx", "vtx", "texture", "mk64:course_vtx"]

def check_element_type(directory, fun):
    """Check the type of elements in YAML files."""
    for yaml_file in enumerate_yaml_files(directory):
        with open(yaml_file, 'r') as file:
            try:
                data = yaml.safe_load(file)
                assert isinstance(data, dict), "YAML file must contain a dictionary"
                for key, value in data.items():
                    if key == ":config":
                        continue
                    if not fun(value):
                        print(f"Element '{key}' in {yaml_file} does not match the expected type")
            except yaml.YAMLError as e:
                print(f"Error parsing {yaml_file}: {e}")

check_element_type(os.path.join(folder_path, "models"), is_models)
check_element_type(os.path.join(folder_path, "textures"), is_textures)
check_element_type(os.path.join(folder_path, "other"), is_other)