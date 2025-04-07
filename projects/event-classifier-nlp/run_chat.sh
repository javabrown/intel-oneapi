#!/bin/bash

# ---------------------
# Chat App Runner Script
# ---------------------

SCRIPT="scripts/chat.py"

# Check Python
if ! command -v python3 &> /dev/null; then
    echo "Python3 is not installed. Please install Python 3.7 or higher."
    exit 1
fi

# Check model folder
if [ ! -d "models" ]; then
    echo "Trained model not found in ./models."
    echo "➡️  Run ./run_train.sh to train your model first."
    exit 1
fi

# Run the chat interface
echo "🟢 Starting Chat App..."
python3 "$SCRIPT"
