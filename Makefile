CFLAGS += -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wfloat-equal -Wvla --std=c11
ARFLAGS += -U
VOPTS = --leak-check=full --show-leak-kinds=all --error-exitcode=1 -q

.PHONY: all
all: dsa.a util.a

dsa.a: dsa.a(btree/btree.o graph/adjacency_list.o graph/adjacency_matrix.o heaps/arr_heap.o heaps/tree_heap.o list/arraylist.o list/singly_linked_list.o map/hashmap.o queue/circ_queue.o set/hashtable.o spatial/kdtree.o spatial/pr_quadtree.o stack/stack.o problems/prime.o graph/traversal.o sorting/bubble_sort.o sorting/merge_sort.o sorting/heap_sort.o sorting/counting_sort.o)

util.a: util.a(util/array_lookup.o util/arrays.o util/comparison_helpers.o util/destroy_helpers.o util/interval_search.o util/kvpair.o util/printing.o util/swapping.o sorting/bubble_sort.o spatial/point.o)

.PHONY: debug
debug: CFLAGS += -g -gstabs -O0
debug: all

.SUFFIXES: .trun .test

.c.test:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -g -o $* $< $@.c dsa.a util.a -lm $(LDLIBS)

.test.trun:
	valgrind $(VOPTS) ./$*

check: $(subst .test.c,.trun,$(wildcard */*.test.c))

.PHONY: clean
clean:
	$(RM) dsa.a util.a
	find . -type f -perm /111 -exec rm -f {} \;
