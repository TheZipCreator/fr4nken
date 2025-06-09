const c = @import("c.zig");
const std = @import("std");
const value = @import("value.zig");
const event = @import("event.zig");

comptime {
    @export(&value.fr_acquireV, .{ .name = "fr_acquireV", .linkage = .strong });
    @export(&value.fr_releaseV, .{ .name = "fr_releaseV", .linkage = .strong });
    @export(&value.fr_hash, .{ .name = "fr_hash", .linkage = .strong });
    @export(&value.fr_equal, .{ .name = "fr_equal", .linkage = .strong });
    @export(&value.fr_toString, .{ .name = "fr_toString", .linkage = .strong });
    @export(&event.eventDataTag, .{ .name = "fr_eventDataTag", .linkage = .strong });
}

export fn zig_preinit() callconv(.C) void {
    event.eventDataTag = c.fr_newDataTag();
	const make = c.fr_registryGet("make").*;
    c.fr_addMethod(make, "event", &makeEvent);
}

fn makeEvent(_: [*c]c.fr_Object, _: usize, _: [*c]const c.fr_Value) callconv(.C) c.fr_Value {
    return .{ .type = c.FR_TYPE_OBJECT, .unnamed_0 = .{ .vobject = event.Event.make() } };
}

export fn zig_init() callconv(.C) void {}
export fn zig_deinit() callconv(.C) void {}
