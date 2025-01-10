#!/bin/bash

cmake --build build --target all -- -j 4
cmake --build build --target run