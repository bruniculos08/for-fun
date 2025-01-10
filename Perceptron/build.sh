#! \bin\bash
if [ "$1" = "all" ]; then
    echo "compiling and executing...";
    gcc perceptron.c -Wall -lm -o perceptron && ./perceptron;
else
    echo "compiling only...";
    gcc perceptron.c -Wall -lm -o perceptron
fi
# rm perceptron