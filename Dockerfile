FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV ONEAPI_ROOT=/opt/intel/oneapi

# Install base tools
RUN apt-get update && apt-get install -y \
    build-essential wget curl gnupg lsb-release \
    python3 python3-pip python3-dev git cmake \
    libx11-dev libxtst6 libnss3 libasound2 \
    software-properties-common && apt-get clean

# Install oneAPI repo
RUN wget https://apt.repos.intel.com/oneapi/intel-oneapi-repo.sh && \
    bash intel-oneapi-repo.sh && \
    apt-get update && \
    apt-get install -y intel-oneapi-dpcpp-compiler intel-oneapi-runtime

# Install JupyterLab
RUN pip3 install jupyterlab

# Set environment
RUN echo "source /opt/intel/oneapi/setvars.sh" >> ~/.bashrc
SHELL ["/bin/bash", "-c"]

WORKDIR /workspace

EXPOSE 8888

CMD jupyter lab --ip=0.0.0.0 --port=8888 --no-browser --allow-root

