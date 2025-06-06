const std = @import("std");

export fn zig_preinit() callconv(.C) void {
    std.debug.print("zig preinit!\n", .{});
}

export fn zig_init() callconv(.C) void {
    std.debug.print("zig init!\n", .{});
}
