#!/bin/bash

start=$1

mkdir "$start/n"
echo "This is the entrance" > "$start/n/desc.txt"

mkdir "$start/n/e"
echo "Sit at the lobby" > "$start/n/e/desc.txt"

mkdir "$start/n/e/e"
echo "Run across the halls" > "$start/n/e/e/desc.txt"

mkdir "$start/n/e/e/s"
echo "You find yourself at the garden" > "$start/n/e/e/s/desc.txt"

mkdir "$start/n/e/e/s/s"
echo "You find more herbs" > "$start/n/e/e/s/s/desc.txt"

mkdir "$start/n/e/e/s/s/w"
echo "There is a gazebo hidden here" > "$start/n/e/e/s/s/w/desc.txt"
echo "You obtained a Yellow Stick!" > "$start/n/e/e/s/s/w/item.txt"

mkdir "$start/n/e/e/s/s/e"
echo "There are blue flowers here" > "$start/n/e/e/s/s/e/desc.txt"

mkdir "$start/n/e/e/s/s/e/e"
echo "You are now decontaminating" > "$start/n/e/e/s/s/e/e/desc.txt"

mkdir "$start/n/e/e/s/s/e/e/n"
echo "This is a BOSS room, RUN!" > "$start/n/e/e/s/s/e/e/n/desc.txt"

mkdir "$start/n/e/e/s/s/e/e/n/e"
echo "You are now safe" > "$start/n/e/e/s/s/e/e/n/e/desc.txt"

echo "Connector is at $start/n/e/e/s/s/e/e/n/e"
