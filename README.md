# LOL
named Log fOr Log.

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

lol_fini();
```

# TODO

- [x] add support for log file
- [ ] add support for log file rotate
