# rm -rf bin/mrpemu* .build/*.o && make DEBUG=TRUE
# rm -rf .build/mrp* && make SHARED=TRUE

rm -rf .build/mrp* && make SHARED=TRUE ARCH=WIN32
# rm -rf bin/mrpemu* .build/*.o && make DEBUG=TRUE ARCH=WIN32