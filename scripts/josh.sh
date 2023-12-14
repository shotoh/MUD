#!/bin/bash

start=$1

mkdir "$start/e"
echo "You begin at a Base" > "$start/e/desc.txt"

mkdir "$start/e/n"
echo "Use Bathroom b4 you begin" > "$start/e/n/desc.txt"

mkdir "$start/e/n/n"
echo " Entering the Game Room" > "$start/e/n/n/desc.txt"

mkdir "$start/e/n/n/n"
echo "Relax in the Lounge" > "$start/e/n/n/n/desc.txt"

mkdir "$start/e/n/n/n/n"
echo "NSFW-> Sexy Room #58008" > "$start/e/n/n/n/n/desc.txt"
echo "You found a Big Black Stick!" > "$start/e/n/n/n/n/item.txt"

mkdir "$start/e/n/n/e"
echo "Entering the Dungeon" > "$start/e/n/n/e/desc.txt"

mkdir "$start/e/n/n/e/e"
echo "You found freedom room" > "$start/e/n/n/e/e/desc.txt"

mkdir "$start/e/n/n/e/e/s"
echo "This is the Kitchen" > "$start/e/n/n/e/e/s/desc.txt"

mkdir "$start/e/n/n/e/e/s/s"
echo "Entering the Dining Room" > "$start/e/n/n/e/e/s/s/desc.txt"

mkdir "$start/e/n/n/e/e/n"
echo "This is the Living Room" > "$start/e/n/n/e/e/n/desc.txt"

mkdir "$start/e/n/n/e/e/n/n"
echo "You went outside!" > "$start/e/n/n/e/e/n/n/desc.txt"

echo "Connector is at $start/e/n/n/e/e/n/n"
