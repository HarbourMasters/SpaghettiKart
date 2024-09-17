import json
import subprocess
import threading
import os
import argparse

parser = argparse.ArgumentParser(description="Extract 3D models from .blend files")
parser.add_argument("blender", type=str, help="Path to blender executable")
parser.add_argument("json_file", type=str, help="Path to json file with models")
parser.add_argument("-t", "--threads", type=int, default=1, help="Number of threads to use")
args = parser.parse_args()

if not os.path.exists("models"):
    os.makedirs("models")

models:list[dict[str, str|list[str]]] = json.load(open(args.json_file))

blender = args.blender

args_blender = [
    "--background",
    "--python",
    "tools/blender/blender_export.py",
    "--",
]

failed = []

for model in models:
    file = model["file"]
    out_dir = "models/"+model["out_dir"] if "out_dir" in model else "models"
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    for obj in model["models"]:
        out_file = out_dir+"/"+obj+".blend"
        if os.path.exists(out_file):
            continue
        args_blender.append(file)
        args_blender.append(obj)
        args_blender.append(out_file)
        result = subprocess.run([blender]+args_blender)
        if result.returncode != 0:
            failed.append(out_file)
        args_blender.pop()
        args_blender.pop()
        args_blender.pop()

if len(failed) == 0:
    print("All models exported successfully")
else:
    print("Failed to export:")
    print("\n".join(failed))