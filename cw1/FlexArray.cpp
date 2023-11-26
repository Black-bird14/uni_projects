#include "FlexArray.h"
#include <iostream>
#include <string>
using namespace std;


FlexArray::FlexArray() {
	//Default constructor, initializes flexarray attributes to default values

	size_=0;
	capacity_=INITIALCAP;
	arr_=new int[capacity_];
	headroom_= capacity_/2;
	tailroom_= capacity_/2;
}

FlexArray::FlexArray(const int* arr, int size) {
	//Parametrized constructor, initializes flexarray attributes to input values

	size_=size;
	headroom_= size_;
	tailroom_= size_;
	capacity_=LO_THRESHOLD*size;
	arr_=new int[capacity_];
	for(int i=0; i<size_; i++){
		arr_[headroom_+i]=arr[i];
	}
}

FlexArray::~FlexArray() {
	//Destructor automatically called when program ends, frees up memory (cleanup)
	delete[] arr_;
}

FlexArray::FlexArray(const FlexArray& other) {
	//Copy constructor that initializes new flexarray by deep copy of the input flexarray
	size_=other.size_;
	capacity_=other.capacity_;
	arr_= new int[capacity_];
	headroom_=other.headroom_;
	tailroom_=other.tailroom_;
    for (int i = 0; i < size_; i++) arr_[headroom_+i] = other.arr_[headroom_+i]; 
}

FlexArray& FlexArray::operator=(const FlexArray& other) {
	//Copy assignment operator that assigns new values to flexarray attributes by
	//deep copy of input flexarray

	if(this!=&other){
		delete [] arr_;
		size_=other.size_;
		capacity_=other.capacity_;
		arr_= new int[capacity_];
		headroom_=other.headroom_;
		tailroom_=other.tailroom_;
		for (int i = 0; i < size_; i++) arr_[headroom_+i] = other.arr_[headroom_+i]; 
		}
		return *this;
	}

int FlexArray::getSize() const {
	//Returns the size of the internal array

	return size_;
}

int FlexArray::getCapacity() const {
	//Returns the capacity of the internal array

	return capacity_;
}

string FlexArray::print() const {
	//Prints all the occupied cells of the internal array
	string print="[";
	for(int i=0; i<size_; i++){
		if(i==size_-1)
			print+=to_string(arr_[headroom_+i]);
		else
			print+=to_string(arr_[headroom_+i])+", ";
	}
	print+="]";
	return print;
}

string FlexArray::printAll() const {
	//Gives a visual representation of the internal array including the headroom and tailroom
	string printa="[";
	for(int i=0; i<capacity_;i++){
		if(i<headroom_||i>=headroom_+size_)
			printa+="X";
		else
			printa+=to_string(arr_[i]);
		
		if(i<capacity_-1)
			printa+=", ";
	}
	printa+="]";
	return printa; 
}

int FlexArray::get(int i) const {
	//Get the element at index i of the internal array
	
	return arr_[i+headroom_]; 
}

bool FlexArray::set(int i, int v) {
	//Set to value v, the internal array cell at index i
	if(i<0||i>size_-1)
		return false;
	else{
	arr_[headroom_+i]=v;
	return true;
	}
}

void FlexArray::push_back(int v) {
	//If empty array
	if(size_==0 && capacity_%2!=0){
		int index=capacity_/2;
		arr_[index]=v;
		size_++;
	}
	//if capacity is even
	else if(size_==0&&capacity_%2==0){
		int index=(size_/2)-1;
		arr_[index]=v;
		size_++;
		headroom_--;
	}
	else{
		//If no more empty space in tailroom, resize THEN push
		if(tailroom_==0){
			resize_and_center_();
			arr_[headroom_+size_]=v;
			size_++;
			tailroom_--;
		}
		else{
			//For normal array
			arr_[headroom_+size_]=v;
			size_++;
			tailroom_--;
		}
}
}

bool FlexArray::pop_back() {
	//Remove the last element of the internal array

	//Return false if the array is empty
	if(size_==0)
		return false;
	else{
		int* tmparr= new int[capacity_];
		for(int i=0; i<size_-1; i++){
			tmparr[headroom_+i]=arr_[headroom_+i];
		}
		tailroom_++;
		size_--;
		delete[] arr_;
		arr_= tmparr;
		//If after popping, capacity is larger than 7*size, resize and recenter the array
		if((capacity_>HI_THRESHOLD*size_)&&size_!=0) resize_and_center_();
		return true;
	}
}

void FlexArray::push_front(int v) {
	//Add a new element at the "head" of the array

	//If empty array
	if(size_==0 && capacity_%2!=0){
		int index=capacity_/2;
		arr_[index]=v;
		size_++;
		
	}
	//if capacity is even
	else if(size_==0&&capacity_%2==0){
		int index=(size_/2)-1;
		arr_[index]=v;
		size_++;
		headroom_--;
	}
	else{
		//If no more empty space in headroom, resize THEN push
		if(headroom_==0){
			resize_and_center_();
			arr_[headroom_-1]=v;
			size_++;
			headroom_--;
		}
		else{
			//For normal array
			arr_[headroom_-1]=v;
			size_++;
			headroom_--;
		}

	}
}

bool FlexArray::pop_front() {
	//Remove the first element of the internal array

	//Return false if the array is empty
	if(size_==0)
		return false;
	else{
		// Remove the "head" of the array

		if(size_>1){
			int* tmparr= new int[capacity_];
			for(int i=1; i<size_; i++){
				tmparr[headroom_+i]=arr_[headroom_+i];
			}
			headroom_++;
			size_--;
			tailroom_=capacity_-(headroom_+size_);
			delete[] arr_;
			arr_= tmparr;
			//If after popping, capacity is larger than 7*size, resize and recenter the array
			if((capacity_>HI_THRESHOLD*size_)&&size_!=0) resize_and_center_();
			return true;
		}
		 else{
			int* tmparr= arr_;
			arr_=new int[capacity_];
			size_--;
			delete[]tmparr;
			headroom_=capacity_/2;
			tailroom_=capacity_/2;
			return true;
		}
		
	}
}


bool FlexArray::insert(int i, int v) {
	//Insert an integer v, at index i of the internal array

	//If index out of bounds, return false
	if(i>=size_+1||i<0) return false;

	else if(headroom_==0 && tailroom_==0){
		//If there is no headroom and tailroom, resize THEN insert
		resize_and_center_();
		if(size_==0){
			//If the array is empty, treat it as insert(0,v)
			if(capacity_%2!=0){
				int index=capacity_/2;
				arr_[index]=v;
				size_++;
				return true;
			}
			//if capacity is even
			else{
				int index=(capacity_/2)-1;
				arr_[index]=v;
				size_++;
				headroom_=index;
				tailroom_=capacity_-index;
				return true;
			}
	}
	

		else if(i==size_-1&&i>1){
			//In case i is the index of last element in internal array, shift right

			for(int j=size_-1;j>=0;j--){
				if(j<i&&j>1)arr_[j+tailroom_+1]=arr_[j+tailroom_];
				else if(j==i){
					int t= arr_[j+tailroom_];
					arr_[j+tailroom_]=v;
					arr_[j+tailroom_+1]=t;
					
				}
				else arr_[j+tailroom_]=arr_[j+tailroom_];
			}
			tailroom_--;
			size_++;
			return true;
		}
		else if (i==size_&&i>0){
			//If i is the same as size_, do not consider it out of bounds and insert at the end of array

			for(int j=0; j<size_+1; j++){
				if(j!=i)arr_[headroom_+j]=arr_[headroom_+j];
				else arr_[headroom_+j]=v;
			}
			size_++;
			tailroom_--;
			return true;
		}
		else{
			if(size_-i<=i){
				//If there are less elements to move on the right, shift right

				for(int j=size_-1;j>=0;j--){
					if(j!=i) arr_[j+headroom_+1]=arr_[j+headroom_];
					
					else if(j==i){
						int t= arr_[j+headroom_];
						arr_[j+headroom_]=v;
						arr_[j+headroom_+1]=t;
						break;
					}
				}
				tailroom_--;
				size_++;
				return true;
			}
			else{
				//In any other case, shift left

				for(int j=0;j<size_;j++){
					if(j!=i) arr_[headroom_-1+j]=arr_[headroom_+j];
					else if(j==i){
						int t= arr_[headroom_+j];
						int u= arr_[headroom_+j+1];
						arr_[headroom_-1+j]=v;
						arr_[headroom_+j]=t;
						arr_[headroom_+j+1]=u;
						j+=2;
						}
				}
				headroom_--;
				size_++;
				return true;
				}
		}
	}
	else if(tailroom_==0&& headroom_!=0){
		//If there is no more tailroom, then shift right

			for(int j=0;j<size_;j++){
				if(j==0){
					arr_[headroom_-1+j]=arr_[headroom_+j];
					headroom_--;
				}
				else if(j<i&&j>0) arr_[headroom_+j]=arr_[headroom_+j+1];
				else if(j==i){
					int t= arr_[headroom_+j+1];
					arr_[headroom_+j]=v;
					arr_[headroom_+j+1]=t;
					}
			}
			size_++;
			return true;
	}
	else if(headroom_==0&&tailroom_!=0){
		//If there is no more headroom, then shift left 

			for(int j=size_-1;j>=0;j--){
				if(j>i) arr_[j+1]=arr_[j];
				
				else if(j==i){
					int t= arr_[j];
					arr_[j]=v;
					arr_[j+1]=t;
				}
			}
			tailroom_--;
			size_++;
			return true;
	}
	else{
		//If empty array, insert(0,v)
		if(size_==0){
			if(capacity_%2!=0){
				int index=capacity_/2;
				arr_[index]=v;
				size_++;
				return true;
			}
			//if capacity is even
			else{
				int index=(capacity_/2)-1;
				arr_[index]=v;
				size_++;
				headroom_=index;
				tailroom_=capacity_-index;
				return true;
			}
		}

		else if(i==size_-1&&i>1){
			//In case i is the index of last element in internal array, shift right

			for(int j=size_-1;j>=0;j--){
				if(j<i&&j>1)arr_[j+tailroom_+1]=arr_[j+tailroom_];
				else if(j==i){
					int t= arr_[j+tailroom_];
					arr_[j+tailroom_]=v;
					arr_[j+tailroom_+1]=t;
				}
			}
			tailroom_--;
			size_++;
			return true;
		}
		else if (i==size_&&i>0){
			//In case the index provided is equal to size_, do not consider it out of bounds

			for(int j=0; j<size_+1; j++){
				if(j!=i)arr_[headroom_+j]=arr_[headroom_+j];
				else arr_[headroom_+j]=v;
			}
			size_++;
			tailroom_--;
			return true;
		}
		
		else{
			if(size_-i<=i){
				//Shift right if there are either less elements on the right OR 
				//same number of elements on both sides

				for(int j=size_-1;j>=0;j--){
					if(j!=i) arr_[j+headroom_+1]=arr_[j+headroom_];
					else if(j==i){
						int t= arr_[j+headroom_];
						arr_[j+headroom_]=v;
						arr_[j+headroom_+1]=t;
						break;
					}
				}
				tailroom_--;
				size_++;
				return true;
			}
			else{
				//Otherwise shift left (i.e. there are less elements to move on the left)

				for(int j=0;j<size_;j++){
					if(j<i) arr_[headroom_-1+j]=arr_[headroom_+j];
					else if(j==i){
						int t= arr_[headroom_+j];
						int u= arr_[headroom_+j+1];
						arr_[headroom_-1+j]=v;
						arr_[headroom_+j]=t;
						arr_[headroom_+j+1]=u;
						j+=2;
						}
					else arr_[headroom_-1+j]=arr_[headroom_+j];
				}
				headroom_--;
				size_++;
				return true;
				}
		}
	}
}

bool FlexArray::erase(int i) {
	//Erase/remove an element from the array

	//If array is empty, return false
	if(size_==0||i<0||i>=size_)
		return false;

	if(size_-i-1>i){
		//shift to the right if there are less elements to move on the right side
		for(int j=i-1;j>=0;j--){
			arr_[headroom_+j+1]=arr_[headroom_+j];
		}
		headroom_++;
		size_--;
		//If after erasing, capacity is larger than 7*size, resize and recenter the array
		if((capacity_>HI_THRESHOLD*size_)&&size_!=0) resize_and_center_();
		return true;
	}
	else if((size_-i)-1<=i){
		//shift FROM the right if both directions involve moving the same number of elements
		//OR if there are less elements to move on the left side

		for(int j=i+1; j<size_;j++){
			arr_[headroom_+j-1]=arr_[headroom_+j];
		}
		tailroom_++;
		size_--;
		if((capacity_>HI_THRESHOLD*size_)&&size_!=0) resize_and_center_();
		return true;
	}
	
	return false; 
}

void FlexArray::resize_and_center_(){
	//resize the array to a capacity 3 times the existing number of element (size_)
	//As well as resetting the headroom and tailroom of the array.
	
	capacity_= LO_THRESHOLD*size_;
	int* tmparr= new int[capacity_];
	for(int i=0; i<size_; i++){
			tmparr[size_+i]=arr_[headroom_+i];
			
		}
	delete[] arr_;
	arr_= tmparr;
	headroom_=size_;
	tailroom_=size_;
}



