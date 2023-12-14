#!/bin/bash

start=$1

mkdir "$start/n"
echo "You find a campfire" > "$start/n/desc.txt"

mkdir "$start/n/n"
echo "There are swords lying about" > "$start/n/n/desc.txt"

mkdir "$start/n/n/n"
echo "You have reached a red room" > "$start/n/n/n/desc.txt"

mkdir "$start/n/n/n/n"
echo "You found a river" > "$start/n/n/n/n/desc.txt"

mkdir "$start/n/n/n/n/e"
echo "You went through the river" > "$start/n/n/n/n/e/desc.txt"

mkdir "$start/n/n/n/n/e/n"
echo "You spot a small island" > "$start/n/n/n/n/e/n/desc.txt"
echo "You found a Rainbow Stick!" > "$start/n/n/n/n/e/n/item.txt"

mkdir "$start/n/n/n/n/e/e"
echo "You reached the river's end" > "$start/n/n/n/n/e/e/desc.txt"

mkdir "$start/n/n/n/n/e/e/e"
echo "You find the server room" > "$start/n/n/n/n/e/e/e/desc.txt"

mkdir "$start/n/n/n/n/e/e/e/n"
echo "This is the CS2600 room" > "$start/n/n/n/n/e/e/e/n/desc.txt"

mkdir "$start/n/n/n/n/e/e/e/n/e"
echo "You made it to the castle" > "$start/n/n/n/n/e/e/e/n/e/desc.txt"

echo "Connector is at $start/n/n/n/n/e/e/e/n/e"
