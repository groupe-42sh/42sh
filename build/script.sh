rm *png
rm *dot
make clean
if [ "$#" -eq 1 ]; then
    echo "end cleaning";
    exit
fi
cmake ..
make
./42sh --ast-print command
dot -Tpng -O graph.dot
eog graph.dot.png
