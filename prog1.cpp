#include <iostream>
#include <list>
#include <iterator>

const int table_length = 43;

int calculate_hash_value(int num){
	int hash_value = num % table_length;
	if (hash_value < 0){
		hash_value = hash_value + table_length;
	}
	return hash_value;
}

void insert(int num, std::list <int> ** table){
	int hash_value = calculate_hash_value(num);
	table[hash_value]->push_front(num);
}

void find(int num, std::list <int> ** table){
	int hash_value = calculate_hash_value(num);
	std::list <int> :: iterator it;
	for (it = table[hash_value]->begin(); it != table[hash_value]->end(); ++it){
		if (*it == num){
			std::cout << "Found.\n";
		}
		else{
			std::cout << "Not found.\n";
		}
	}
}

int main(int argc, char** argv){
std::list <int> * hash_table [table_length];
std::list <int> ** hash_table_pointer;
hash_table_pointer = hash_table;
for (int i = 0; i < table_length; ++i){
	std::list <int> my_list;
	std::list <int> * my_list_pointer = &my_list;
	hash_table[i] = my_list_pointer;
}
insert(300, hash_table_pointer);
find(300, hash_table_pointer);
return 0;
}