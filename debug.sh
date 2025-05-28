#!/bin/sh
 
# Simple script to rename .obj to .o in current directory

# rm *.o
# rm build/kernel/*.o

# make clean
make incl
make hal
make libs
make ddrivers
make kernel
./rename_obj_to_o.sh
make ucx
./rename_obj_to_o.sh
make ucx

mv echo.o build/app
mv build/kernel/echo.o build/app/echo.o
make echo
./rename_obj_to_o.sh
make echo
./rename_obj_to_o.sh