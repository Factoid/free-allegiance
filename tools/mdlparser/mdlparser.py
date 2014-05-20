#!/usr/bin/env python3
import struct
import sys
import io
import jsonpickle
import json
import os
import PIL.Image
import PIL.ImageDraw
import argparse

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
  def __init__(self,x,y,z):
    self.x = x
    self.y = y
    self.z = z

  @classmethod
  def fromReader(cls,reader):
    x, y, z = struct.unpack( "fff", reader.f.read(4*3) )
    return cls(x,y,z)

class UV:
  def __init__(self,reader):
    self.u, self.v = struct.unpack( "ff", reader.f.read(8) )
    self.v = 1.0 - self.v

class Color:
  def __init__(self,reader):
    self.r, self.g, self.b, self.a = struct.unpack( "ffff", reader.f.read(4*4) )

class Light:
  def __init__(self,reader):
    self.color = Color(reader)
    p = Vector3.fromReader(reader)
    self.position = p
    self.period, self.phase, self.ramp_up, self.hold, self.ramp_down = struct.unpack( "fffff", reader.f.read(4*5) )

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
    self.position = Vector3.fromReader(reader)
    self.forward = Vector3.fromReader(reader)

    self.up = Vector3.fromReader(reader)

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
#LOD Order seems to be reversed
    for i in range(int(len(self.lodGeo)/2)):
      tmp = self.lodGeo[i]["value"]
      self.lodGeo[i]["value"] = self.lodGeo[-(i+1)]["value"]
      self.lodGeo[-(i+1)]["value"] = tmp

    for l in self.lodGeo:
      l["value"] = SharedPtr(l["value"])

class LODGeoFactory:
  def apply(self,stack):
    return LODGeo(stack)

class Vertex:
  def __init__(self,reader):
    self.pos = Vector3.fromReader(reader)
    self.tex = UV(reader)
    self.normal = Vector3.fromReader(reader)

class MeshGeoFactory:
  def read(self,reader,stack):
    return MeshGeo(reader)

def to_uv(s,v,f):
  return (v[f].tex.u*s,(1.0-v[f].tex.v)*s)

class MeshGeo(Geo):
  UVimage = False
  write_path = ""
  base_name = ""
  mesh_num = 0

  def __init__(self,reader):
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

    if MeshGeo.UVimage:
      self.make_uv_texture(MDLFile.write_path+"/"+reader.namespace,verts,indices,"{1}/{0}_uv.png".format(reader.namespace,MDLFile.write_path))

    self.to_obj("{0}/{1}".format(MDLFile.write_path,self.resourcePath),verts,indices)

  def get_lists(self):
    vert = []
    norm = []
    for v in self.verts:
      vert.extend([v.pos.x,v.pos.y,v.pos.z])
      norm.extend([v.normal.x,v.normal.y,v.normal.z])
    return (vert,norm)

  def make_uv_texture(self,namespace,verts,indices,path):
    print( "Making UV image", path )
    size = 1024
    #img = PIL.Image.new("RGB",(size,size),(255,255,255))
    img = PIL.Image.open(namespace+"bmp.png").resize((size,size),PIL.Image.BICUBIC)
    draw = PIL.ImageDraw.Draw(img)

    faces = zip( indices[0::3], indices[1::3], indices[2::3] )
    for face in faces:
      face = list(face)
      draw.line([to_uv(size,verts,face[0]),to_uv(size,verts,face[1]),to_uv(size,verts,face[1]),to_uv(size,verts,face[2]),to_uv(size,verts,face[2]),to_uv(size,verts,face[0])],(255,0,0),1)

    img.save(path)
    
  def to_obj(self,path,verts,indices):
    with open(path,"w") as f:
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
  def __str__(self):
    return "x: " + str(self.x) + " y: " + str(self.y)

  def __init__(self):
    pass

  def clone(self):
    copy = Point()
    copy.x = self.x
    copy.y = self.y
    return copy
    
  def fromReader(self,reader):
    self.x, self.y = struct.unpack("ii",reader.f.read(8))
    return self

def to_range(c,mask):
  v = c & mask
  if mask == 0: return 255

  while( mask & 1 == 0 ):
    v >>= 1
    mask >>= 1
  return int(float(v)/float(mask)*255)

class BinarySurfaceInfo:
  def __str__(self):
    return "Surface: " + str(self.size) + " pitch : " + str(self.pitch) + " bit count : " + str(self.bitCount)

  def __init__(self):
    pass

  def clone(self):
    copy = BinarySurfaceInfo()
    copy.size = self.size.clone()
    copy.pitch = self.pitch
    copy.bitCount = self.bitCount
    copy.redMask = self.redMask
    copy.greenMask = self.greenMask
    copy.blueMask = self.blueMask
    copy.alphaMask = self.alphaMask
    copy.colorKeyed = self.colorKeyed
    return copy
    
  def fromReader(self,reader):
    self.size = Point().fromReader(reader)
    self.pitch, self.bitCount, self.redMask, self.greenMask, self.blueMask, self.alphaMask, self.colorKeyed = struct.unpack( "hIIIII?", reader.f.read(25) )
    return self

  def get_rgb(self, c):
    r = to_range(c,self.redMask)
    g = to_range(c,self.greenMask)
    b = to_range(c,self.blueMask)
    a = to_range(c,self.alphaMask)
    return (r,g,b,a)

class FrameImage:
  def __init__(self,frame,bgImage,reader):
    self.bgImage = bgImage
    self.nFrame = reader.read_dword()
    offsets = []
    for i in range(self.nFrame):
      offsets.append(reader.read_dword())
    memBuf = reader.f.read(offsets[-1]) 
    clone = self.bgImage.clone()
    for i in range(self.nFrame-1):
      clone.name = clone.namespace + "_" + str(i+1) + ".png"
      clone.extractRLEData(memBuf[offsets[i]:offsets[i+1]])
      clone.save_png()

class FrameImageFactory:
  def read(self,reader,stack):
    frame = stack.pop()
    img = stack.pop()
    return FrameImage(frame,img,reader)

class Image:
  def __init__(self):
    pass
  
  def clone(self):
    copy = Image()
    copy.namespace = self.namespace
    copy.name = self.name
    copy.info = self.info.clone()
    copy.rawdata = self.rawdata[:]
    return copy

  def extractRLEData(self,rleBuffer):
    RLELengthMask = 0x3fff
    RLEMask = 0xc000
    RLEMaskFill = 0x0000
    RLEMaskByte = 0x4000
    RLEMaskWord = 0x8000
    RLEMaskDWord = 0xc000

    rPtr = 0
    wPtr = 0
    outBuf = bytearray() 
    while rPtr < len(rleBuffer):
      word = rleBuffer[rPtr] | rleBuffer[rPtr+1] << 8
      length = word & RLELengthMask
      cmd = word & RLEMask
      rPtr += 2
      if cmd == RLEMaskFill:
        for i in range(length):
          outBuf.append(self.rawdata[wPtr] ^ rleBuffer[rPtr])
          wPtr += 1
          rPtr += 1
      elif cmd == RLEMaskByte:
        byte = rleBuffer[rPtr]
        rPtr += 1
        for i in range(length):
          outBuf.append(self.rawdata[wPtr] ^ byte)
          wPtr += 1
      elif cmd == RLEMaskWord:
        bl = rleBuffer[rPtr:rPtr+2]
        rPtr += 2
        for i in range(length):
          for x in range(2):
            outBuf.append(self.rawdata[wPtr+x] ^ bl[x])
          wPtr += 2
      elif cmd == RLEMaskDWord:
        bl = rleBuffer[rPtr:rPtr+4]
        rPtr += 4
        for i in range(length):
          for x in range(4):
            outBuf.append(self.rawdata[wPtr+x] ^ bl[x])
          wPtr += 4
    self.rawdata = outBuf

  def save_png(self):
    self.write_png( "{0}/{1}".format(MDLFile.write_path,self.name) )

  def fromReader(self,reader):
    self.namespace = reader.namespace
    self.name = reader.namespace + ".png"
    
    self.info = BinarySurfaceInfo().fromReader(reader)
    reader.align_bytes()

    self.rawdata = reader.f.read(self.info.pitch * self.info.size.y)
    
    self.save_png()
    return self

  def __getstate__(self):
    return { 'resourcePath' : self.name }

  def __setstate__(self,state):
    self.__dict__.update(state)

  def write_png(self,path):
    #if os.path.isfile(path): return

    img = PIL.Image.new("RGBA",(self.info.size.x,self.info.size.y))
    colors = []
    
    pixdata = []
    for i in range(self.info.size.y):
      pixdata.extend(self.rawdata[self.info.pitch*i:self.info.pitch*i+self.info.size.x*2])
    
    ptups = zip( pixdata[0::2], pixdata[1::2] )
    for p in ptups:
      c = p[0] | p[1] << 8
      colors.append(self.info.get_rgb(c))

    img.putdata(colors)
    img.save(path)

class ImportImageFactory:
  def read(self,reader,stack):
    return Image().fromReader(reader)

class TextureGeo(Geo):
  def __init__(self,stack):
    self.geo = UniquePtr(stack.pop())
    self.image = stack.pop()

  def __str__(self):
    return "TextureGeo (" + self.geo.ptr_wrapper.data + ", " + self.image + ")"

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
      try:
        NamespaceManager.add( namespace, libname, MDLFile("{1}/{0}.mdl".format(libname,args.artwork),namespace).objects[libname])
        print( "Trying again..." )
        return NamespaceManager.objects[namespace][libname]
      except (KeyError, FileNotFoundError):
        print( "Still can't find asset, returning nothing" )
        NamespaceManager.add( namespace, libname, None )
        return NamespaceManager.objects[namespace][libname]

  @staticmethod
  def add( namespace, libname, obj ):
    if namespace not in NamespaceManager.objects:
      NamespaceManager.objects[namespace] = {}
    NamespaceManager.objects[namespace][libname] = obj
  
class CharData:
  def fromReader(self,reader):
    self.size = Point().fromReader(reader)
    self.offset = reader.read_int()
    return self

  def setUV(self,x,y,w,h):
    self.startUV = Point()
    self.startUV.x = float(x)/w
    self.startUV.y = float(y)/h
    
    self.endUV = Point()
    self.endUV.x = float(x+self.size.x)/w
    self.endUV.y = float(y+self.size.y)/h

  def hasRoom(self,dX,width):
    return dX + self.size.x <= width-1

  def drawCharacter(self,data,dx,dy,img):
    rPtr = self.offset
    for y in range(self.size.y):
      for x in range(int((self.size.x+7)/8)):
        b = data[rPtr]
        m = 1
        for i in range(8):
          px = (dx+x*8+i,dy+y)
          if b & m != 0:
            img.putpixel(px,(255,255,255))
          m <<= 1
        rPtr += 1

class Font:
  def fromReader(self,reader):
    self.namespace = reader.namespace
    self.objName = reader.currentObjectName
    self.height = reader.read_dword()
    self.width = reader.read_dword()
    dataSize = reader.read_dword()
    self.char_data = []
    for i in range(256):
      self.char_data.append(CharData().fromReader(reader))
    data = reader.f.read(dataSize)

    self.generateFontTexture(data)
    return self

  def generateFontTexture(self,data):
    texWidth = 256
    texHeight = 256
    nCharWide = texWidth / self.width
    nCharHigh = texHeight / self.height

    if nCharWide * nCharHigh <= 256:
      texWidth <<= 1
      nCharWide = texWidth / self.width
      if nCharWide * nCharHigh <= 256:
        texHeight <<= 1
        nCharHigh = texHeight / self.height
        if nCharWide * nCharHigh <= 256:
          print( "Texture is too big, aborting" )
          return

    img = PIL.Image.new("RGB",(texWidth,texHeight))
    dX = 0
    dY = 0
    fTexWidth = float(texWidth-1)
    fTexHeight = float(texHeight-1)
    for i in range(256):
      if not self.char_data[i].hasRoom(dX,texWidth):
        dX = 0
        dY += self.height
      self.char_data[i].setUV(dX,dY,fTexWidth,fTexHeight)
      self.char_data[i].drawCharacter(data,dX,dY,img)
      dX += self.char_data[i].size.x
    
    path = "{0}{1}.png".format(MDLFile.write_path,self.objName)
    img.save( path )

class ImportFontFactory:
  def read(self,reader,stack):
    font = Font()
    font.fromReader(reader)
    return font

class NotMDLException(Exception):
  pass

class BadMDLVersionException(Exception):
  pass

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
  token_str = ["OBJ_END","OBJ_FLOAT","OBJ_STRING","OBJ_TRUE","OBJ_FALSE","OBJ_LIST","OBJ_APPLY","OBJ_BINARY","OBJ_REF","OBJ_IMPORT","OBJ_PAIR"]

  def __init__(self,path,namespace):
    print( "Loading MDL file", path, "in namespace", namespace )
    self.namespace = namespace
    with io.open(path,"rb") as self.f:
      magic, = struct.unpack("I",self.f.read(4))
      
      if magic != MDLFile.MAGIC:
        raise NotMDLException
#("Not an MDL File")

      version, = struct.unpack("I",self.f.read(4))
      if version != MDLFile.VERSION:
        raise BadMDLVersionException
#("Wrong MDL Version")

      numNameSpaces, numImports, numExports, numDefinitions = struct.unpack("IIII",self.f.read(4*4))      
      self.nsImports = {}
      for i in range(numNameSpaces):
        self.nsImports[i] = self.read_string()

      self.objImports = {}
      for i in range(numImports):
        nsLibIndex = self.read_dword()
        nsLibName = self.read_string()
        self.objImports[int(i)] = NamespaceManager.find(self.nsImports[nsLibIndex],nsLibName)

      numObjects = numExports + numDefinitions
      self.exportNames = []
      for i in range(numExports):
        self.exportNames.append(self.read_string())
     
      self.objects = {}
      for i in range(numObjects):
        objIndex = self.read_dword() 
        obj = self.read_object(self.exportNames[objIndex])
        self.objects[self.exportNames[objIndex]] = obj
          
  def read_object(self,objName):
    stack = []
    self.currentObjectName = objName
    while True:
      token = self.read_dword()
      #print("Token is",MDLFile.token_str[token])
      if token == MDLFile.OBJ_FLOAT: stack.append( self.read_float() )
      elif token == MDLFile.OBJ_STRING: stack.append( self.read_string() )
      elif token == MDLFile.OBJ_TRUE: stack.append( True )
      elif token == MDLFile.OBJ_FALSE: stack.append( False )
      elif token == MDLFile.OBJ_LIST: stack.append( self.read_list() )
      elif token == MDLFile.OBJ_APPLY: stack.append( self.read_apply(stack) )
      elif token == MDLFile.OBJ_BINARY: stack.append( self.read_binary(stack) )
      elif token == MDLFile.OBJ_REF:
        i = self.read_dword()
        stack.append( self.objects[self.exportNames[i]] )
      elif token == MDLFile.OBJ_IMPORT: stack.append( self.objImports[self.read_dword()] )
      elif token == MDLFile.OBJ_PAIR: stack.append( self.read_pair(stack) )
      elif token == MDLFile.OBJ_END:
        if len(stack) > 0:
          return stack.pop()
        else:
          return None
      else: raise Exception("Read Object, bad token {0} at position {1}({2})".format(token,self.f.tell(),hex(self.f.tell())))
      #print("Stack is now",stack)
    self.currentObjectName = None

  def read_list(self):
    count = self.read_dword()
    lData = []
    for i in range(count):
      lData.insert(0,self.read_object("foo"+str(i)))
    return lData

  def read_pair(self,stack):
    return {"key" : stack.pop(), "value" : stack.pop()}

  def read_float(self):
    return struct.unpack("f",self.f.read(4))[0]

  def read_int(self):
    return struct.unpack("i",self.f.read(4))[0]

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

  def align_bytes(self):
    skip = 3-((self.f.tell()-1)%4)
    self.f.read(skip)

  def write_json(self):
    with open( "/".join([MDLFile.write_path,self.namespace])+".json", "w" ) as f:
      jsonpickle.set_encoder_options('json', indent=4, sort_keys=True)
      json_str = jsonpickle.encode(self,False)
      f.write( json_str )

  def __getstate__(self):
    if "object" in self.objects: self.objects['object'] = UniquePtr(self.objects['object'])
    return { 'value0' : self.objects }

  def __setstate__(self,state):
    self.__dict__.update(state)

if len(sys.argv) < 2:
  print( "Provide filename root" )
  exit(1)

parser = argparse.ArgumentParser()
parser.add_argument( 'file' )
parser.add_argument( '--artwork', default="./Artwork" )
parser.add_argument( '--dest', default="./decompiled" )
parser.add_argument( '-u', action="store_true" )
args = parser.parse_args()

NamespaceManager.add( "model", "ModifiableNumber", ModifiableNumberFactory() )
NamespaceManager.add( "effect", "LightsGeo", LightsGeoFactory() )
NamespaceManager.add( "model", "time", TimeFactory() )
NamespaceManager.add( "effect", "FrameData", FrameDataFactory() )
NamespaceManager.add( "model", "LODGeo", LODGeoFactory() )
NamespaceManager.add( "model", "MeshGeo", MeshGeoFactory() )
NamespaceManager.add( "model", "TextureGeo", TextureGeoFactory() )
NamespaceManager.add( "model", "ImportImage", ImportImageFactory() )
NamespaceManager.add( "model", "GroupGeo", GroupGeoFactory() )
NamespaceManager.add( "model", "FrameImage", FrameImageFactory() )
NamespaceManager.add( "model", "ImportFont", ImportFontFactory() )
MDLFile.write_path = args.dest
MeshGeo.UVimage = args.u
try:
  srcDir = os.path.join(args.artwork,args.file)+".mdl"
  print("Src Path :",srcDir)
  mdlfile = MDLFile(srcDir,args.file)
  mdlfile.write_json()
except NotMDLException:
  print("ASCII MDL File, skipping")
