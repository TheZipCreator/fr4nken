# create object to interact with window and renderer

import sdl2
import bindings

var
  graphicsDataTag*: uint32 = 0

type
  Graphics* = object
    win*: WindowPtr
    rend*: RendererPtr

proc destroyGraphics*(tag: uint32, data: pointer) {.cdecl.} =
  dealloc(data)

import std/strformat

proc createGraphics*(win: WindowPtr, rend: RendererPtr): fr_Object =
  var data = create(Graphics)
  data.win = win
  data.rend = rend
  result = fr_new(graphicsDataTag, cast[pointer](data), (destroyGraphics))
  result.addMethod("color", self, args):
    ensureArgsEq(4)
    ensureType(args[0], vtInt)
    ensureType(args[1], vtInt)
    ensureType(args[2], vtInt)
    ensureType(args[3], vtInt)
    let data = self.getData[:Graphics]()
    data.rend.setDrawColor cast[uint8](args[0].asInt()), cast[uint8](args[1].asInt()), cast[uint8](args[2].asInt()), cast[uint8](args[3].asInt())
    return undefined
  result.addMethod("line", self, args):
    ensureArgsEq(4)
    ensureType(args[0], vtFloat)
    ensureType(args[1], vtFloat)
    ensureType(args[2], vtFloat)
    ensureType(args[3], vtFloat)
    let data = self.getData[:Graphics]()
    discard data.rend.drawLineF(args[0].asFloat(), args[1].asFloat(), args[2].asFloat(), args[3].asFloat())
    return undefined

