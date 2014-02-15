#!/usr/bin/env python3
import struct
import sys
import io
import jsonpickle
import json
from PIL import Image

class Ptr(object):
  pass

class UniquePtr(Ptr):
  def __init__(self,obj):
    self.ptr_wrapper = { "data": obj, "valid" : 1 }
    self.__dict__.update( TypeTracker.add_type(obj.__class__.__name__) )

class SharedPtr(Ptr):
  instances = {}
  index = 0

  def __init__(self,obj):
    self.ptr_wrapper = { "data": obj, "id": SharedPtr.getInstance(obj) }
    self.__dict__.update( TypeTracker.add_type(obj.__class__.__name__) )

  def getInstance(obj):
    if obj in SharedPtr.instances:
      return SharedPtr.instances[obj]
    else:
      SharedPtr.index += 1
      SharedPtr.instances[obj] = SharedPtr.index
      return SharedPtr.instances[obj] | 0x80000000
       
class TypeTracker(object):
  types = {}
  index = 0

  def add_type(className):
    if className not in TypeTracker.types:
      TypeTracker.index += 1
      TypeTracker.types[className] = TypeTracker.index

    return { "polymorphic_name" : className, "polymorphic_id" : TypeTracker.types[className] | 0x80000000 }
       
class Geo(object):
  pass

class ModifiableNumberFactory:
  def apply(self,stack):
    return stack.pop()

class Vector3:
  def __init__(self,reader):
    self.x, self.y, self.z = struct.unpack( "fff", reader.f.read(4*3) )

class UV:
  def __init__(self,reader):
    self.u, self.v = struct.unpack( "ff", reader.f.read(8) )
    self.v = 1.0 - self.v

class Color:
  def __init__(self,reader):
    self.r, self.g, self.b = struct.unpack( "fff", reader.f.read(4*3) )
    self.a = 1.0

class Light:
  def __init__(self,reader):
    self.color = Color(reader)
    self.position = Vector3(reader)
    self.period, self.phase, self.ramp_up, self.hold, self.ramp_down = struct.unpack( "fffff", reader.f.read(4*5) )
    reader.f.read(4) ## byte align

class LightsGeoFactory:
  def read(self,reader,stack):
    l = []
    count = reader.read_dword()
    for i in range(count):
      l.append(Light(reader))
    return l 

class Time:
  def __init__(self):
    pass

class TimeFactory:
  pass

class Frame:
  def __init__(self,reader):
    self.name = reader.read_string()
    self.position = Vector3(reader)
    self.forward = Vector3(reader)

    self.up = Vector3(reader)

class FrameDataFactory:
  def read(self,reader,stack):
    count = reader.read_dword()
    f = [] 
    for i in range(count):
      f.append(Frame(reader))
    return f

class GroupGeo(Geo):
  def __init__(self,stack):
    l = stack.pop()
    l.reverse()
    self.geo_list = []
    for g in l:
      self.geo_list.append(UniquePtr(g))

class GroupGeoFactory:
  def apply(self,stack):
    return GroupGeo(stack)

class LODGeo(Geo):
  def __init__(self,stack):
    self.rootGeo = UniquePtr(stack.pop())
    self.lodGeo = stack.pop()
    for l in self.lodGeo:
      l["value"] = SharedPtr(l["value"])

class LODGeoFactory:
  def apply(self,stack):
    return LODGeo(stack)

class Vertex:
  def __init__(self,reader):
    self.pos = Vector3(reader)
    self.tex = UV(reader)
    self.normal = Vector3(reader)

class MeshGeoFactory:
  def read(self,reader,stack):
    return MeshGeo(reader)
        
class MeshGeo(Geo):
  write_path = ""
  base_name = ""
  mesh_num = 0

  def __init__(self,reader):
    print( "Creating MeshGeo object" )

    self.resourcePath = "{0}_{1}.obj".format(reader.namespace,MeshGeo.mesh_num)
    MeshGeo.mesh_num += 1
    
    verts = []
    indices = []
    numVerts = reader.read_dword()
    numIndices = reader.read_dword()
    for i in range(numVerts):
      verts.append(Vertex(reader))
    for i in range(numIndices):
      indices.append(reader.read_word())

    self.to_obj("{0}/{1}".format(MDLFile.write_path,self.resourcePath),verts,indices)

  def get_lists(self):
    vert = []
    norm = []
    for v in self.verts:
      vert.extend([v.pos.x,v.pos.y,v.pos.z])
      norm.extend([v.normal.x,v.normal.y,v.normal.z])
    return (vert,norm)

  def to_obj(self,path,verts,indices):
    with open(path,"w") as f:
      print("Num verts %i, Num Faces %i to %s" % (len(verts),len(indices)/3,path))
      for v in verts:
        f.write( "v {0} {1} {2}\n".format(v.pos.x,v.pos.y,v.pos.z) )
        f.write( "vt {0} {1}\n".format(v.tex.u,v.tex.v) )
        f.write( "vn {0} {1} {2}\n".format(v.normal.x,v.normal.y,v.normal.z) )

      faces = zip( indices[0::3], indices[1::3], indices[2::3] )
      for face in faces:
        face = list(face)
        face[0] += 1
        face[1] += 1
        face[2] += 1
        f.write( "f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}\n".format(face[0],face[1],face[2]) )

class Point:
  def __init__(self,reader):
    self.x, self.y = struct.unpack("ii",reader.f.read(8))

def to_range(c,mask):
  v = c & mask
  if mask == 0: return 255

  while( mask & 1 == 0 ):
    v >>= 1
    mask >>= 1
  return int(float(v)/float(mask)*255)

class BinarySurfaceInfo:
  def __init__(self,reader):
    self.size = Point(reader)
    self.pitch, self.bitCount, self.redMask, self.greenMask, self.blueMask, self.alphaMask, self.colorKeyed = struct.unpack( "hIIIII?", reader.f.read(25) )

  def get_rgb(self, c):
    r = to_range(c,self.redMask)
    g = to_range(c,self.greenMask)
    b = to_range(c,self.blueMask)
    a = to_range(c,self.alphaMask)
    return (r,g,b,a)

class ImportImage:
  def __init__(self,reader):
    self.namespace = reader.namespace
    self.name = reader.namespace + ".png"
    self.info = BinarySurfaceInfo(reader)
    self.pixdata = reader.f.read(self.info.pitch * self.info.size.y)
    reader.align_bytes()
    self.write_png( "{0}/{1}".format(MDLFile.write_path,self.name) )

  def __getstate__(self):
    return { 'resourcePath' : self.name }

  def __setstate__(self,state):
    self.__dict__.update(state)

  def write_png(self,path):
    img = Image.new("RGBA",(self.info.size.x,self.info.size.y))
    
    ptups = zip( self.pixdata[0::2], self.pixdata[1::2] )
    colors = []
    for p in ptups:
      c = p[0] << 8 | p[1]
      colors.append(self.info.get_rgb(c))
    img.putdata(colors)
    img.save(path)

class ImportImageFactory:
  def read(self,reader,stack):
    return ImportImage(reader)

class TextureGeo(Geo):
  def __init__(self,stack):
    self.geo = UniquePtr(stack.pop())
    self.image = stack.pop()

class TextureGeoFactory:
  def apply(self,stack):
    return TextureGeo(stack);

class NamespaceManager:
  objects = {}

  @staticmethod
  def find( namespace, libname ):
    try:
      return NamespaceManager.objects[namespace][libname]
    except KeyError:
      print( "Couldn't find", namespace, libname )
      print( "Attempting to add {0}.mdl".format(libname) )
      NamespaceManager.add( namespace, libname, MDLFile("../Artwork/{0}.mdl".format(libname),namespace).objects[libname])
      return NamespaceManager.objects[namespace][libname]

  @staticmethod
  def add( namespace, libname, obj ):
    if namespace not in NamespaceManager.objects:
      NamespaceManager.objects[namespace] = {}
    NamespaceManager.objects[namespace][libname] = obj

class MDLFile:
  write_path = ""
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

  def __init__(self,path,namespace):
    self.namespace = namespace
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
      else: raise Exception("Read Object, bad token {0} at position {1}({2})".format(token,self.f.tell(),hex(self.f.tell())))

  def read_list(self):
    count = self.read_dword()
    lData = []
    for i in range(count):
      lData.append(self.read_object())
    return lData

  def read_pair(self,stack):
    return {"key" : stack.pop(), "value" : stack.pop()}

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
    self.align_bytes()
    return s.decode("utf-8")

  def read_binary(self,stack):
    obj = stack.pop()
    return obj.read(self,stack)

  def get_img(self):
    return self.objects['object'].get_img()

  def get_geo(self):
    return self.objects['object'].get_geo()

  def export_meshes(self,path):
    self.get_geo().to_obj(path)

  def align_bytes(self):
    skip = 3-((self.f.tell()-1)%4) 
    self.f.read(skip)

  def write_json(self):
    with open( "/".join([MDLFile.write_path,self.namespace])+".json", "w" ) as f:
      jsonpickle.set_encoder_options('json', indent=4, sort_keys=True)
      json_str = jsonpickle.encode(self,False)
      f.write( json_str )

  def __getstate__(self):
    self.objects['object'] = UniquePtr(self.objects['object'])
    return { 'value0' : self.objects }

  def __setstate__(self,state):
    self.__dict__.update(state)

if len(sys.argv) < 2:
  print( "Provide filename root" )
  exit(1)

rootName = sys.argv[1]
mdlName = rootName+".mdl"

NamespaceManager.add( "model", "ModifiableNumber", ModifiableNumberFactory() )
NamespaceManager.add( "effect", "LightsGeo", LightsGeoFactory() )
NamespaceManager.add( "model", "time", TimeFactory() )
NamespaceManager.add( "effect", "FrameData", FrameDataFactory() )
NamespaceManager.add( "model", "LODGeo", LODGeoFactory() )
NamespaceManager.add( "model", "MeshGeo", MeshGeoFactory() )
NamespaceManager.add( "model", "TextureGeo", TextureGeoFactory() )
NamespaceManager.add( "model", "ImportImage", ImportImageFactory() )
NamespaceManager.add( "model", "GroupGeo", GroupGeoFactory() )
MDLFile.write_path = "./decompiled/"
mdlfile = MDLFile("../Artwork/"+mdlName,rootName)
mdlfile.write_json()
