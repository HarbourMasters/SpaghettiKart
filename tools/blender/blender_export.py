import bpy
import os
import sys


sys.path.append('tools/blender')
import fast64
from fast64.fast64_internal.f3d.f3d_gbi import get_F3D_GBI
from fast64.fast64_internal.f3d.f3d_material import createF3DMat
from fast64.fast64_internal.f3d.f3d_parser import F3DContext, getImportData, importMeshC
from fast64.fast64_internal.mk64 import MK64_Properties
from fast64.fast64_internal.mk64.f3d.operators import MK64_ImportCourseDL
from fast64.fast64_internal.mk64.f3d.properties import MK64CourseDLImportSettings

from clean_scene import clean_scene

dir_path = os.path.dirname(os.path.realpath(__file__))
dir_path = dir_path.replace("\\", "/")
dir_path = os.path.dirname(dir_path)
dir_path = os.path.dirname(dir_path)

print(dir_path)

fast64.register()

class FakeOperator:
    def __init__(self):
        self.report = lambda x, y: print(y)

argv = sys.argv
argv = argv[argv.index("--") + 1:]  # get all args after "--"

bpy.context.scene.gameEditorMode = "MK64"
mk64_settings: MK64_Properties = bpy.context.scene.fast64.mk64
course_DL_import_settings: MK64CourseDLImportSettings = mk64_settings.course_DL_import_settings
base_path = bpy.path.abspath(dir_path)

file = argv[0]
obj = argv[1]
out_file = argv[2]

course_DL_import_settings.name = obj
course_DL_import_settings.path = file
course_DL_import_settings.base_path = base_path

clean_scene()
MK64_ImportCourseDL.execute(FakeOperator(),bpy.context)
bpy.ops.wm.save_as_mainfile(filepath=out_file)