# Kaleidoscope-Simulator-Control

Use the physical keyboard to generate real-time input for simulated firmware runs.

## Introduction

This plugin sends the state of any keys of the keyboard to the host system via
serial communication. This information may be fed to a simulation of the same
keyboard running on the host system.

## Usage

Include the plugin's header somewhere at the top of your sketch.

```cpp
#include "Kaleidoscope-Simulator-Control.h"
```

The plugin captures key events and discards them while enabled. Discarding here means
that the key events do not lead to any keys being send to the host system via usb keyboard reports.
Instead, while enabled, the plugin keeps track of the state of the keyboard matrix and sends it to the host via serial communication.

Because of this, it is important to let Kaleidoscope-Simulator-Control be the first in the list of registered plugins.

```cpp
KALEIDOSCOPE_INIT_PLUGINS(
   SimulatorControl,
   ... more plugins ...
)
```

## Activation

The plugin is deactivated when the firmware starts. It has to be actively activated. This is done by hitting two pre-defined keys together. For the Keyboardio Model01 these two keys are pre-defined as
the outermost upper keys on both halfs of the keyboard.

Alternative keys can be defined through the method `setActivationKeys(...)` that must be invoked with the row/col values of the two activation keys. This is best done in Arduino's `setup()` method, e.g.

```cpp
void setup() {
   SimulatorControl.setActivationKeys(0 /*row of key 1*/, 1 /*col of key 1*/,
                                      1 /*row of key 1*/, 1 /*col of key 1*/);
   ... more setup ...
}
```
