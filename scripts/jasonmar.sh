#!/bin/bash

start=$1

mkdir "$start/n"
echo "Here is the facility" > "$start/n/desc.txt"

mkdir "$start/n/n"
echo "Now you are at the farm room" > "$start/n/n/desc.txt"

mkdir "$start/n/n/n"
echo "Spend time in the family room" > "$start/n/n/n/desc.txt"

#Branch
mkdir "$start/n/n/n/e"
echo "Sleep in the fluffy room" > "$start/n/n/n/e/desc.txt"

mkdir "$start/n/n/n/e/e"
echo "Here is the food court" > "$start/n/n/n/e/e/desc.txt"
echo "You found a Brown Stick!" > "$start/n/n/n/e/e/item.txt"

mkdir "$start/n/n/n/n"
echo "This is the french fry area" > "$start/n/n/n/n/desc.txt"

mkdir "$start/n/n/n/n/n"
echo "You find some fruits here" > "$start/n/n/n/n/n/desc.txt"

mkdir "$start/n/n/n/n/n/e"
echo "There are faces staring at you" > "$start/n/n/n/n/n/e/desc.txt"

mkdir "$start/n/n/n/n/n/e/e"
echo "It feels kind of hot here" > "$start/n/n/n/n/n/e/e/desc.txt"

mkdir "$start/n/n/n/n/n/e/e/e"
echo "Here is the final room" > "$start/n/n/n/n/n/e/e/e/desc.txt"

echo "Connector is at $start/n/n/n/n/n/e/e/e"
