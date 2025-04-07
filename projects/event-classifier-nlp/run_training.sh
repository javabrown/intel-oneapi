#!/bin/bash

# -----------------------
# Project Bootstrap Script
# -----------------------
PROJECT_DIR="event-classifier-nlp"
REQUIREMENTS_FILE="requirements.txt"
TRAIN_SCRIPT="scripts/train.py"

echo "🔍 Checking environment..."

# Step 1: Check Python version
if ! command -v python3 &> /dev/null; then
    echo "❌ Python3 is not installed. Please install Python 3.7 or higher."
    exit 1
fi

# Step 2: Check pip
if ! command -v pip &> /dev/null; then
    echo "❌ pip is not installed. Please install pip."
    exit 1
fi

# Step 3: Check required packages
MISSING=()
while IFS= read -r line || [[ -n "$line" ]]; do
    PKG=$(echo $line | cut -d '=' -f 1)
    if ! pip show "$PKG" &> /dev/null; then
        MISSING+=("$line")
    fi
done < "$REQUIREMENTS_FILE"

if [ ${#MISSING[@]} -eq 0 ]; then
    echo "✅ All Python packages are already installed."
else
    echo "⚠️  Missing packages:"
    for pkg in "${MISSING[@]}"; do
        echo "   - $pkg"
    done
    read -p "➡️  Do you want to install missing packages now? (y/n): " choice
    if [[ "$choice" == "y" || "$choice" == "Y" ]]; then
        pip install -r "$REQUIREMENTS_FILE"
    else
        echo "❌ Cannot proceed without required packages. Exiting."
        exit 1
    fi
fi

# Step 4: Run the training script
echo "🚀 Running training script..."
python3 "$TRAIN_SCRIPT"
