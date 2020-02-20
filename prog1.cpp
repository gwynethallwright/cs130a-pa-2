#include <iostream>
#include <list>
#include <vector>
#include <array>
#include <iterator>

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

void percolate_up(int i, std::vector<int> * heap_array){
	if (heap_array[get_parent_index(i)] > heap_array[i]){
		swap(heap_array[i], heap_array[get_parent_index(i)]);
		percolate_up(get_parent_index(i), heap_array);
	}
}

void percolate_down(int i, std::vector<int> * heap_array){
	int left = get_left_index(i);
	int right = get_right_index(i);
	int small_index = i;
	if (heap_array[left] < heap_array[i]){
		small_index = left;
	}
	if (heap_array[right] < heap_array[small_index]){
		small_index = right;
	}
	if (small_index != i){
		swap(heap_array[i], heap_array[small_index]);
		percolate_down(small_index, heap_array);
	}
}

void delete_min(std::vector<int> * heap_array){
	if (heap_array->size() != 0){
		heap_array->front() = heap_array->back();
		heap_array->pop_back();
		percolate_down(0, heap_array);
	}
}

void insert_heap(int to_insert, std::vector<int> * heap_array){
	heap_array->push_back(to_insert);
	int new_index = heap_array->size()-1;
	percolate_up(new_index, heap_array);
}

void print_heap(std::vector<int> * heap_array){
	std::vector<int>::const_iterator it;
	for(it = heap_array->begin(); it != heap_array->end(); ++it){
		std::cout << (*it) << " ";
    }
    std::cout << "\n";
}

int calculate_hash_value(int num){
	int hash_value = num % table_length;
	if (hash_value < 0){
		hash_value = hash_value + table_length;
	}
	return hash_value;
}

auto find(int num, std::vector<std::array<int, 2>> ** table, int suppress_output){
	struct result {int return_value; std::vector<std::array<int, 2> > :: iterator pointer;};
	int hash_value = calculate_hash_value(num);
	std::vector<std::array<int, 2>> :: iterator it;
	for (it = table[hash_value]->begin(); it != table[hash_value]->end(); ++it){
		if ((*it)[0] == num){
			if (suppress_output == 0){
				std::cout << "item found, count = " << (*it)[1] << "\n";
			}
			return result {1, it};
		}
	}
	if (suppress_output == 0){
		std::cout << "item not found.\n";
	}
	return result {0, it};
}

void insert(int num, std::vector<std::array<int, 2>> ** table){
	auto [found, iterator] = find(num, table, 1);
	std::array<int, 2> * pointer = &(*iterator);
	if (found == 0){
		int hash_value = calculate_hash_value(num);
		static std::array<int, 2> my_array;
		std::array<int, 2> * my_array_pointer = &my_array;
		my_array[0] = num;
		my_array[1] = 1;
		std::vector<std::array<int, 2>> :: iterator it = table[hash_value]->begin();
	    table[hash_value]->insert(it, my_array);
	    std::cout << "item successfully inserted, count = 1\n"; 
	}
	else {
		(*pointer)[1] = (*pointer)[1]+1;
		std::cout << "item already present, count = " << (*pointer)[1] << "\n";
	}
}

void delete_item(int num, std::vector<std::array<int, 2>> ** table){
	auto [found, pointer] = find(num, table, 1);
	if (found == 1){
		if ((*pointer)[1] == 1){
			table[calculate_hash_value(num)]->erase(pointer);
		}
		else {
			(*pointer)[1] = (*pointer)[1]-1;
		}
	}
}

std::vector<std::array<int, 2>> ** create_hash_table(){
	static std::vector<std::array<int, 2>> * hash_table [table_length];
	std::vector<std::array<int, 2>> ** hash_table_pointer;
	hash_table_pointer = hash_table;
	for (int i = 0; i < table_length; ++i){
		static std::vector<std::array<int, 2>> my_list;
		std::vector<std::array<int, 2>> * my_list_pointer = &my_list;
		hash_table[i] = my_list_pointer;
	}
	return hash_table_pointer;
}

int main(int argc, char** argv){
	std::vector<std::array<int, 2>> ** hash_table_pointer = create_hash_table();
	insert(300, hash_table_pointer);
	insert(300, hash_table_pointer);
	insert(300+table_length, hash_table_pointer);
	find(300, hash_table_pointer, 0);
	find(300+table_length, hash_table_pointer, 0);
	delete_item(300, hash_table_pointer);
	find(300, hash_table_pointer, 0);
	delete_item(300, hash_table_pointer);
	find(300, hash_table_pointer, 0);
	delete_item(300, hash_table_pointer);
	find(300, hash_table_pointer, 0);
	std::vector<int> heap_array;
	insert_heap(50, &heap_array);
	print_heap(&heap_array);
	return 0;
}