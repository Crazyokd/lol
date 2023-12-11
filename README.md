# LOL
named Log fOr Log.

# Usage
```shell
sudo apt install libyaml-dev
meson setup --prefix=`pwd`/install build
meson install -C build && ./install/bin/lol-example

# format meson.build
meson compile muon-fmt -C build
# format .c & .h source file
meson compile format -C build
```

# TODO
- [ ] add `abort` function.
- [x] configurable
