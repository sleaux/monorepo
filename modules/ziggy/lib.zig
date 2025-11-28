const std = @import("std");

export fn add_two(a: i32, b: i32) i32 {
    return a + b;
}

test add_two {
    try std.testing.expect(add_two(1, 2) == 3);
}
