#include <iostream>
#include <chrono>
#include "huffman.h"
using namespace std;

int main(int argc, char *argv[])
{
	//if (argc != 3)
	//{
	//	cout << "Usage:\n\t huffmanCoding inputfile outputfile" << endl;
	//	exit(1);
	//}
	//huffman h(argv[1], argv[2]);
	auto start_time = std::chrono::high_resolution_clock::now();
	huffman h("lena_256.bmp", "lena_256666.bin");
	//h.create_pq();
	h.readGrayBMP();
	h.create_huffman_tree();
	h.calculate_huffman_codes();
	h.coding_save();
	h.decoding_save();
	cout << endl << "endl";
	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
	std::cout << "Execution time: " << duration_sec.count() << " s (seconds)" << std::endl;
	return 0;
}