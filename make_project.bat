@echo off
title build project script
rmdir /Q /S build
cmake -S . -B build
start ./build/opengl_study.sln
@echo on
