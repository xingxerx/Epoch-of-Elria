#!/bin/bash

# Set the script to exit immediately on any command error
set -e

# Get the directory of the current script
SCRIPT_DIR="$(dirname "$0")"

# Get the current working directory
WORKING_DIR="$(pwd)"

# Initialize variables
CONDA_ENV_NAME="ai-toolkit-env" # Default environment name
FORCE_RESET=false

# Parse arguments
for arg in "$@"; do
    if [ "$arg" == "-force" ]; then
        FORCE_RESET=true
    else
        CONDA_ENV_NAME="$arg" # Set environment name if argument is not -force
    fi
done

# Refresh conda env
conda init
. ~/.bashrc

# Check if environment exists and remove if -force is used
if conda env list | grep -q "^$CONDA_ENV_NAME\s"; then
    if [ "$FORCE_RESET" == "true" ]; then
        echo "Force reset requested. Removing existing Conda environment '$CONDA_ENV_NAME'..."
        conda env remove -n "$CONDA_ENV_NAME" -y
        echo "Existing Conda environment '$CONDA_ENV_NAME' removed."
    else
        echo "Conda environment '$CONDA_ENV_NAME' already exists. Use -force to overwrite."
        # Optionally, exit here if not forcing and environment exists
        # exit 0
    fi
fi

# Create or reset environment from conda.yaml located in the script's directory
echo "Creating or (re)creating Conda environment '$CONDA_ENV_NAME' from $SCRIPT_DIR/conda-environment.yml..."
conda env create --name "$CONDA_ENV_NAME" -f "$SCRIPT_DIR/conda-environment.yml"

echo "Script execution completed."
