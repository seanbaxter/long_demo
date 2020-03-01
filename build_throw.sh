set -x
circle throw.cxx -o libthrow.so
circle catch.cxx -M libthrow.so