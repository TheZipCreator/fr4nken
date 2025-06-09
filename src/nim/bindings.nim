# direct bindings
type
  fr_Object* = distinct pointer
  fr_Closure* = object
    obj: fr_Object
    meth: cstring
  ValueUnion* {.union.} = object
    vint: int64
    vfloat: float64
    vbool: bool
    vobject: fr_Object
    vclosure: fr_Closure
  ValueType* = enum
    vtUndefined = 0, vtNull = 1, vtInt = 2, vtFloat = 3, vtBool = 4, vtObject = 5, vtClosure = 6
  fr_Value = object
    tag: uint8
    u: ValueUnion
  fr_DataInfo = object
    tag: uint32
    data: pointer
  fr_DestroyData* = proc(tag: uint32, data: pointer) {.cdecl.}
  usize = uint64 # could break on non-64-bit architectures
  fr_Method* = proc(self: fr_Object, nargs: usize, args: ptr UncheckedArray[fr_Value]) {.cdecl.}

  

proc fr_newDataTag*(): uint32 {.importc: "fr_newDataTag"}
proc fr_new*(dataTag: uint32, data: pointer, destroy: fr_DestroyData): fr_Object {.importc: "fr_new".}
proc fr_registryPut*(name: cstring, obj: fr_Object) {.importc: "fr_registryPut".}
proc fr_registryGet*(name: cstring): ptr fr_Object {.importc: "fr_registryGet".}
proc fr_addMethod*(obj: fr_Object, name: cstring, meth: fr_Method) {.importc: "fr_addMethod".}
proc fr_printError*(fmt: cstring) {.importc: "fr_printError", varargs.}
proc fr_typename*(ty: uint8): cstring {.importc: "fr_typename".}
proc fr_getDataInfo*(obj: fr_Object): fr_DataInfo {.importc: "fr_getDataInfo".}
proc fr_callMethod*(obj: fr_Object, name: cstring, nargs: usize, args: ptr UncheckedArray[fr_Value]): fr_Value {.importc: "fr_callMethod".}

# higher level bindings
type
  Value* = fr_Value
  Object* = fr_Object
  Method* = proc(self: Object, args: openArray[Value]): Value

template getType*(val: Value): ValueType = cast[ValueType](val.tag)
template asInt*(val: Value): int64 = val.u.vint
template asFloat*(val: Value): float64 = val.u.vfloat
template asBool*(val: Value): bool = val.u.vbool
template asObject*(val: Value): Object = val.u.vobject

template ensureType*(val: Value, ty: ValueType): untyped =
  if val.getType() != ty:
    fr_printError("Expected variable of type %s, got variable of type %s.", fr_typename(val.tag), fr_typename(cast[uint8](ty)))
    return undefined

template ensureArgsEq*(amt: int): untyped =
  if args.len != amt:
    fr_printError("Expected exactly %d arguments, got %d arguments.", cast[cint](amt), cast[cint](args.len))
    return undefined

const
  undefined* = Value(tag: cast[uint8](vtUndefined))
  null* = Value(tag: cast[uint8](vtUndefined))

template makeInt*(v: int64): Value = Value(tag: cast[uint8](vtInt), u: ValueUnion(vint: v))
template makeFloat*(v: float64): Value = Value(tag: cast[uint8](vtFloat), u: ValueUnion(vfloat: v))
template makeBool*(v: bool): Value = Value(tag: cast[uint8](vtBool), u: ValueUnion(vbool: v))
template makeObject*(v: Object): Value = Value(tag: cast[uint8](vtInt), u: ValueUnion(vobject: v))
template getData*[T](o: Object): ptr T = cast[ptr T](fr_getDataInfo(o).data)

template addMethod*(obj: Object, name: cstring; self, args, actions: untyped): untyped =
  fr_addMethod(obj, name, cast[fr_Method](proc(self: fr_Object, nargs: usize, cargs: ptr UncheckedArray[fr_Value]): fr_Value {.cdecl.} =
    var args = toOpenArray(cargs, 0, cast[int](nargs)-1)
    actions
  ))

# SDL
import sdl2
proc drawLineF*(rend: RendererPtr, x1: cfloat, y1: cfloat, x2: cfloat, y2: cfloat): int {.importc: "SDL_RenderDrawLineF".}
