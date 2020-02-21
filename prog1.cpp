#include <iostream>
#include <list>
#include <vector>
#include <array>
#include <iterator>

/* vector of iterators! Each iterator points to a list entry. std::list<std::array<int, 2>>::iterator */
static std::vector< std::list<std::array<int, 2>>::iterator > heap_array;

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
		percolate_up(get_parent_index(i));
	}
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
		percolate_down(small_index);
	}
}

int calculate_hash_value(int num){
	int hash_value = num % table_length;
	if (hash_value < 0){
		hash_value = hash_value + table_length;
	}
	return hash_value;
}

void delete_min(std::list<std::array<int, 2>> ** table){
	if (heap_array.size() != 0){
		if ((*(heap_array[0]))[1] == 1){
			int num = (*(heap_array[0]))[0];
			int hash = calculate_hash_value(num);
			/*table[hash]->erase(heap_array[0]);*/
		}
		else {
			/*(*(heap_array[0]))[1] = (*(heap_array[0]))[1]-1;*/
		}
		heap_array[0] = heap_array.back();
		heap_array.pop_back();
		percolate_down(0);
	}
}

void insert_heap(std::list<std::array<int, 2>>::iterator to_insert){
	heap_array.push_back(to_insert);
	int new_index = heap_array.size()-1;
	percolate_up(new_index);
}

void print_heap(){
	static std::vector<std::list<std::array<int, 2>>::iterator>::iterator it;
	for(it = heap_array.begin(); it != heap_array.end(); ++it){
		std::cout << (*(*it))[0] << " ";
    }
    std::cout << "\n";
}

auto find(int num, std::list<std::array<int, 2>> ** table, int suppress_output){
	struct result {int return_value; std::list<std::array<int, 2> > :: iterator pointer;};
	int hash_value = calculate_hash_value(num);
	std::list<std::array<int, 2>> :: iterator it;
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

void insert(int num, std::list<std::array<int, 2>> ** table){
	auto [found, ite] = find(num, table, 1);
	std::array<int, 2> * pointer = &(*ite);
	if (found == 0){
		int hash_value = calculate_hash_value(num);

		std::array<int, 2> * my_array = (std::array<int, 2> *) malloc(sizeof(std::array<int, 2>));
		(*my_array)[0] = num;
		(*my_array)[1] = 1;
		std::list<std::array<int, 2>> :: iterator it = table[hash_value]->begin();
	    table[hash_value]->insert(it, (*my_array));

	    insert_heap(it);
	    std::cout << "item successfully inserted, count = 1\n"; 
	}
	else {
		(*pointer)[1] = (*pointer)[1]+1;
		insert_heap(ite);
		std::cout << "item already present, count = " << (*pointer)[1] << "\n";
	}
}

void delete_item(int num, std::list<std::array<int, 2>> ** table){
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

std::list<std::array<int, 2>> ** create_hash_table(){
	static std::list<std::array<int, 2>> * hash_table [table_length];
	std::list<std::array<int, 2>> ** hash_table_pointer;
	hash_table_pointer = hash_table;
	for (int i = 0; i < table_length; ++i){
		static std::list<std::array<int, 2>> my_list;
		std::list<std::array<int, 2>> * my_list_pointer = &my_list;
		hash_table[i] = my_list_pointer;
	}
	return hash_table_pointer;
}

int main(int argc, char** argv){
	static std::list<std::array<int, 2>> ** hash_table_pointer = create_hash_table();

	insert(300, hash_table_pointer);
	print_heap();
	insert(300, hash_table_pointer);
	print_heap();
	insert(-51, hash_table_pointer);
	print_heap();
	find(300, hash_table_pointer, 0);
	print_heap();
	insert(-54, hash_table_pointer);
	print_heap();
	insert(-600, hash_table_pointer);
	print_heap();
	insert(-601, hash_table_pointer);
	print_heap();
	insert(-602, hash_table_pointer);
	print_heap();
	insert(-603, hash_table_pointer);
	print_heap();
	return 0;
}