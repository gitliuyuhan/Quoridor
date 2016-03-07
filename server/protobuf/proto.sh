#!/bin/sh
protoc -I=./ --cpp_out=./ ./game.SessionMsg.proto
