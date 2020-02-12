#include <list>

void insert(int num, int* table){
	int hash_value = num%43;
	if (hash_value < 0){
		hash_value = hash_value + 43;
	}
}

int main(int argc, char** argv){
std::list <int> * hash_table [43];
std::list <int> ** hash_table_pointer;
hash_table_pointer = hash_table;
for (int i = 0; i < 43; ++i){
	std::list <int> my_list;
	std::list <int> * my_list_pointer = &my_list;
	hash_table[i] = my_list_pointer;
}
return 0;
}