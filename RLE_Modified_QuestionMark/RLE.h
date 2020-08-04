#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct error_file {
	error_file(const std::string& name) {
		std::cerr << "Error: could not open file \"" << name << "\"\n";
	}
};

struct error_extension {
	error_extension(bool should) {
		if (should)
			std::cerr << "Error: file should contain \".rle\" extension\n";
		else
			std::cerr << "Error: file shouldn't contain \".rle\" extension\n";
	}
};

class RLE {
private:
	std::vector<char> m_block;

	unsigned int m_initialSize{ 0 };
	unsigned int m_encodedSize{ 0 };

private:
	void log();

public:
	RLE();
	RLE(unsigned int blockSize);

	void encode(const std::string& filename);
	void decode(const std::string& filename);
};