cd lua
make linux
make install
cd ../univ
make 
make install
cp sloong.conf /etc/ld.so.conf.d/sloong.conf
ldconfig