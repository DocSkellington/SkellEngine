---
layout: tutorial
title: Logger
shorttitle: Logger
---

The [Logger]({{ "/documentation/alpha/c++/classengine_1_1Logger.html" | relative_url }}), as its name suggests, is used to log messages.
The messages can be written to a file and/or to the standard output, depending on the configuration.
The previous log file is erased when the engine starts.

Each message has a level (or a type).
The types are `Info`, `Warning` and `Error`.
Depending on the configuration, only certain levels are printed.

The logger allows to [print simple log messages]({{ "/documentation/alpha/c++/classengine_1_1Logger.html#a1f58527f865753bb12de87cd881a6805" | relative_url }}). You can also [print an exception]({{ "/documentation/alpha/c++/classengine_1_1Logger.html#a052fd72d3e80d16bb516cae1582a3f19" | relative_url }}) (with [possibly a prefixed message]({{ "/documentation/alpha/c++/classengine_1_1Logger.html#a1ff418e60f744c86318ea4ae0889fdb1" | relative_url }})).
You can also [force printing a message]({{ "/documentation/alpha/c++/classengine_1_1Logger.html#abe28a957cb687b4656e73e93e3f5bc37" | relative_url }}).
This function does not require to have an reference to the logger but only prints to the standard output and ignores the settings.