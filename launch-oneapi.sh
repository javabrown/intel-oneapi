#!/bin/bash

IMAGE_NAME="intel-oneapi"
CONTAINER_NAME="oneapi-dev"
PROJECT_ROOT="$(pwd)"
PORT=8888

echo "Checking if Docker image '$IMAGE_NAME' exists..."
if [[ "$(docker images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
    echo "Image not found. Building image '$IMAGE_NAME'..."
    docker build -t $IMAGE_NAME .
else
    echo "Docker image '$IMAGE_NAME' already exists."
fi

echo "Cleaning up existing container (if any)..."
if docker ps -a | grep -q $CONTAINER_NAME; then
    docker stop $CONTAINER_NAME > /dev/null 2>&1
    docker rm $CONTAINER_NAME > /dev/null 2>&1
    echo "Old container removed."
fi

echo "Launching new container '$CONTAINER_NAME'..."
docker run -it --name $CONTAINER_NAME \
    -p $PORT:8888 \
    -v "$PROJECT_ROOT:/workspace" \
    $IMAGE_NAME

