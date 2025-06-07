const std = @import("std");
const c = @import("c.zig");

const ally = std.heap.c_allocator;

pub var eventDataTag: u32 = undefined;

pub const Event = struct {
    listeners: std.ArrayList(*c.fr_Object),

    fn destroy(_: u32, d: ?*anyopaque) callconv(.C) void {
        const data = @as(*Event, @ptrCast(@alignCast(d)));
        for(data.listeners.items) |obj| {
            c.fr_release(obj);
        }
        data.listeners.deinit();
        ally.destroy(data);
    }

    fn trigger(self: [*c]c.fr_Object, nargs: usize, args: [*c]const c.fr_Value) callconv(.C) c.fr_Value {
        const data = @as(*Event, @ptrCast(@alignCast(self.*.data)));
        for(data.listeners.items) |obj| {
            _ = c.fr_callMethod(obj, "trigger", nargs, args);
        }
        return c.FR_UNDEFINED;
    }
    fn listen(self: [*c]c.fr_Object, nargs: usize, args: [*c]const c.fr_Value) callconv(.C) c.fr_Value {
        const data = @as(*Event, @ptrCast(@alignCast(self.*.data)));
        for(args[0..nargs]) |val| {
            if(val.type != c.FR_TYPE_OBJECT) {
                c.fr_printError("Listener object is not an object.");
                continue;
            }
            const obj = val.unnamed_0.vobject;
            if(!c.fr_hasMethod(obj, "trigger")) {
                c.fr_printError("Listener object does not have `trigger` method.");
                continue;
            }
            c.fr_acquire(obj);
            data.listeners.append(obj) catch c.fr_printError("Could not append to listener arraylist.");
        }
        return c.FR_UNDEFINED;
    }
    
    pub fn make() *c.fr_Object {
        const event = ally.create(Event) catch @panic("");
        event.listeners = std.ArrayList(*c.fr_Object).init(ally);
        const ret = c.fr_new(eventDataTag, event, &destroy);
        c.fr_addMethod(ret, "trigger", &trigger);
        c.fr_addMethod(ret, "listen", &listen);
        return ret;
    }
};
