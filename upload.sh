#!/bin/bash
FILE=.piovenv/Scripts/activate
if [ -f "$FILE" ]; then
    echo "$FILE exists."
    cd .piovenv/Scripts/
    . activate
else 
    echo "$FILE does not exist, installing virtual environment"
    python -m venv .piovenv
    cd .piovenv/Scripts/
    . activate
    python -m pip install platformio
fi

cd ../..
pio run -t upload

echo "Press Enter to exit"
read