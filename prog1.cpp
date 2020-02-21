#include <iostream>
#include <list>
#include <vector>
#include <array>
#include <iterator>
#include <sstream>
#include <string>

/* vector of iterators! Each iterator points to a list entry. std::list<std::array<int, 3>>::iterator */
static std::vector< std::list<std::array<int, 3>>::iterator > heap_array;

const int table_length = 43;

int get_parent_index(int i){
	return (i-1)/2;
}

int get_left_index(int i){
	return (2*i+1);
}

int get_right_index(int i){
	return (2*i+2);
}

void percolate_up(int i){
	if (i && (*(heap_array[get_parent_index(i)]))[0] > (*heap_array[i])[0]){
		std::swap(heap_array[i], heap_array[get_parent_index(i)]);
		(*heap_array[i])[2] = i;
		percolate_up(get_parent_index(i));
	}
	(*heap_array[i])[2] = i;
}

void percolate_down(int i){
	int left = get_left_index(i);
	int right = get_right_index(i);
	int small_index = i;
	if (left < heap_array.size() && (*(heap_array[left]))[0] < (*(heap_array[i]))[0]){
		small_index = left;
	}
	if (right < heap_array.size() && (*(heap_array[right]))[0] < (*(heap_array[small_index]))[0]){
		small_index = right;
	}
	if (small_index != i){
		std::swap(heap_array[i], heap_array[small_index]);
		(*heap_array[i])[2] = i;
		percolate_down(small_index);
	}
}

void heapify(int i){

	int parent = get_parent_index(i);
	if ( i == 0 || ((*(heap_array[parent]))[0] > ((*(heap_array[i]))[0] ))) {
		percolate_down(i);
	}
	else {
		percolate_up(i);
	}
}

int calculate_hash_value(int num){
	int hash_value = num % table_length;
	if (hash_value < 0){
		hash_value = hash_value + table_length;
	}
	return hash_value;
}

void delete_min(std::list<std::array<int, 3>> ** table){
	if (heap_array.size() != 0){
		if ((*(heap_array[0]))[1] == 1){
			int num = (*(heap_array[0]))[0];
			int hash = calculate_hash_value(num);
			table[hash]->erase(heap_array[0]);
			std::cout << "min item " << (*(heap_array[0]))[0] << " successfully deleted\n";
			heap_array[0] = heap_array.back();
			heap_array.pop_back();
			percolate_down(0);
		}
		else {
			(*(heap_array[0]))[1] = (*(heap_array[0]))[1]-1;
			std::cout << "min item " << (*(heap_array[0]))[0] << " successfully deleted\n";
		}
	}
}

void insert_heap(std::list<std::array<int, 3>>::iterator to_insert){
	heap_array.push_back(to_insert);
	int new_index = heap_array.size()-1;
	percolate_up(new_index);
}

void print_heap(){
	static std::vector< std::list<std::array<int, 3>>::iterator >::iterator it;
	for(it = heap_array.begin(); it != heap_array.end(); ++it){
		std::cout << (*(*it))[0] << " ";
    }
    std::cout << "\n";

	for(it = heap_array.begin(); it != heap_array.end(); ++it){
		std::cout << (*(*it))[2] << " ";
    }
    std::cout << "\n";
}

auto find(int num, std::list<std::array<int, 3>> ** table, int suppress_output){
	struct result {int return_value; std::list<std::array<int, 3> > :: iterator pointer;};
	int hash_value = calculate_hash_value(num);
	std::list<std::array<int, 3>> :: iterator it;
	for (it = table[hash_value]->begin(); it != table[hash_value]->end(); ++it){
		if ((*it)[0] == num){
			if (suppress_output == 0){
				std::cout << "item found, count = " << (*it)[1] << "\n";
			}
			return result {1, it};
		}
	}
	if (suppress_output == 0){
		std::cout << "item not found\n";
	}
	return result {0, it};
}

void insert(int num, std::list<std::array<int, 3>> ** table){
	auto [found, ite] = find(num, table, 1);
	std::array<int, 3> * pointer = &(*ite);
	if (found == 0){
		int hash_value = calculate_hash_value(num);

		std::array<int, 3> * my_array = (std::array<int, 3> *) malloc(sizeof(std::array<int, 3>));
		(*my_array)[0] = num;
		(*my_array)[1] = 1;

		std::list<std::array<int, 3>> :: iterator it = table[hash_value]->begin();
	    table[hash_value]->insert(it, (*my_array));

		std::list<std::array<int, 3>> :: iterator it_2 = table[hash_value]->begin();

	    insert_heap(it_2);

	    std::cout << "item successfully inserted, count = 1\n"; 
	}
	else {
		(*pointer)[1] = (*pointer)[1]+1;
		/*(*pointer)[2] = insert_heap(ite);*/
		std::cout << "item already present, count = " << (*pointer)[1] << "\n";
	}
}

void delete_item(int num, std::list<std::array<int, 3>> ** table){
	auto [found, pointer] = find(num, table, 1);
	if (found == 1){
		if ((*pointer)[1] == 1){

			std::swap(heap_array[(*pointer)[2]], heap_array.back());
			heap_array.pop_back();
			heapify((*pointer)[2]);

			table[calculate_hash_value(num)]->erase(pointer);
		}
		else {
			(*pointer)[1] = (*pointer)[1]-1;
		}
		std::cout << "item successfully deleted\n";
	}
	else {
		std::cout << "item not present in table\n";
	}
}

std::list<std::array<int, 3>> ** create_hash_table(){
	static std::list<std::array<int, 3>> * hash_table [table_length];
	std::list<std::array<int, 3>> ** hash_table_pointer;
	hash_table_pointer = hash_table;
	for (int i = 0; i < table_length; ++i){
		static std::list<std::array<int, 3>> my_list;
		std::list<std::array<int, 3>> * my_list_pointer = &my_list;
		hash_table[i] = my_list_pointer;
	}
	return hash_table_pointer;
}

int main(int argc, char** argv){
	static std::list<std::array<int, 3>> ** hash_table_pointer = create_hash_table();
	std::string current;
	std::string argument;
	int i = 0;
    while (i != argc){
    	current = argv[i];
    	if (current == "insert"){
    		++i;
    	    argument = argv[i];
    	    if (i != (argc-1)){
    	    	argument.pop_back();
    	    }
    		insert(std::atoi(argument.c_str()), hash_table_pointer);
    	}
    	else if (current == "lookup"){
    		++i;
    	    argument = argv[i];
    	    if (i != (argc-1)){
    	    	argument.pop_back();
    	    }
    		find(std::atoi(argument.c_str()), hash_table_pointer, 0);
    	}
    	else if (current == "delete"){
    		++i;
    	    argument = argv[i];
    	    if (i != (argc-1)){
    	    	argument.pop_back();
    	    }
    		delete_item(std::atoi(argument.c_str()), hash_table_pointer);
    	}
    	else if (current == "print,"){
    		print_heap();
    	}
        else if (current == "deleteMin,"){
    		delete_min(hash_table_pointer);
    	}
    	++i;
    }
	/*
	insert(3, hash_table_pointer);
	print_heap();
	find(302, hash_table_pointer, 0);
	print_heap();
	insert(302, hash_table_pointer);
	print_heap();
	insert(-51, hash_table_pointer);
	print_heap();
	find(300, hash_table_pointer, 0);
	print_heap();
	insert(-54, hash_table_pointer);
	print_heap();
	insert(3, hash_table_pointer);
	print_heap();
	insert(-601, hash_table_pointer);
	print_heap();
	insert(-601, hash_table_pointer);
	print_heap();
	insert(-603, hash_table_pointer);
	print_heap();
	delete_min(hash_table_pointer);
	print_heap();
	find(-603, hash_table_pointer, 0);
	print_heap();
	delete_item(3, hash_table_pointer);
	print_heap();
	find(3, hash_table_pointer, 0);
	delete_item(3, hash_table_pointer);
	print_heap();
	find(3, hash_table_pointer, 0);*/
	return 0;
}