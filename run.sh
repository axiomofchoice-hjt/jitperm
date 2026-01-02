#!/bin/sh

xmake project -k compile_commands build
xmake
xmake run
