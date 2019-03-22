rm *dot
rm *png
make clean

if [ $# -ne 0 ]; then
echo "cleaning done"
exit
fi

cmake ..
make
./42sh --ast-print command
dot -Tpng -O graph.dot
eog graph.dot.png
