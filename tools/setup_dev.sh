#!/bin/bash
set -e

python3 -m venv .venv
source .venv/bin/activate
pip install -r tools/requirements-python.txt
pre-commit install
