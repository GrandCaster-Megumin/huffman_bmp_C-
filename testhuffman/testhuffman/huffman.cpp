#include "huffman.h"
#include<iostream>
#include <fstream>
#include <vector>
#include <cstdint>



void huffman::readGrayBMP() {

    std::ifstream file(in_file_name, std::ios::binary);
    if (!file) {
        std::cerr << "can not open file:" << in_file_name << std::endl;
        return;
    }

    // 讀取文件標頭
    file.read(reinterpret_cast<char*>(&bmpfileheader), sizeof(bmpfileheader));
    if (bmpfileheader.fileType != 0x4D42) { // "BM" in little-endian
        std::cerr << "is not bmp" << std::endl;
        return;
    }

    // 讀取資訊標頭
    file.read(reinterpret_cast<char*>(&bmpinfoheader), sizeof(bmpinfoheader));

    // 驗證是否為8-bit灰階BMP
    if (bmpinfoheader.bitCount != 8) {
        std::cerr << "is not gray" << std::endl;
        return;
    }

    std::cout << "with: " << bmpinfoheader.width << " pix\n";
    std::cout << "high: " << bmpinfoheader.height << " pix\n";

    // 跳過調色板 (灰階BMP通常有256個顏色，每個顏色占4 bytes)
    const int paletteSize = 256 * 4;
    file.seekg(paletteSize, std::ios::cur);

    // 讀取像素資料
    int rowSize = ((bmpinfoheader.width + 3) / 4) * 4; // 每行必須是4的倍數
    std::vector<uint8_t> pixelData(rowSize * abs(bmpinfoheader.height));
    file.seekg(bmpfileheader.offsetData, file.beg);
    file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

    // 打印所有像素點顏色值
    for (int y = 0; y < abs(bmpinfoheader.height); ++y) {
        for (int x = 0; x < bmpinfoheader.width; ++x) {
            int byteIndex = y * rowSize + x; // 直接取像素值
            uint8_t grayValue = pixelData[byteIndex];
            //std::cout << "X,Y=  (" << x << ", " << y << "): "
            //    << static_cast<int>(grayValue) << '\n';
            int grayValueInt = static_cast<int>(grayValue);
            node_array[grayValueInt]->freq++;
        }
    }
    fileData = pixelData;
    //將出現顏色次數記錄
    for (int i = 0; i < 256; i++)
    {
        if (node_array[i]->freq)
        {
            pq.push(node_array[i]);
        }
    }
}
//-------------------------------------BMP
// 創建節點數組，每個節點對應一個 ASCII 字符
void huffman::create_node_array()
{
    for (int i = 0; i < 257; i++)
    {
        node_array[i] = new huffman_node;
        node_array[i]->id = i;
        node_array[i]->freq = 0;
    }
}

// 遍歷 Huffman 樹並為每個葉節點分配 Huffman 編碼
void huffman::traverse(node_ptr node, string code)
{
    if (node->left == NULL && node->right == NULL)
    {
        node->code = code;
        std::cout << node->id<<" " << node->code << endl;
    }
    else
    {
        traverse(node->left, code + '0');
        traverse(node->right, code + '1');
    }
}

// 將二進制字符串轉換為十進制整數 沒用到
int huffman::binary_to_decimal(const string& in)
{
    int result = 0;
    for (int i = 0; i < in.size(); i++)
        result = result * 2 + in[i] - '0';//數字字元-'0' =數字  result * 2為左移
    return result;
}

// 將十進制整數轉換為 8 位二進制字符串 沒用到
string huffman::decimal_to_binary(int in)
{
    string temp = "";
    string result = "";
    while (in)
    {
        temp += ('0' + in % 2);
        in /= 2;
    } //6->011  需要反轉
    result.append(8 - temp.size(), '0'); // 不夠8位的部分補0
    for (int i = temp.size() - 1; i >= 0; i--)
    {
        result += temp[i];
    }
    return result;
}

// 根據輸入的編碼構建 Huffman 樹，忽略已存在的分支
inline void huffman::build_tree(string& path, char a_code)
{
    node_ptr current = root;
    for (int i = 0; i < path.size(); i++)
    {
        if (path[i] == '0')
        {
            if (current->left == NULL)
                current->left = new huffman_node;
            current = current->left;
        }
        else if (path[i] == '1')
        {
            if (current->right == NULL)
                current->right = new huffman_node;
            current = current->right;
        }
    }
    current->id = a_code; // 將字符附加到葉節點
}

// Huffman 類的構造函數，初始化輸入文件名和輸出文件名，並創建節點數組
huffman::huffman(string in, string out)
{
    in_file_name = in;
    out_file_name = out;
    create_node_array();
}


// 創建 Huffman 樹
void huffman::create_huffman_tree()
{
    priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
    while (temp.size() > 1)
    {
        root = new huffman_node;
        root->freq = 0;
        root->left = temp.top();
        root->freq += temp.top()->freq;

        temp.pop();
        root->right = temp.top();
        root->freq += temp.top()->freq;

        temp.pop();
        temp.push(root);
    }
}

// 計算 Huffman 編碼
void huffman::calculate_huffman_codes()
{
    traverse(root, "");
}

//// 將編碼結果保存到文件 這邊黑白圖沒放調色盤
void huffman::coding_save()
{
    out_file.open(out_file_name, ios::out | ios::binary);
    int nodesum = 0;
    if (!out_file.is_open()) {
        cerr << "Error opening files!" << endl;
        return;
    }

    // Write BMP headers to the output file
    out_file.write(reinterpret_cast<char*>(&bmpfileheader), sizeof(BMPFileHeader));
    out_file.write(reinterpret_cast<char*>(&bmpinfoheader), sizeof(BMPInfoHeader));

    //將有用到的資料數紀錄
    for (int i = 0; i < 257; i++) {
        if (node_array[i]->freq > 0)
            nodesum += 1;
    }
    //寫入資料數huff編碼  格式id len code
    out_file.write(reinterpret_cast<char*>(&nodesum), sizeof(uint8_t));
    for (int i = 0; i < 257;i++) {
        if (node_array[i]->freq > 0) { // 確保節點有效
            uint8_t id = node_array[i]->id;
            uint8_t code_length = static_cast<uint8_t>(node_array[i]->code.length());
            std::vector<char> buffer(node_array[i]->code.begin(), node_array[i]->code.end());
            // 寫入 id 和 code_length
            out_file.write(reinterpret_cast<char*>(&id), sizeof(uint8_t));
            out_file.write(reinterpret_cast<char*>(&code_length), sizeof(uint8_t));

            // 將 code 字串轉為二進制後寫入
            for (auto& b : buffer) {
                out_file << b;
            }
            //out_file.write(node_array[i]->code.c_str(), node_array[i]->code.length());
            //debug用
            //std::cout << node_array[i]->id << node_array[i]->code.length() << node_array[i]->code;
        }
    }
    // 創建一個空的 std::vector<char> 來存放轉換後的字符
    std::vector<char> charVector;
    // 使用 for 迴圈遍歷 copiedData 並轉換為字符放入 charVector
    for (uint8_t byte : fileData) {
        std::string code = node_array[byte]->code;  // 假設這裡是 std::string
        for (char c : code) {
            charVector.push_back(c);  // 把每個字符插入到 vector 中
        }
    }
    //將charVector寫到檔案
    for (auto& b : charVector) {
        out_file << b;
    }


    out_file.close();
    std::cout << "Successfully saved Huffman encoded file!" << endl;
}



// 將解碼結果保存到文件
void huffman::decoding_save() {

    std::vector<dictionary> dictionary;
    std::cout << "decoding_save";
    in_file.open(out_file_name, ios::in | ios::binary);
    if (!in_file.is_open()) {
        cerr << "Error opening files!" << endl;
        return;
    }

    // 读取 BMP 文件头和信息头
    in_file.read(reinterpret_cast<char*>(&bmpfileheader), sizeof(BMPFileHeader));
    in_file.read(reinterpret_cast<char*>(&bmpinfoheader), sizeof(BMPInfoHeader));
    std::cout << "decoding_save2";
    uint8_t size_of_node;

    in_file.read(reinterpret_cast<char*>(&size_of_node), sizeof(size_of_node));
    for (int i = 0; i < size_of_node;i++) {
        uint8_t id, code_length;

        // 讀取 id 和 code_length
        in_file.read(reinterpret_cast<char*>(&id), sizeof(uint8_t));
        in_file.read(reinterpret_cast<char*>(&code_length), sizeof(uint8_t));

        string code;
        char c[1];
        for (int i = 0; i < code_length; i++)
        {
            in_file.read(c, sizeof(c));
            code += std::string(c, 1);
        }
        dictionary.push_back({ id, std::string(code) });
    }
    //將0101資料建樹，這個重要隊與解碼速度差很大
    troot = new huffman_node();
    for (const auto& dict : dictionary)
    {
        node_ptr current = troot;
        for (char bit : dict.code) {
            if (bit == '0') {
                if (current->left == NULL) {
                    current->left = new huffman_node();
                }
                current = current->left;
            }
            else {
                if (current->right == NULL) {
                    current->right = new huffman_node();
                }
                current = current->right;
            }
        }
        current->id = id;
    }

    //將資料0101資料轉成正常編碼
    string allpix;
    allpix.assign(std::istreambuf_iterator<char>(in_file), std::istreambuf_iterator<char>());
    in_file >> allpix;
    std::ostringstream oss;

    huffman_node* current = root; // 開始於樹的根節點
    for (char bit : allpix) {
        // 根據 bit 移動樹節點
        if (bit == '0' && current->left) {
            current = current->left;
        }
        else if (bit == '1' && current->right) {
            current = current->right;
        }
        else {
            cerr << "Error: Invalid Huffman encoding in input.\n";
            return; // 若輸入非合法編碼，直接退出
        }

        // 若到達葉節點，記錄 ID 並重置到根節點
        if (current->left == nullptr && current->right == nullptr) {
            oss << current->id << " "; // 將 ID 寫入 oss，並以空格分隔
            current = root; // 重置為根節點
        }
    }

    /// ///////////////沒建立tree版本手動算要跑2天
    //size_t index = 0;  // 用於追踪 allpix 的當前位置
    //while (index < allpix.size()) {
    //    std::string currentSubstr;
    //    bool matched = false;

    //    // 動態增加子字串長度直到匹配成功
    //    for (size_t length = 1; index + length <= allpix.size(); ++length) {
    //        currentSubstr = allpix.substr(index, length);  // 提取子字串

    //        // 與字典中的 code 比對
    //        for (const auto& dict : dictionary) {
    //            if (currentSubstr == dict.code) {
    //                // 匹配成功，輸出 id 並移動 index
    //                std::cout << "Matched ID: " << static_cast<int>(dict.id) << std::endl;
    //                index += length;  // 跳過已匹配的部分

    //                oss << dict.id;
    //            }
    //        }
    //    }
    //}
    out_file.open("new.bmp", ios::out | ios::binary);
    if (!out_file.is_open()) {
        cerr << "Error opening files!" << endl;
        return;
    }

    // Write BMP headers to the output file
    out_file.write(reinterpret_cast<char*>(&bmpfileheader), sizeof(bmpfileheader));
    out_file.write(reinterpret_cast<char*>(&bmpinfoheader), sizeof(bmpinfoheader));

    // 調色板
    struct Color {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t reserved; // 通常為 0
    };
    std::vector<Color> palette(256);
    for (int i = 0; i < 256; ++i) {
        palette[i].blue = static_cast<uint8_t>(i);  // 藍色分量
        palette[i].green = static_cast<uint8_t>(i); // 綠色分量
        palette[i].red = static_cast<uint8_t>(i);   // 紅色分量
        palette[i].reserved = 0;                     // 保留位通常為 0
    }
    // 寫入調色板 (每個顏色 4 字節，共 256 顏色)
    out_file.write(reinterpret_cast<const char*>(palette.data()), palette.size() * sizeof(Color));
    //將資料轉換格式
    std::string alldata = oss.str();  // 提取 oss 中的資料為 std::string
    std::vector<uint8_t> pixelData;
    std::istringstream iss(alldata);
    int pixelValue;
    while (iss >> pixelValue) {
        pixelData.push_back(static_cast<uint8_t>(pixelValue));
    }
    //寫入轉換格式vector<uint8_t> pixelData 像素
    out_file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

    out_file.close();
    std::cout << alldata;//檢查像素還原
    std::cout << "Successfully decoded and saved BMP file!" << endl;
}
