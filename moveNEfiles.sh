#!/bin/bash

# Check if the correct number of arguments are provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 target_directory"
    exit 1
fi

# Assign the target directory to a variable
TARGET_DIR=$1

# Check if the target directory exists
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: Target directory '$TARGET_DIR' does not exist."
    exit 1
fi

# Move files
cp -r configfiles/ChargedLeptonReco ${TARGET_DIR}/configfiles
cp -r UserTools/ChargedLeptonLikelihoodReco ${TARGET_DIR}/UserTools
cp    DataModel/DetectorResponsePredictor.* ${TARGET_DIR}/DataModel
cp    DataModel/LikelihoodCalculator.* ${TARGET_DIR}/DataModel
cp    DataModel/THistReader.h ${TARGET_DIR}/DataModel

# Check if the move command was successful
if [ $? -eq 0 ]; then
    echo "Files moved successfully."
else
    echo "Error: Failed to move files."
    exit 1
fi

