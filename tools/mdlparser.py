#!/usr/bin/env python3
import struct
import sys
import io
from collada import *
import numpy

class ModifiableNumber:
  def __init__(self):
    pass

  def apply(self,stack):
    self.value = stack.pop()
    return self

class Vector:
  def __init__(self,reader):
    self.x, self.y, self.z = struct.unpack( "fff", reader.f.read(4*3) )

class UV:
  def __init__(self,reader):
    self.u, self.v = struct.unpack( "ff", reader.f.read(8) )

class Color:
  def __init__(self,reader):
    self.r, self.g, self.b = struct.unpack( "fff", reader.f.read(4*3) )

class Light:
  def __init__(self,reader):
    self.color = Color(reader)
    self.vector = Vector(reader)
    self.period, self.phase, self.ramp_up, self.hold, self.ramp_down = struct.unpack( "fffff", reader.f.read(4*5) )
    reader.f.read(4) ## byte align

class LightsGeo:
  def __init__(self):
    self.lights = []

  def read(self,reader,stack):
    count = reader.read_dword()
    for i in range(count):
      self.lights.append(Light(reader))
    return self

class Time:
  def __init__(self):
    pass

class Frame:
  def __init__(self,reader):
    self.name = reader.read_string()
    self.position = Vector(reader)
    self.forward = Vector(reader)
    self.up = Vector(reader)

class FrameData:
  def __init__(self):
    self.frames = []

  def read(self,reader,stack):
    count = reader.read_dword()
    for i in range(count):
      self.frames.append(Frame(reader))
    return self

class LODGeo:
  def __init__(self):
    pass

  def apply(self,stack):
    self.rootgeo = stack.pop()
    self.lodgeo = stack.pop()
    return self

class Vertex:
  def __init__(self,reader):
    self.pos = Vector(reader)
    self.tex = UV(reader)
    self.normal = Vector(reader)

class MeshGeoMaker:
  def read(self,reader,stack):
    return MeshGeo(reader)
        
class MeshGeo:
  def __init__(self,reader):
    self.verts = []
    self.indices = []
    numVerts = reader.read_dword()
    numIndices = reader.read_dword()
    for i in range(numVerts):
      self.verts.append(Vertex(reader))
    for i in range(numIndices):
      self.indices.append(reader.read_word())

  def get_lists(self):
    vert = []
    norm = []
    for v in self.verts:
      vert.extend([v.pos.x,v.pos.y,v.pos.z])
      norm.extend([v.normal.x,v.normal.y,v.normal.z])
    return (vert,norm)

  def to_collada(self):
    mesh = Collada()
    effect = material.Effect("effect0",[],"phong",diffuse=(1,1,1), specular=(1,1,1))
    mat = material.Material("material0","mymaterial",effect)
    mesh.effects.append(effect)
    mesh.materials.append(mat)

    vList, nList = self.get_lists()
    vert_src = source.FloatSource("model-verts", numpy.array(vList), ('X','Y','Z'))
    norm_src = source.FloatSource("model-normals", numpy.array(nList), ('X','Y','Z'))
    geo = geometry.Geometry(mesh,"geometry0","model",[vert_src,norm_src])
    input_list = source.InputList()
    input_list.addInput(0,'VERTEX','#model-verts')
    input_list.addInput(1,'NORMAL','#model-normals')
    triset = geo.createTriangleSet(numpy.array(self.indices),input_list,"materialref")
    geo.primitives.append(triset)
    mesh.geometries.append(geo)

    matnode = scene.MaterialNode("materialref",mat,inputs=[])
    geonode = scene.GeometryNode(geo,[matnode])
    node = scene.Node("node0",children=[geonode])
    myscene = scene.Scene("myscene",[node])
    mesh.scenes.append(myscene)
    mesh.scene = myscene

    return mesh

  def to_obj(self,path):
    with open(path,"w") as f:
      print("Num verts %i, Num Faces %i" % (len(self.verts),len(self.indices)/3))
      for v in self.verts:
        f.write( "v {0} {1} {2}\n".format(v.pos.x,v.pos.y,v.pos.z) )
        f.write( "vt {0} {1}\n".format(v.tex.u,v.tex.v) )
        f.write( "vn {0} {1} {2}\n".format(v.normal.x,v.normal.y,v.normal.z) )

      faces = zip( self.indices[0::3], self.indices[1::3], self.indices[2::3] )
      for face in faces:
        face = list(face)
        face[0] += 1
        face[1] += 1
        face[2] += 1
        f.write( "f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}\n".format(face[0],face[1],face[2]) )

class Point:
  def __init__(self,reader):
    self.x, self.y = struct.unpack("ii",reader.f.read(8))

class BinarySurfaceInfo:
  def __init__(self,reader):
    self.size = Point(reader)
    self.pitch, self.bitCount, self.redMask, self.greenMask, self.blueMask, self.alphaMask, self.colorKeyed = struct.unpack( "hIIIII?", reader.f.read(25) )

class ImportImage:
  def __init__(self):
    pass

  def read(self,reader,stack):
    self.info = BinarySurfaceInfo(reader)
    self.pixdata = reader.f.read(self.info.pitch * self.info.size.y)
    return self

class TextureGeo:
  def __init__(self):
    pass

  def apply(self,stack):
    self.geo = stack.pop()
    self.image = stack.pop()
    return self

class NamespaceManager:
  objects = {}

  @staticmethod
  def find( namespace, libname ):
    return NamespaceManager.objects[namespace][libname]

  @staticmethod
  def add( namespace, libname, obj ):
    if namespace not in NamespaceManager.objects:
      NamespaceManager.objects[namespace] = {}
    NamespaceManager.objects[namespace][libname] = obj

class MDLFile:
  MAGIC = 0xdebadf00
  VERSION = 0x00010000
  OBJ_END = 0
  OBJ_FLOAT = 1
  OBJ_STRING = 2
  OBJ_TRUE = 3
  OBJ_FALSE = 4
  OBJ_LIST = 5
  OBJ_APPLY = 6
  OBJ_BINARY = 7
  OBJ_REF = 8
  OBJ_IMPORT = 9
  OBJ_PAIR = 10

  def __init__(self,path):
    with io.open(path,"rb") as self.f:
      magic, version, numNameSpaces, numImports, numExports, numDefinitions = struct.unpack("IIIIII",self.f.read(4*6))
      if magic != MDLFile.MAGIC:
        raise Exception("Not an MDL File")

      if version != MDLFile.VERSION:
        raise Exception("Wrong MDL Version")

      self.nsImports = {}
      for i in range(numNameSpaces):
        self.nsImports[i] = self.read_string()

      self.objImports = {}
      for i in range(numImports):
        nsLibIndex = self.read_dword()
        nsLibName = self.read_string()
        self.objImports[i] = NamespaceManager.find(self.nsImports[nsLibIndex],nsLibName)

      numObjects = numExports + numDefinitions
      self.exportNames = []
      for i in range(numExports):
        self.exportNames.append(self.read_string())
     
      self.objects = {}
      for i in range(numObjects):
        objIndex = self.read_dword() 
        obj = self.read_object()
        self.objects[self.exportNames[objIndex]] = obj
          
  def read_object(self):
    stack = []
    while True:
      token = self.read_dword()
      if token == MDLFile.OBJ_FLOAT: stack.append( self.read_float() )
      elif token == MDLFile.OBJ_STRING: stack.append( self.read_string() )
      elif token == MDLFile.OBJ_TRUE: stack.append( True )
      elif token == MDLFile.OBJ_FALSE: stack.append( False )
      elif token == MDLFile.OBJ_LIST: stack.append( self.read_list() )
      elif token == MDLFile.OBJ_APPLY: stack.append( self.read_apply(stack) )
      elif token == MDLFile.OBJ_BINARY: stack.append( self.read_binary(stack) )
      elif token == MDLFile.OBJ_REF: stack.append( self.objects[self.read_dword()] )
      elif token == MDLFile.OBJ_IMPORT: stack.append( self.objImports[self.read_dword()] )
      elif token == MDLFile.OBJ_PAIR: stack.append( self.read_pair(stack) )
      elif token == MDLFile.OBJ_END:
        if len(stack) > 0:
          return stack.pop()
        else:
          return None
      else: raise Exception("Read Object, bad token %i" % token)

  def read_list(self):
    count = self.read_dword()
    lData = []
    for i in range(count):
      lData.append(self.read_object())
    return lData

  def read_pair(self,stack):
    return (stack.pop(), stack.pop())

  def read_float(self):
    return struct.unpack("f",self.f.read(4))[0]

  def read_dword(self):
    return struct.unpack("I",self.f.read(4))[0]

  def read_word(self):
    return struct.unpack("H",self.f.read(2))[0]

  def read_apply(self,stack):
    obj = stack.pop()
    return obj.apply(stack)
  
  def read_string(self):
    s = bytearray()
    c = self.f.read(1)
    while ord(c) != 0:
      s.append(ord(c))
      c = self.f.read(1)
    l = len(s)
    skip = 3-(l%4)
    self.f.read(skip)
    return s.decode("utf-8")

  def read_binary(self,stack):
    obj = stack.pop()
    return obj.read(self,stack)

  def get_geo(self):
    return self.objects['object'].rootgeo.geo

  def export_meshes(self,path):
    self.get_geo().to_obj(path)
#    mesh = geo.to_collada()
#    mesh.write(path)

NamespaceManager.add( "model", "ModifiableNumber", ModifiableNumber() )
NamespaceManager.add( "effect", "LightsGeo", LightsGeo() )
NamespaceManager.add( "model", "time", Time() )
NamespaceManager.add( "effect", "FrameData", FrameData() )
NamespaceManager.add( "model", "LODGeo", LODGeo() )
NamespaceManager.add( "model", "MeshGeo", MeshGeoMaker() )
NamespaceManager.add( "model", "TextureGeo", TextureGeo() )
NamespaceManager.add( "model", "ImportImage", ImportImage() )
NamespaceManager.add( "int01bmp", "int01bmp", MDLFile("../Artwork/int01bmp.mdl").objects["int01bmp"] )
mdlfile = MDLFile("../Artwork/int01.mdl")
mdlfile.export_meshes("./int01.obj")
