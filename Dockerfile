FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV ONEAPI_ROOT=/opt/intel/oneapi

# 1. Install basic development tools
RUN apt-get update && apt-get install -y \
    build-essential curl gnupg lsb-release python3 python3-pip git cmake \
    libx11-dev libxtst6 libnss3 libasound2 software-properties-common && \
    apt-get clean

# 2. Add Intel's latest GPG key
RUN curl -fsSL "https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB" \
    | gpg --dearmor -o /usr/share/keyrings/oneapi-archive-keyring.gpg

# 3. Add Intel's oneAPI APT repository
RUN echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" \
    > /etc/apt/sources.list.d/oneAPI.list

# 4. Install the full Intel oneAPI Base Toolkit (includes DPC++, MKL, TBB, etc.)
RUN apt-get update && apt-get install -y intel-basekit

# 5. Install JupyterLab
RUN pip3 install --no-cache-dir jupyterlab

# 6. Auto-source oneAPI environment on shell login
RUN echo "source /opt/intel/oneapi/setvars.sh --force" >> /root/.bashrc

# 7. Set default shell and workspace
SHELL ["/bin/bash", "-c"]
WORKDIR /workspace

# 8. Expose Jupyter port
EXPOSE 8888

# 9. Launch JupyterLab
CMD jupyter lab --ip=0.0.0.0 --port=8888 --no-browser --allow-root
