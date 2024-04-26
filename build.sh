rm -r src/*
rm -r build/*
mkdir src
mkdir build
asn1c rrc.asn1 -D src -pdu=auto -no-gen-OER -fcompound-names -no-gen-example -fno-include-deps
gcc -g -Isrc -I. -o build/rrc main.c request.c complete.c src/*.c -lsctp -DPDU=auto -DASN_DISABLE_OER_SUPPORT
./build/rrc