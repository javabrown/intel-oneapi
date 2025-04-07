#!/bin/bash

# Create main project directory
mkdir -p event-classifier-nlp/{data,models,scripts}

# Create sample training data
cat <<EOF > event-classifier-nlp/data/sample_data.json
[
  {"text": "Apple released its Q2 earnings today.", "label": "Earnings"},
  {"text": "Tesla filed for a stock split.", "label": "Stock Split"},
  {"text": "Google acquired Fitbit for $2.1 billion.", "label": "M&A"}
]
EOF

# Create requirements.txt
cat <<EOF > event-classifier-nlp/requirements.txt
transformers==4.40.1
datasets==2.19.1
torch>=1.10.0
EOF

# Create training script placeholder
cat <<EOF > event-classifier-nlp/scripts/train.py
# TODO: Add training code here (see ChatGPT-provided version)
print("Training script placeholder.")
EOF

# Print success message
echo "✅ Project 'event-classifier-nlp' created successfully."
echo "➡️  Next step: Add training code to scripts/train.py"

