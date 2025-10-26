#!/bin/bash

unset WAYLAND_DISPLAY
export XDG_SESSION_TYPE=x11
./build/my_program
