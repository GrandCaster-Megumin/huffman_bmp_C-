# huffman_bmp_C++
![image](https://github.com/user-attachments/assets/f5335fef-6385-44f1-b6f1-e303e65e8881)
有編碼，解碼像素點
編譯 主 huffmanCoding.cpp 副 huffman.cpp huffman.h
限灰階256  範例lena_256.bmp ->lena_256666.bin->new.bmp
執行環境 win10  visual studio 2019
### 步驟
讀bmp->建樹->編碼->存檔->讀存檔->解碼->重建格式
### 標頭檔格式
Bmp標頭，調色盤，樹葉節點宣告，樹宣告，宣告可用函式
### 建樹方法
放到排序列兩兩最小頻率建樹放回直到排序列為1
![image](https://github.com/user-attachments/assets/c5326c0d-df52-4ba9-9f31-4641cce0886d)
### 存檔格式
標頭->灰階調色盤是已知跳過->下一格格式出現次數->編碼符號,長度,碼 如 255,3,011->編碼串
### 解碼 
讀取標頭->讀8個碼知道下個格式出現次數-> 讀取8個碼編//碼符號, 讀取8個碼//長度,讀取長度//碼->讀到底

用碼重建樹★用暴力比對速度會很慢

重建步驟 碼01跑左右沒葉子就創葉子跑完碼放符號
![image](https://github.com/user-attachments/assets/38f5bd66-1870-440f-87f0-5d54f453bdfa)
### 解碼
重樹根比對比對到有葉子重跑，直到結束
### 重建格式
標頭->灰階調色盤->解碼


參考來源 https://github.com/cynricfu/huffman-coding
2024_11_30 test can use
