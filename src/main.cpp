#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <regex>
#include <chrono>
#include <cmath>

typedef struct smapTime {
	std::chrono::nanoseconds search;
	std::chrono::nanoseconds insert;
} mapTime;

std::vector<std::string>* createWordsVector(std::string words_str) {
	//Create a vector for words
	std::vector<std::string>* words_vector = new std::vector<std::string>;

	//A regex that allows you to retrieve words with accents
	std::regex word_regex(R"([^\s\[\](){}<>\+\-=\'\",\.;:!?%\$£€µ¤°@\^¨~`\|\&\\\/*]+)", std::regex::ECMAScript);
	std::sregex_iterator words_begin = std::sregex_iterator(words_str.begin(), words_str.end(), word_regex);
	std::sregex_iterator words_end = std::sregex_iterator();

	for(std::sregex_iterator it = words_begin;  it != words_end; ++it) {	//get all word on a line
		words_vector->push_back( (*it).str() );	//Save word in vector
	}

	return words_vector;
}

std::string getFileContent(const std::string path) {
	std::ifstream file;   //File to read
	file.open(path, std::ios_base::in);	//Open in read only

	if (!file.is_open()) {	//Cannot open file
		throw std::ios_base::failure("Impossible to read the file '" + path +"'");
	}
	else if(!file.good()) {
		throw std::ios_base::failure("Error in file  '" + path +"'");
	}
	
	//Create string with content of file
	std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return file_content;
}

void testingMap(std::vector<std::string>* words_vector, std::vector<mapTime> * time_vector) {
	//Create unordered map
	std::unordered_map<std::string, int> * map = new std::unordered_map<std::string, int>;

	std::vector<mapTime>::iterator time_it = time_vector->begin();

	//For each word in vector
	for (std::vector<std::string>::iterator it = words_vector->begin(); it != words_vector->end(); ++it) {
		
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		//Search word in map
		std::unordered_map<std::string, int>::iterator got = map->find(*it);

		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		time_it->search += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

		if(got == map->end()) {	//Not found
			std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
			map->emplace(*it, 1);
			std::chrono::steady_clock::time_point t4 = std::chrono::steady_clock::now();
			time_it->insert +=  std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3);
		}
		else {	//Found
			++got->second;
		}

		++time_it;
	}

	delete map;
}

void makeComplexity(const std::string PATH_IN, const std::string PATH_OUT, const std::size_t NB_TESTS) {
	//Get all the text in a file
	std::string file_content = getFileContent(PATH_IN);

	//Create a vector for words
	std::vector<std::string>* words_vector = createWordsVector(file_content);

	std::vector<mapTime> * time_vector = new std::vector<mapTime>;
	time_vector->assign(words_vector->size(), mapTime { (std::chrono::nanoseconds) 0, (std::chrono::nanoseconds) 0} );


	std::cout << "Test of insertion and search times of " << words_vector->size() << " words in an unordered_map repeated " << NB_TESTS << " times" << std::endl;

	for (size_t i = 0; i < NB_TESTS; ++i) {
		double percent = ((double)i/NB_TESTS)*100;
		if(std::fmod(percent,1) == 0) {
			std::cout << "\r" << percent << "%";
		}

		testingMap(words_vector, time_vector);
	}

	std::cout << "\r100%" << std::endl;

	//ouput file
	std::ofstream output;
	output.open(PATH_OUT, std::ios_base::out);	//Open in write only

	if (!output.is_open()) {	//Cannot open file
		throw std::ios_base::failure("Impossible to write the file  '" + PATH_OUT +"'");
	}

	output << "#Test of insertion and search times of " << words_vector->size() << " words in an unordered_map repeated " << NB_TESTS << " times" << std::endl;

	std::cout << "Calculation of averages..." << std::endl;

	//Print in file results
	for (std::vector<mapTime>::iterator time_it = time_vector->begin(); time_it != time_vector->end(); ++time_it) {
		output << time_it->search.count() / NB_TESTS << " " << time_it->insert.count() / NB_TESTS << std::endl;
	}

	std::cout << "Finish !" << std::endl;

	output.close();

	delete time_vector;
	delete words_vector;
}

void makeTest(const std::string PATH_IN) {
	//Get all the text in a file
	std::string file_content = getFileContent(PATH_IN);

	//Create a vector for words
	std::vector<std::string>* words_vector = createWordsVector(file_content);

	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	std::unordered_map<std::string, int> * map = new std::unordered_map<std::string, int>;

	//For each word in vector
	for (std::vector<std::string>::iterator it = words_vector->begin(); it != words_vector->end(); ++it) {
		
		std::unordered_map<std::string, int>::iterator got = map->find(*it);
		if(got == map->end()) {	//Not found
			map->emplace(*it, 1);
		}
		else {	//Found
			++got->second;
		}
	}

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	long long creation_time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << "Time to create the map: " << creation_time << " microseconds, size: " << map->size() << ", number of words: " << words_vector->size() << std::endl << std::endl; 

	char choice;
	std::string word;
	std::cout << "s - Search word in map" << std::endl << "q - Quit" << std::endl;
	do {
		std::cout << "What do you want to do ?" << std::endl << " > ";
		std::cin >> choice;

		if (choice == 's') {
			std::cout << "The word you are searching for > ";
			std::cin >> word;

			t1 = std::chrono::steady_clock::now();
			//Search word in map
			std::unordered_map<std::string, int>::iterator got = map->find(word);
			t2 = std::chrono::steady_clock::now();

			if(got != map->end()) {	//Found
				std::cout << "Word '" << got->first << "' appears " << got->second << " times in the file" << std::endl;
			}
			else {
				std::cout << "Word '" << word << "' doesn't exist in the file" << std::endl;
			}

			std::cout << "Search time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << " nanoseconds" << std::endl;

		}
		else if(choice != 'q') {
			std::cerr << "invalid choice" << std::endl;
			std::cerr << "s - Search word in map" << std::endl << "q - Quit" << std::endl;
		
		}

		std::cout << std::endl;

	} while (choice != 'q');

	delete map;
	delete words_vector;
}

void printHelp(std::string cmd) {
		std::cout << "This program includes 2 different functions, one to show the use of a map and the other to prove the complexity (time) of a map" << std::endl;
		std::cout << "To see how it works do: `" << cmd << " {--search, -s} <path_to_input_file>`" << std::endl;
		std::cout << "To check complexity do: `" << cmd << " {--complexity, -c} <path_to_input_file> <path_to_output_file> <nb_tests>`" << std::endl;
}

int main(int argc, char* argv[]) {
	if(argc >= 3) {
		std::string type = argv[1];
		const std::string PATH_IN = argv[2];

		if(type == "--complexity" || type == "-c") {
			if(argc != 5) {
				std::cerr << "Usage : " << argv[0] << " " << argv[1] << " <path_to_input_file> <path_to_output_file> <nb_tests>" << std::endl;
				return EXIT_FAILURE;
			}
			else {
				const std::string PATH_OUT = argv[3];
				const std::size_t NB_TESTS = (size_t) atoi(argv[4]);

				try {
					makeComplexity(PATH_IN, PATH_OUT, NB_TESTS);
				} catch(const std::ios_base::failure& e) {
					std::cerr << e.what() << std::endl;
				}
			}

		}
		else if(type == "--search" || type == "-s") {
			try {
					makeTest(PATH_IN);
				} catch(const std::ios_base::failure& e) {
					std::cerr << e.what() << std::endl;
				}
		}
		else {
			printHelp(argv[0]);
		}
	}
	else {
		printHelp(argv[0]);
	}

	return EXIT_SUCCESS;
}