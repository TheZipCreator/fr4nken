const c = @import("c.zig");
const std = @import("std");
const fmt = std.fmt;

// /// Acquires a value
// void fr_acquireV(fr_Value v);
// /// Releases a value
// void fr_releaseV(fr_Value v);
// /// Hashes a value
// size_t fr_hash(fr_Value v);
// /// Converts a value to a string
// fr_Object *fr_toString(fr_Value v);

pub fn fr_acquireV(v: c.fr_Value) callconv(.C) void {
    if(v.type != c.FR_TYPE_OBJECT)
        return;
    c.fr_acquire(v.unnamed_0.vobject);
}
pub fn fr_releaseV(v: c.fr_Value) callconv(.C) void {
    if(v.type != c.FR_TYPE_OBJECT)
        return;
    c.fr_release(v.unnamed_0.vobject);
}
pub fn fr_hash(v: c.fr_Value) callconv(.C) usize {
    return switch(v.type) {
        c.FR_TYPE_UNDEFINED => 0,
        c.FR_TYPE_NULL => 1,
        c.FR_TYPE_INT => @intCast(v.unnamed_0.vint),
        c.FR_TYPE_FLOAT => @intCast(@as(u64, @bitCast(v.unnamed_0.vfloat))),
        c.FR_TYPE_BOOL => if(v.unnamed_0.vbool) 2 else 3,
        c.FR_TYPE_OBJECT => {
            const obj = v.unnamed_0.vobject;
            if(!c.fr_hasMethod(obj, "hash"))
                return @intFromPtr(obj);
            const val = c.fr_callMethod(obj, "hash", 0, null);
            if(val.type != c.FR_TYPE_INT) {
                c.fr_printError("Hash function returned non-integer!");
                c.fr_releaseV(val);
                return 0;
            }
            return @bitCast(val.unnamed_0.vint);
        },
        else => {
            c.fr_printError("Unexpected type %"++c.PRIu8++" in hash", v.type);
            return 0;
        }
    };
}
pub fn fr_equal(a: c.fr_Value, b: c.fr_Value) callconv(.C) bool {
    if(a.type != b.type)
        return false;
    return switch(a.type) {
        c.FR_TYPE_UNDEFINED, c.FR_TYPE_NULL => true,
        c.FR_TYPE_INT => a.unnamed_0.vint == b.unnamed_0.vint,
        c.FR_TYPE_FLOAT => a.unnamed_0.vfloat == b.unnamed_0.vfloat,
        c.FR_TYPE_BOOL => a.unnamed_0.vbool == b.unnamed_0.vbool,
        c.FR_TYPE_OBJECT => {
            const ao = a.unnamed_0.vobject;
            const bo = b.unnamed_0.vobject;
            if(!c.fr_hasMethod(ao, "equals"))
                return ao == bo;
            const val = c.fr_callMethod(ao, "equals", 1, &[_]c.fr_Value { b });
            if(val.type != c.FR_TYPE_BOOL) {
                c.fr_printError("Equals function returned non-bool!");
                c.fr_releaseV(val);
                return false;
            }
            return val.unnamed_0.vbool;
        },
        else => {
            c.fr_printError("Unexpected type %"++c.PRIu8++" in equals", a.type);
            return false;
        }
    };
}
pub fn intoFrString(str: []const u8) *c.fr_Object {
    return c.fr_newString(str.ptr, str.len);
}
pub fn fr_toString(v: c.fr_Value) callconv(.C) *c.fr_Object {
    return switch(v.type) {
        c.FR_TYPE_UNDEFINED => intoFrString("undefined"),
        c.FR_TYPE_NULL => intoFrString("null"),
        c.FR_TYPE_INT => {
            var buf: [32]u8 = undefined;
            return intoFrString(fmt.bufPrint(&buf, "{d}", .{ v.unnamed_0.vint }) catch "");
        },
        c.FR_TYPE_FLOAT => {
            var buf: [32]u8 = undefined;
            return intoFrString(fmt.bufPrint(&buf, "{d}", .{ v.unnamed_0.vfloat }) catch "");
        },
        c.FR_TYPE_BOOL => intoFrString(if(v.unnamed_0.vbool) "true" else "false"),
        c.FR_TYPE_OBJECT => {
            const obj = v.unnamed_0.vobject;
            if(c.fr_hasMethod(obj, "toString")) {
                const val = c.fr_callMethod(obj, "toString", 0, null);
                if(val.type != c.FR_TYPE_OBJECT or val.unnamed_0.vobject.*.dataTag != c.fr_stringDataTag) {
                    c.fr_printError("toString returned non-string!");
                    c.fr_releaseV(val);
                    return intoFrString("unknown");
                }
                return val.unnamed_0.vobject;
            }
            var buf: [32]u8 = undefined;
            return intoFrString(fmt.bufPrint(&buf, "<object @ {*}>", .{ v.unnamed_0.vobject }) catch "");
        },
        else => {
            c.fr_printError("Unexpected type %"++c.PRIu8++" in toString", v.type);
            return intoFrString("unknown");
        }
    };
}
