#include <iostream>
#include <thread>


int main()
{
	// SETUP
	auto func1 = [](int offset) {return offset * offset; };
	auto func2 = [](int offset) {return offset; };
	auto p_ii  = std::make_pair(1, 2);                                          // pair<int, int>
	auto p_id  = std::make_pair(1, 2.0);                                        // pair<int, double>
	auto p_id2 = std::make_pair(1, 2.0);                                        // pair<int, double>
	auto p_id3 = std::make_pair(1, 2.1);                                        // pair<int, double>
	auto p_li1 = std::make_pair([](int offset) {return offset * offset; }, 1);  // pair<lambda(int), int> || pair<main::{lambda(int)#3},  int>
	auto p_li2 = std::make_pair([](int offset) {return offset; }, 1);           // pair<lambda(int), int> || pair<main::{lambda(int)#4},  int>
	auto p_fi1 = std::make_pair(func1, 1);                                      // pair<lambda(int), int> || pair<main::{lambda(int)#1}&, int>
	auto p_fi2 = std::make_pair(func2, 1);                                      // pair<lambda(int), int> || pair<main::{lambda(int)#2}&, int>

	
	// testing & experimenting
	
	auto get1  = std::get<0>(p_ii);      // lea   -> mov -> call -> mov -> mov
	auto get2  = std::get<0>(p_id);      // lea   -> mov -> call -> mov -> mov
	auto get3  = p_id.first;             // mov   -> mov
	auto get4  = std::get<1>(p_id);      // lea   -> mov -> call -> movsd -> movsd
	auto get5  = p_id.second;            // movsd -> movsd
	auto get6  = std::get<0>(p_li1);     // lea   -> mov -> call
	auto get7  = p_li1.first;            // --------------------
	auto get8  = std::get<0>(p_fi1);     // lea   -> mov -> call
	auto get9  = p_fi1.first;            // --------------------
//  only works since c++14
	auto get10 = std::get<double>(p_id); // lea   -> mov -> call -> movsd -> movsd 

	auto res = p_ii.first + p_id.first + std::get<1>(p_id) + p_li1.first(2) + std::get<0>(p_fi1)(2) + p_ii.second;
//                    mov     |     mov    |         |         |        |       |          |            |       |
//                     |      |      |     |         |         |        |       |          |            |       |
//                      `--- add ---´      |         |         |        |       |          |            |       |
//                        cvtsi2sd         |         |         |        |       |          |            |       |
//                          movsd          |         |         |        |       |          |            |       |
//                            |            |        lea        |        |       |          |            |       |
//                            |            |        mov        |        |       |          |            |       |
//                            |            |       call        |        |       |          |            |       |
//                            |            |       movsd       |        |       |          |            |       |
//                             `-------- addsd -----´          |        |       |          |            |       |
//                                       movsd                 |        |       |          |            |       |
//                                         |                   |       lea      |          |            |       |
//                                         |                   |       mov      |          |            |       |
//                                         |                   |       mov      |          |            |       |
//                                         |                   |      call      |          |            |       |
//                                     cvtsi2sd                |        |       |          |            |       |
//                                      movapd                 |        |       |          |            |       |
//                                          `--------------- addsd ----´        |          |            |       |
//                                                           movsd              |          |            |       |
//                                                             |                |         lea           |       |
//                                                             |                |         mov           |       |
//                                                             |                |         call          |       |
//                                                             |                |         mov           |       |
//                                                             |                |         mov           |       |
//                                                             |                |         call          |       |
//                                                         cvtsi2sd             |          |            |       |
//                                                              `------------ addsd ------´             |       |        
//                                                                              |                       |      mov
//                                                                          cvtsi2sd                    |       |
//                                                                               `------------------- addsd ---´
//                                                                                                    movsd



	auto comp1 = (get1 == get2);
	std::cout << comp1 << '\n';
	std::cout << (get1 == get2) << '\n';
	std::cout << (get1 == get3) << '\n';

//************************************************
//***  no error @ godbolt...                   ***
//***  VS + clang: 'operator ==' is ambiguous  ***
//************************************************
//	auto comp2 = (get6 == get7);
//	std::cout << comp2 << '\n';
//
//	auto comp3 = (get7 == get8);
//	std::cout << comp3 << '\n';

	auto comp4 = (p_id == p_id2);  // true
	std::cout << comp4 << '\n';

	auto comp5 = (p_id == p_id3);  // false
	std::cout << comp5 << '\n';

//***************************************************************************
//***  no operator "==" matches these operands                            ***
//***  operand types are : std::pair<int, double> == std::pair<int, int>  ***
//***************************************************************************
//	auto comp6 = (p_id == p_ii);

//*******************************************************************************************************************************************
//*** no match for 'operator==' (operand types are 																						  ***
//*** 'std::pair<main()::<lambda(int)>, int>' and 'std::pair<main()::<lambda(int)>, int>')												  ***
//*** VS gives some better insight:																										  ***
//*** '(std::pair<main::<lambda_00012ae85230793d243031e0a4101699>,int>, std::pair<main::<lambda_3ca0d5e23427693c9c3218cd1926580d>,int>)'  ***
//*******************************************************************************************************************************************
//	auto comp7 = (p_fi2 == p_li2);


}

