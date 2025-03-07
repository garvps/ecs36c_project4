all: test_multimap cfs_sched

test_multimap: test_multimap.cc multimap.h
	g++ -Wall -Werror -std=c++11 -pthread -lgtest -o $@ $<

cfs_sched: cfs_sched.cc
	g++ -Wall -Werror -std=c++11 -o $@ $<

clean:
	-rm -f test_multimap cfs_sched
