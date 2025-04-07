# scripts/predict.py

from transformers import pipeline

# Path to your trained model (from train.py)
model_path = "models"

# Load the model into a text classification pipeline
classifier = pipeline("text-classification", model=model_path)

# Test input (you can change this anytime)
sample_input = "Microsoft announces major stock buyback."

# Get prediction
result = classifier(sample_input)[0]
print(f"Input Text: {sample_input}")
print(f"Predicted Label: {result['label']} (Confidence: {result['score']:.2f})")
