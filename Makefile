all: zap unzap test_pqueue test_bstream

zap: zap.cc huffman.h pqueue.h bstream.h
	g++ -Wall -Werror -std=c++17 -o zap zap.cc

unzap: unzap.cc huffman.h
	g++ -Wall -Werror -std=c++17 -o unzap unzap.cc

test_pqueue: test_pqueue.cc pqueue.h
	g++ -Wall -Werror -std=c++17 -o test_pqueue test_pqueue.cc -pthread -lgtest

test_bstream: test_bstream.cc bstream.h
	g++ -Wall -Werror -std=c++17 -o test_bstream test_bstream.cc -pthread -lgtest

clean:
	rm -f unzap zap test_pqueue test_bstream
	rm -f *.zap *.unzap
