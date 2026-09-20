# Vendored ITEAD Nextion library

Upstream: https://github.com/itead/ITEADLIB_Arduino_Nextion (master)

This copy is vendored because the RouterLift sketch depends on local
modifications to the library. The library's headers include `NexConfig.h`
with quotes (`#include "NexConfig.h"`), so it always resolves from this
folder -- a copy placed in the sketch folder or an extra include path is
ignored. The modifications therefore have to live here.

Only the library source is vendored. Upstream's `examples/`, `doc/`,
`html/` and `latex/` (~40 MB of generated Doxygen output and demo .tft/.HMI
display binaries) are omitted; none of it is needed to build.

`~/Documents/Arduino/libraries/ITEADLIB_Arduino_Nextion` is a symlink to
this folder, so the Arduino IDE, arduino-cli and the VS Code extension all
build against this copy. Edits here take effect on the next build.

## Local modifications

Record each change as you make it.

- [ ] `NexConfig.h` -- `nexSerial` set to the port the panel is wired to
      (upstream default is `Serial2`; sketch comments say `Serial3`).
- [ ] `NexHardware.h` / `NexHardware.cpp` -- `nexInit()` takes a baud rate.
      The sketch calls `nexInit(115200)`; upstream declares `nexInit(void)`.
