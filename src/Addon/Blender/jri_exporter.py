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

class JRIWriterBase:
    def GetMeshVertexCount(self, Mesh):
        VertexCount = 0
        for Face in Mesh.faces:
            VertexCount += len(Face.vertices)
        return VertexCount

class JRITextWriter(JRIWriterBase):
    def WriteMeshVertices(self, Mesh, Config):
        Index = 0
        File = Config.file
        Whitespace = Config.whitespace

        VertexCount = self.GetMeshVertexCount(Mesh)
        File.write("{}vertex count:{};\n".format(" " * Whitespace, VertexCount))

        for Face in Mesh.faces:
            Vertices = list(Face.vertices)

            for Vertex in [Mesh.vertices[Vertex] for Vertex in Vertices]:
                Position = Vertex.co
                File.write("{}{:9f};{:9f};{:9f};".format(
                        " " * Whitespace, Position[0],
                        Position[1], Position[2]))
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
    "Export configuration"
    def __init__(self, filepath, file):
        self.filepath = filepath
        self.file = file
        self.whitespace = 2

class JRIExporter(bpy.types.Operator, ExportHelper):
    ''' Export to JustRenderIt mesh format'''

    bl_idname = 'export.jri'
    bl_label = 'Export JustRenderIt'
    filename_ext = '.jsm'

    filter_glob = StringProperty(
            default="*.jsm",
            options={'HIDDEN'},
            )

    # export selected mesh only?
    export_selected = BoolProperty(
            name="Export selected",
            description="Export selected mesh only, or export everything",
            default=True,
            )

    # export to text or binary?
    export_format = EnumProperty(
            name="Format",
            description="Choose export format",
            items=(('TEXT', "Text format", "Export mesh to text format"),
                   ('BIN', "Binary format", "Export mesh to binary format")),
            default='TEXT',
            )

    @classmethod
    def poll(cls, context):
        return context.active_object is not None

    def execute(self, context):
        print("executing")

        file = open(self.filepath, 'w')

        config = JRIExporterConfig(
            self.filepath,
            file)

        print("Exporting " + self.export_format);
        if self.export_format == "BIN":
            print("Binary exportor not implemented")
            return {"FINISHED"}
        else:
            writer = JRITextWriter()

        if self.export_selected:
            objs = bpy.context.selected_objects
        else:
            objs = bpy.context.scene.objects

        for obj in objs:
            if obj.type == "MESH":
                try:
                    mesh = obj.to_mesh(context.scene,
                                       True, 'PREVIEW')
                except RuntimeError:
                    mesh = None

                if mesh is None:
                    continue

                writer.WriteMeshVertices(mesh, config)

        return {"FINISHED"}

def menu_func(self, context):
    self.layout.operator(JRIExporter.bl_idname,
                         text="JustRenderIt Static Mesh (.jsm)")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)


def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    unregister()
    register()
