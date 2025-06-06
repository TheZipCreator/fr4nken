const c = @cImport({
    @cInclude("c/errorhandling.h");
    @cInclude("c/hashtable.h");
    @cInclude("c/init.h");
    @cInclude("c/object.h");
    @cInclude("c/objectutils.h");
    @cInclude("c/string.h");
    @cInclude("c/value.h");
    @cInclude("inttypes.h");
});

pub usingnamespace c;
