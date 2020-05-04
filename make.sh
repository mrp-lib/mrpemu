rm -rf bin/libmrp* .build/win32/*.o .build/xxnux/*.o

# make TYPE=SHARED
# make DEBUG=TRUE

make TYPE=SHARED ARCH=WIN32
# make DEBUG=TRUE ARCH=WIN32

cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp sdcard/mythroad/
cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp ~/sdcard/mythroad/
cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp /mnt/c/develop/qt/build-test1-Desktop_Qt_5_13_2_MinGW_32_bit-Debug/sdcard/mythroad/
cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp /mnt/c/develop/qt/build-test1-Desktop_Qt_5_13_2_MinGW_32_bit-Release/sdcard/mythroad/
cp -f bin/libmrp.so ~/develop/qt/build-test1-Desktop_Qt_5_14_2_GCC_64bit-Debug/
cp -f bin/libmrp.so ~/develop/qt/build-test1-Desktop_Qt_5_14_2_GCC_64bit-Release/
cp -f ./bin/libmrp.dll /mnt/c/develop/qt/build-test1-Desktop_Qt_5_13_2_MinGW_32_bit-Release/release/libmrp.dll
cp -f ./bin/libmrp.dll /mnt/c/develop/qt/build-test1-Desktop_Qt_5_13_2_MinGW_32_bit-Debug/release/libmrp.dll
# cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp /mnt/c/develop/qt/build-mrpemuv-Desktop_Qt_5_13_2_MinGW_32_bit-Release/sdcard/mythroad/