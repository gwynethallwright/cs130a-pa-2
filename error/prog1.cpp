#include <iostream>
#include <vector>
#include <array>
#include <iterator>
#include <list>
#include <memory>

static std::vector<std::array<int, 2>*> heap_array;

void insert_into_heap(int num){
    std::array<int, 2> * test = (std::array<int, 2> *) malloc(sizeof(std::array<int, 2>));
	(*test)[0] = num;
	heap_array.push_back(test);
}

void print_heap(){
	static std::vector<std::array<int, 2>*>::const_iterator it;
	for(it = heap_array.begin(); it != heap_array.end(); ++it){
		std::cout << (*(*it))[0] << " ";
    }
    std::cout << "\n";
}

int main(int argc, char** argv){
	static std::vector< std::list<std::array<int, 2>>::iterator > heap_array;
	insert_into_heap(121);
	print_heap();
	insert_into_heap(-500);
	print_heap();
	insert_into_heap(30);
	print_heap();
	return 0;
}
