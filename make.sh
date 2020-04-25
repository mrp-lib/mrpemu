rm -rf bin/mrpemu* .build/win32/*.o .build/xxnux/*.o

# make DEBUG=TRUE
# make SHARED=TRUE

make SHARED=TRUE ARCH=WIN32
# make DEBUG=TRUE ARCH=WIN32

cp -f ./bin/mrpemu.dll /mnt/c/develop/qt/build-mrpemuv-Desktop_Qt_5_13_2_MinGW_32_bit-Release/release/mrpemu.dll
cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp sdcard/mythroad/
cp -f /mnt/c/vmshare/proj/hello/hello/bin/*.mrp /mnt/c/develop/qt/build-mrpemuv-Desktop_Qt_5_13_2_MinGW_32_bit-Release/sdcard/mythroad/