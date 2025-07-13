// #include "set.hpp"

// set::set(searchable_bag &bag_ref):bag(bag_ref){}
// void set::insert(int val){
// 	if(!bag.has(val))
// 		bag.insert(val);
// }
// void set::insert(int *array, int size){
// 	for(int i = 0; i < size; ++i)
// 		insert(array[i]);
// }
// void set::print() const{
// 	bag.print();
// }
// void set::clear(){
// 	bag.clear();
// }
// bool set::has(int val) const{
// 	return (bag.has(val));
// }
// searchable_bag &set::get_bag(){
// 	return bag;
// }

#include "set.hpp"



set::set(searchable_bag &bag_ref):bag(bag_ref){}
void set::insert(int val){
	if (!bag.has(val))
		bag.insert(val);
}
void set::insert(int *array, int size){
	for (int i = 0; i < size; ++i)
		insert(array[i]);
}
void set::print() const{
	return bag.print();
}
void set::clear(){
	return bag.clear();
}
bool set::has(int val) const{
	return bag.has(val);
}
searchable_bag &set::get_bag(){
	return bag;
}

