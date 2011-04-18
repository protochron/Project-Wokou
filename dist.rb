#!/usr/bin/ruby

# This can be run to return a project back to a clean slate. It removes all the files cmake generates.

#require 'Dir'
#require 'File'

`rm -rf CMakeFiles`
`rm -rf bin`
`rm CMakeCache.txt`
`rm Makefile`
`rm cmake_install.cmake`
`rm *.xcodeproj`

