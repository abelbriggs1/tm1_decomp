#!/bin/bash
set -e

git config --global --add safe.directory /workspace
python3 -m pip install -r requirements.txt
pre-commit install
