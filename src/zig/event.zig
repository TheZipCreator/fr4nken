const std = @import("std");
const c = @import("c.zig");

const ally = std.heap.c_allocator;

pub var eventDataTag: u32 = undefined;

pub const Event = struct {
    listeners: std.ArrayList(c.fr_Closure),

    fn destroy(_: u32, d: ?*anyopaque) callconv(.C) void {
        const data = @as(*Event, @ptrCast(@alignCast(d)));
        for(data.listeners.items) |cl| {
            c.fr_release(cl.obj);
        }
        data.listeners.deinit();
        ally.destroy(data);
    }

    fn trigger(self: [*c]c.fr_Object, nargs: usize, args: [*c]const c.fr_Value) callconv(.C) c.fr_Value {
        const data = @as(*Event, @ptrCast(@alignCast(self.*.data)));
        for(data.listeners.items) |cl| {
            _ = c.fr_callClosure(cl, nargs, args);
        }
        return c.FR_UNDEFINED;
    }
    fn listen(self: [*c]c.fr_Object, nargs: usize, args: [*c]const c.fr_Value) callconv(.C) c.fr_Value {
        const data = @as(*Event, @ptrCast(@alignCast(self.*.data)));
        for(args[0..nargs]) |val| {
            if(val.type != c.FR_TYPE_CLOSURE) {
                c.fr_printError("Listener is not closure.");
                continue;
            }
            c.fr_acquireV(val);
            data.listeners.append(val.unnamed_0.vclosure) catch c.fr_printError("Could not append to listener arraylist.");
        }
        return c.FR_UNDEFINED;
    }
    
    pub fn make() *c.fr_Object {
        const event = ally.create(Event) catch @panic("");
        event.listeners = std.ArrayList(c.fr_Closure).init(ally);
        const ret = c.fr_new(eventDataTag, event, &destroy);
        c.fr_addMethod(ret, "trigger", &trigger);
        c.fr_addMethod(ret, "listen", &listen);
        return ret;
    }
};
