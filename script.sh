make clean;
rm *png
rm *dot
make
./test command
dot -Tpng -O graph.dot
