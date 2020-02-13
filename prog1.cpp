#include <iostream>
#include <list>
#include <vector>
#include <array>
#include <iterator>

const int table_length = 43;

int calculate_hash_value(int num){
	int hash_value = num % table_length;
	if (hash_value < 0){
		hash_value = hash_value + table_length;
	}
	return hash_value;
}

auto find(int num, std::vector<std::array<int, 2>> ** table){
	struct result {int return_value; std::array<int, 2> * pointer;};
	int hash_value = calculate_hash_value(num);
	std::vector<std::array<int, 2>> :: iterator it;
	for (it = table[hash_value]->begin(); it != table[hash_value]->end(); ++it){
		if ((*it)[0] == num){
			std::cout << "Found.\n";
			return result {1, &(*it)};
		}
	}
	std::cout << "Not found.\n";
	return result {0, &(*it)};
}

void insert(int num, std::vector<std::array<int, 2>> ** table){
	auto [found, pointer] = find(num, table);
	if (found == 0){
		int hash_value = calculate_hash_value(num);
		static std::array<int, 2> my_array;
		std::array<int, 2> * my_array_pointer = &my_array;
		my_array[0] = num;
		my_array[1] = 1;
		std::vector<std::array<int, 2>> :: iterator it = table[hash_value]->begin();
	    table[hash_value]->insert(it, my_array);
	}
	else {
		(*pointer)[1] = (*pointer)[1]+1;
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
	insert(300+table_length, hash_table_pointer);
	find(300, hash_table_pointer);
	find(300+table_length, hash_table_pointer);
	return 0;
}