all: lib_freecplus.a lib_freecplus.so libftp.a libftp.so

lib_freecplus.a:_freecplus.h _freecplus.cpp
	g++ -c -o lib_freecplus.a _freecplus.cpp

lib_freecplus.so:_freecplus.h _freecplus.cpp
	g++ -fPIC -shared -o lib_freecplus.so _freecplus.cpp

libftp.a:ftplib.h ftplib.c
	gcc -c -o libftp.a ftplib.c

libftp.so:ftplib.h ftplib.c
	gcc -fPIC -shared -o libftp.so ftplib.c

clean:
	rm -f lib_freecplus.a lib_freecplus.so libftp.a libftp.so
