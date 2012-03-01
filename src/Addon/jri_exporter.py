bl_info = {
    "name": "JRI Exporter",
    "description": "Export static mesh to JustRenderIt.",
    "author": "Ran Jiao",
    "version": (0, 1),
    "blender": (2, 53, 0),
    "location": "File > Export > JustRenderIt (.jsm)",
    "warning": "", # used for warning icon and text in addons panel
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"
}

import os
from math import radians

import bpy
from mathutils import *
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty

import os, bpy

def GetMeshVertexCount(Mesh):
    VertexCount = 0
    for Face in Mesh.faces:
        VertexCount += len(Face.vertices)
    return VertexCount

def WriteMeshVertices(Mesh, Config):
    Index = 0
    File = Config.file
    Whitespace = Config.whitespace
    
    VertexCount = GetMeshVertexCount(Mesh)
    File.write("{}vertex count:{};\n".format(" " * Whitespace, VertexCount))

    for Face in Mesh.faces:
        Vertices = list(Face.vertices)

        for Vertex in [Mesh.vertices[Vertex] for Vertex in Vertices]:
            Position = Vertex.co
            File.write("{}{:9f};{:9f};{:9f};".format(" " * Whitespace, Position[0], Position[1], Position[2]))
            Index += 1
            if Index == VertexCount:
                File.write(";\n")
            else:
                File.write(",\n")

    Index = 0
    File.write("{}{};\n".format(" " * Whitespace, len(Mesh.faces)))

    for Face in Mesh.faces:
        File.write("{}{};".format(" " * Whitespace, len(Face.vertices)))
        for Vertex in Face.vertices:
            File.write("{};".format(Index))
            Index += 1
        if Index == VertexCount:
            File.write(";\n")
        else:
            File.write(",\n")
        
class JRIExporterConfig:
    "Exoirt configuration"
    def __init__(self, filepath, file):
        self.filepath = filepath
        self.file = file
        self.whitespace = 2
        
def GenerateTestConfig:
    filepath = "d:\proj\media\test.jsm";
    file = open(filepath, 'w')
    return JRIExporterConfig(filepath, file)
    
class JRIExporter(bpy.types.Operator, ExportHelper):
    ''' Export to JustRenderIt mesh format'''
    
    bl_idname = 'export.jri'
    bl_label = 'Export JustRenderIt'
    filename_ext = '.jsm'

    filter_glob = StringProperty(
            default="*.jsm",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting = BoolProperty(
            name="Example Boolean",
            description="Example Tooltip",
            default=True,
            )

    type = EnumProperty(
            name="Example Enum",
            description="Choose between two items",
            items=(('OPT_A', "First Option", "Description one"),
                   ('OPT_B', "Second Option", "Description two")),
            default='OPT_A',
            )

    @classmethod
    def poll(cls, context):
        return context.active_object is not None
        
    def export_mesh(self, config):
        WriteMeshVertices(config)
        
    def execute(self, context):
        print("executing")
        
        # get a new file name
        basedir = os.path.dirname(bpy.data.filepath)
        if not basedir:
            raise Exception("Blend file is not saved")
        
        filepath = bpy.path.ensure_ext(self.filepath, ".jsm")
        file = open(filepath, 'w')
        
        config = JRIExporterConfig(
            context,
            filepath,
            file,
        )
        
        self.export_mesh(config);
        return {"FINISHED"}
        
    # def invoke(self, context, event):
        # print("invoking")
        # self.export_mesh()
        # return {"RUNNING_MODAL"}

def menu_func(self, context):
    self.layout.operator(JRIExporter.bl_idname, text="JustRenderIt Static Mesh (.jsm)")
    
def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func)


if __name__ == "__main__":
    register()
    pass