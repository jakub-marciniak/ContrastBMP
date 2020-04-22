#include <iostream>
#include <fstream>

#define HeaderSize 54

#define GET_VALUE(x, from) (memcpy(&x, &from, sizeof(x)))

using namespace std;

enum {
	Magic = 0,
	BSize = 2,
	Reserved1 = 6,
	Reserved2 = 8,
	Offset = 10,

	HSize = 14,
	Width = 18,
	Height = 22,

	BitCount = 28,

	SizeImage = 34,
};

struct BMPFileHeader {
	uint16_t magic;
	uint32_t bsize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
	uint32_t hsize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitcount;
	uint32_t compression;
	uint32_t sizeimage;
	uint32_t xpelspermeter; //ignore
	uint32_t ypelspermeter; //ignore
	uint32_t clrused;
	uint32_t clrimportant; //ignore
} typedef BMPInfoHeader;

uint8_t calc(uint8_t p, double contrast) {
	int32_t p2 = p;
	p2 = (int32_t)((p2 - 128) * contrast) + 128;
	if (p2 < 0) {
		p2 = 0;
	}
	else if (p2 > 255) {
		p2 = 255;
	}
	return (uint8_t)p2;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	double contrast;
	cin >> contrast;
	string name;
	cin >> name;
	string nameout = name;
	nameout.erase(nameout.find(".bmp"));
	nameout += "-out.bmp";

	ifstream f(name, ios::binary);

	char tmp[HeaderSize];

	f.read(tmp, HeaderSize);

	BMPFileHeader bmpfh;

	GET_VALUE(bmpfh.width, tmp[Width]);
	GET_VALUE(bmpfh.height, tmp[Height]);
	//cout << "height: " << bmpfh.height << " width: " << bmpfh.width << "\n";

	GET_VALUE(bmpfh.offset, tmp[Offset]);
	GET_VALUE(bmpfh.bsize, tmp[BSize]);
	//cout << "offset: " << bmpfh.offset << " bsize: " << bmpfh.bsize << "\n";
	
	char* var = new char[bmpfh.bsize];
	f.read(var, bmpfh.bsize);
	f.close();
	for (uint32_t i = bmpfh.offset; i < bmpfh.bsize; i++) {
		var[i] = calc(var[i], contrast);
	}

	ofstream fout(nameout, ios::binary);

	fout.write(tmp, HeaderSize);
	fout.write(var, bmpfh.bsize);

	cout << nameout;

	exit(0);
}
