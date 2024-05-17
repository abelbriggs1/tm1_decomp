#!/bin/bash
set -e

git config --global --add safe.directory /workspace
python3 -m virtualenv .venv
source .venv/bin/activate
pip install -r requirements.txt
pre-commit install
