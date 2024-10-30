# LOL
named Logger fOr Log.

# Compile

## use make

```shell
make
```

## use meson

```shell
meson setup --prefix=`pwd`/install build
meson install -C build && ./install/bin/lol-example

# format meson.build
meson compile muon-fmt -C build
# format .c & .h source file
meson compile format -C build
```

# Usage

```c
// lol_init("main", LOL_INFO, "lol.log", LOL_DEBUG);
lol_init2();

lol_fatal("fatal");
lol_error("error");
lol_warn("warn");
lol_info("info");
lol_debug("debug");
lol_trace("trace");

lol_add_domain("test", LOL_INFO, NULL, LOL_NONE);
lol_fatal2("test", "fatal");
lol_error2("test", "error");
lol_warn2("test", "warn");
lol_info2("test", "info");
lol_debug2("test", "debug");
lol_trace2("test", "trace");

// like lol_xxx2 but with better performance
lol_fatal3(test, "fatal");
lol_error3(test, "error");
lol_warn3(test, "warn");
lol_info3(test, "info");
lol_debug3(test, "debug");
lol_trace3(test, "trace");
lol_fini();
```

# TODO

- [x] add support for log file
- [ ] optimize `libc_write` call
