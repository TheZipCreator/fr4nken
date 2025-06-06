const std = @import("std");
const value = @import("value.zig");

comptime {
    @export(&value.fr_acquireV, .{ .name = "fr_acquireV", .linkage = .strong });
    @export(&value.fr_releaseV, .{ .name = "fr_releaseV", .linkage = .strong });
    @export(&value.fr_hash, .{ .name = "fr_hash", .linkage = .strong });
    @export(&value.fr_equal, .{ .name = "fr_equal", .linkage = .strong });
    @export(&value.fr_toString, .{ .name = "fr_toString", .linkage = .strong });
}

export fn zig_preinit() callconv(.C) void {
    std.debug.print("zig preinit!\n", .{});
}

export fn zig_init() callconv(.C) void {
    std.debug.print("zig init!\n", .{});
}
