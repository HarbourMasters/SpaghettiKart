import sys
import os
import bpy
sys.path.append('tools/blender')
import fast64
from fast64.fast64_internal.mk64 import MK64_Properties
from fast64.fast64_internal.mk64.f3d.properties import MK64CourseDLImportSettings
fast64.register()
dir_path = os.path.dirname(os.path.realpath(__file__))
dir_path = dir_path.replace("\\", "/")
dir_path = os.path.dirname(dir_path)
dir_path = os.path.dirname(dir_path)

bpy.context.scene.gameEditorMode = "MK64"
mk64_settings: MK64_Properties = bpy.context.scene.fast64.mk64
import_settings: MK64CourseDLImportSettings = mk64_settings.course_DL_import_settings
import_settings.name = "d_course_rainbow_road_dl_D8"
import_settings.path = dir_path+"/courses/rainbow_road/course_data.c"
import_settings.base_path = dir_path