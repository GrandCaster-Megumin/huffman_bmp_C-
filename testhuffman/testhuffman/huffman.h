#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
#pragma pack(push, 1) // �T�O���c����覡�PBMP�榡�@�P
struct dictionary {
	uint8_t id;           // 儲存 id
	std::string code;     // 儲存 code 字串
};
struct BMPFileHeader {
	uint16_t fileType;     // ������� (������ "BM")
	uint32_t fileSize;     // ���j�p
	uint16_t reserved1;    // �O�d��
	uint16_t reserved2;    // �O�d��
	uint32_t offsetData;   // ������ƪ�������
};

struct BMPInfoHeader {
	uint32_t size;         // ���c�j�p (������40)
	int32_t width;         // �Ϥ��e��
	int32_t height;        // �Ϥ�����
	uint16_t planes;       // ��m������ (������1)
	uint16_t bitCount;     // �C�ӹ�������� (8���ܦǶ�)
	uint32_t compression;  // ���Y�覡 (0���ܵL���Y)
	uint32_t sizeImage;    // �Ϥ��j�p (�i�]��0)
	int32_t xPixelsPerMeter; // �����ѪR��
	int32_t yPixelsPerMeter; // �����ѪR��
	uint32_t colorsUsed;     // �զ�O�C��� (8-bit�q�`��256)
	uint32_t colorsImportant;// ���n�C��� (�i�]��0)
};

#pragma pack(pop)
struct huffman_node
{
	int id;																				//character 
	int freq;																				//frequency of the character
	string code;																			//huffman code for the character
	huffman_node* left;
	huffman_node* right;
	huffman_node()
	{//constructer
		left = right = NULL;
	}
	// Constructor
    huffman_node(int id, int frequency, string code, huffman_node* left = nullptr, huffman_node* right = nullptr)
        : id(id), freq(frequency), code(code), left(left), right(right) {}
};
typedef huffman_node* node_ptr;

class huffman
{
protected:
	BMPFileHeader bmpfileheader;
	BMPInfoHeader bmpinfoheader;
	node_ptr node_array[257];																//array for 128characters in the Ascii Table
	fstream in_file, out_file;
	node_ptr child, parent, root,troot;
	int id;
	string in_file_name , out_file_name ;
	std::vector<uint8_t> fileData;
	class compare
	{//a object funtion to set comparing rule of priority queue
	public:
		bool operator()(const node_ptr& c1, const node_ptr& c2) const
		{
			return c1->freq > c2->freq;
		}
	};
	priority_queue<node_ptr, vector<node_ptr>, compare> pq;									//priority queue of frequency from high to low
	void create_node_array();																
	void traverse(node_ptr, string);														//traverse the huffman tree and get huffman code for a character
	int binary_to_decimal(const string&);															//convert a 8-bit 0/1 string of binary code to a decimal integer 
	string decimal_to_binary(int);															//convert a decimal integer to a 8-bit 0/1 string of binary code
	inline void build_tree(string&, char);													//build the huffman tree according to information from file 

public:
	huffman(string, string);
	void create_pq();
	void create_huffman_tree();
	void calculate_huffman_codes();
	void coding_save();
	void decoding_save();
	void recreate_huffman_tree();
	void readGrayBMP();
	//void decoding_save();
	//string encode_pixel_data();
};

#endif
