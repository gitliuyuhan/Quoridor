#!/bin/bash
clang++ -o client -std=c++11 -lpthread -lprotobuf TestClient.cpp ../protobuf/game.SessionMsg.pb.cc 
