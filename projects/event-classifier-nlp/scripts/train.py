# scripts/train.py
from datasets import load_dataset, ClassLabel
from transformers import AutoTokenizer, AutoModelForSequenceClassification, TrainingArguments, Trainer, pipeline
import numpy as np
import os

# Load sample data
dataset = load_dataset('json', data_files='data/sample_data.json')

# Map string labels to numbers
unique_labels = list(set(example['label'] for example in dataset['train']))
label2id = {label: idx for idx, label in enumerate(unique_labels)}
id2label = {v: k for k, v in label2id.items()}

# Encode labels
def encode_labels(example):
    example['label'] = label2id[example['label']]
    return example

dataset = dataset.map(encode_labels)

# Load tokenizer
tokenizer = AutoTokenizer.from_pretrained("distilbert-base-uncased")

# Tokenize texts
def tokenize(example):
    return tokenizer(example['text'], truncation=True, padding="max_length")

tokenized_dataset = dataset.map(tokenize, batched=True)

# Load model
model = AutoModelForSequenceClassification.from_pretrained(
    "distilbert-base-uncased",
    num_labels=len(label2id),
    id2label=id2label,
    label2id=label2id
)

# Training args
training_args = TrainingArguments(
    output_dir="models", # Save inside ./models
    evaluation_strategy="no",
    per_device_train_batch_size=8,
    num_train_epochs=3,
    save_strategy="epoch"
)

# Train
trainer = Trainer(
    model=model,
    args=training_args,
    train_dataset=tokenized_dataset['train']
)

# Start training
trainer.train()

# Save model and tokenizer manually after training
trainer.save_model("models")
tokenizer.save_pretrained("models")

# Run validation prediction
print("\n📊 Running test prediction to validate trained model...")

# Use the pipeline to load and test the trained model
classifier = pipeline("text-classification", model="models")

# Sample input text for validation
test_input = "Microsoft announces major stock buyback."
result = classifier(test_input)[0]

# Output prediction
print(f"Input: {test_input}")
print(f"Predicted Label: {result['label']} (Confidence: {result['score']:.2f})")
