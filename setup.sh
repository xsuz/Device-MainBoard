#!/usr/bin/env bash

user=$(git config --get user.name)
email=$(git config --get user.email)

year=$(date +%Y)
repo_url=$(git remote get-url origin)
repo_name=$(basename -s .git "$repo_url")

device_name=${repo_name#Device-}


# modify kicad files

echo "Initializing KiCAD Library..."

cat circuit/Device-Template/Device-Template.kicad_pcb 	| sed 's/Device-Template/'$device_name'/' > 'circuit/Device-Template/'$device_name'.kicad_pcb'
cat circuit/Device-Template/Device-Template.kicad_prl 	| sed 's/Device-Template/'$device_name'/' > 'circuit/Device-Template/'$device_name'.kicad_prl'
cat circuit/Device-Template/Device-Template.kicad_pro 	| sed 's/Device-Template/'$device_name'/' > 'circuit/Device-Template/'$device_name'.kicad_pro'
cat circuit/Device-Template/Device-Template.kicad_sch 	| sed 's/Device-Template/'$device_name'/' > 'circuit/Device-Template/'$device_name'.kicad_sch'

rm circuit/Device-Template/Device-Template* -rf
mv 'circuit/Device-Template' 'circuit/'$device_name

# initialize git submodule

echo "Updating submodules..."

git submodule update --init
