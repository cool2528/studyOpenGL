@echo off
title build project script
cmake -S . -B build
start ./build/opengl_study.sln
@echo on
