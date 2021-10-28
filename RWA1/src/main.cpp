#include <iostream>
#include <vector>
#include <string>

// Function Prototypes
unsigned int get_total_parts();
void get_total_boxes();
void get_part_per_box();
void fill_up_boxes();


// Data Structures
unsigned int total_parts;
std::vector<int> boxes;				// Initializing vector for 4 different size boxes
std::vector<int> PartPerBox;		// Initializing vector for parts per box


// Function definitions

/**
 * @brief Get the total parts object
 * 
 * @return unsigned int 
 */
unsigned int get_total_parts() {

	int total = total_parts;
	// Taking input for total number of parts
	std::cout << "How many parts in total?:";
	std::cin >> total;

	// Checking whether input is positive and reprompt the user for invalid input
	while (total < 0 || total==0) {
		std::cout << "Please enter the valid input!" << std::endl;
		std::cout << "How many parts in total?:";
		std::cin >> total;
	}
	unsigned int total_parts1 = total;
	return total_parts1;
}

/**
 * @brief Get the total boxes object
 * 
 */
void get_total_boxes() {

	int small, med, large, xl;

	// Taking input from the user for boxes of four sizes
	std::cout<<"How many boxes for S/M/L/XL? ";
	std::cin >> small >> med >> large >> xl;

	// Checking whether input is positive and reprompt the user for invalid input
	while (small<0 || med<0 || large<0 || xl<0) {
		std::cout << "Please Enter valid Input" << std::endl;
		std::cout<<"How many boxes for S/M/L/XL? ";
		std::cin >> small >> med >> large >> xl;
	}

	// Adding the respective sizes the vector Boxes
	boxes.push_back(xl);
	boxes.push_back(large);
	boxes.push_back(med);
	boxes.push_back(small);
}

/**
 * @brief Get the part per box object
 * 
 */
void get_part_per_box() {

	int part_small, part_med, part_large, part_xl;

	// Taking user inputs for parts per box
	std::cout<<"How many parts per box for S/M/L/XL? ";
	std::cin >> part_small >> part_med >> part_large >> part_xl;
	while((part_small<0 || part_med<0 || part_large<0 || part_xl<0) || (part_small > part_med || part_small > part_large || part_small > part_xl) || (part_med > part_large || part_med > part_xl) || (part_large > part_xl)) {
		std::cout << "Please Enter valid Input" << std::endl;
		std::cout<<"How many parts per box for S/M/L/XL? ";
		std::cin >> part_small >> part_med >> part_large >> part_xl;
	}
	PartPerBox.push_back(part_xl);
	PartPerBox.push_back(part_large);
	PartPerBox.push_back(part_med);
	PartPerBox.push_back(part_small);
}

/**
 * @brief To fill the boxes according to above constraints and displays results
 * 
 */
void fill_up_boxes() {
	std::string boxName[]{"XL ", "L ", "M ", "S "};
	std:: cout << "Boxes that can be built with " << total_parts << " pegs" << std::endl;
	std:: cout << "-------------------------------------------------" << std::endl;
	for (int i=0; i<4; i++) {
		int unused = total_parts / PartPerBox[i];
		if(unused >= boxes[i]){
            total_parts = total_parts - (boxes[i] * PartPerBox[i]);
			std::cout<<boxName[i] << "box (" << boxes[i] << " x " << PartPerBox[i] << " max) : " << boxes[i] << " -- remaining parts: " << total_parts << std::endl;
		}
		else if(unused == 0){
			std::cout<<boxName[i] << "box (" << boxes[i] << " x " << PartPerBox[i] << " max) : " << unused << " -- remaining parts: " << total_parts << std::endl;
		}
		else {
			total_parts = total_parts - (unused * PartPerBox[i]);
			std::cout<<boxName[i] << "box (" << boxes[i] << " x " << PartPerBox[i] << " max) : " << unused << " -- remaining parts: " << total_parts << std::endl;
		}
	}
	std::cout << "parts not in boxes: " << total_parts << std::endl; 	
}

/**
 * @brief Main function
 * 
 * @return int 
 */
int main()
{
	// call function to get total number of parts
	total_parts = get_total_parts();

	// call function to get total number of boxes of each type
	get_total_boxes();

	// call function to get the max number of parts per box type
	get_part_per_box();

	// call function to fill up boxes and to display result
	fill_up_boxes();
}