#include "RLE.h"

void RLE::log() {
	std::cout << "Input file size: " << m_initialSize << '\n';
	std::cout << "Encoded file size: " << m_encodedSize << '\n';
	std::cout << "Compression ratio: " << (float)m_initialSize / m_encodedSize << '\n';
}

RLE::RLE() {
	m_block.resize(64U);
}

RLE::RLE(unsigned int blockSize) {
	m_block.resize(blockSize);
}

void RLE::encode(const std::string& filename) {
	std::ifstream fin(filename.c_str(), std::ios::binary | std::ios::ate);

	if (!fin.is_open()) {
		throw error_file(filename);
	}

	m_encodedSize = 0;
	m_initialSize = fin.tellg();	// get input file size
	fin.seekg(0, std::ios::beg);

	std::string e_filename{ filename };					// encoded filename
	if (e_filename.find(".rle") == std::string::npos)	// if there is no ".rle" extension
		e_filename.append(".rle");						// add ".rle" to the end
	else {												// otherwise throw error
		fin.close();
		throw error_extension(false);
	}

	std::ofstream fout(e_filename.c_str(), std::ios::binary);

	unsigned int count{ 1 };		// number of equal characters
	while (!fin.eof()) {
		fin.read(m_block.data(), m_block.size() - 1);	// read block from file to vector
		std::streamsize dataSize = fin.gcount();		// get the number of characters by the last read operation

		// Encoding
		for (int i = 0; i < dataSize; ++i) {
			if (m_block[i] == m_block[i + 1]) {			// if the next char equals current
				count++;								// count it
			}
			else {										// another char
				// create a string with the following structure:
				// number_of_equal_chars|current_char
				std::string temp = std::to_string(count) + '|' + m_block[i];
				m_encodedSize += temp.size();	// for log
				fout.write(temp.c_str(), temp.size());	// write block to the output file
				count = 1;
			}
		}
	}

	fin.close();
	fout.close();
	log();
}

void RLE::decode(const std::string& filename) {
	std::ifstream fin(filename.c_str(), std::ios::binary);

	if (!fin.is_open()) {
		throw error_file(filename);
	}

	std::string d_filename{ filename };					// decoded filename

	// all encoded files must contain ".rle" extension!
	std::size_t fileExt_pos = d_filename.find(".rle");	// try to find extension
	if (fileExt_pos != std::string::npos)				// if ".rle" does exist
		d_filename.erase(fileExt_pos, 4);				// erase it
	else {												// otherwise throw error
		fin.close();
		throw error_extension(true);
	}

	std::ofstream fout(d_filename.c_str(), std::ios::binary);

	while (!fin.eof()) {
		fin.read(m_block.data(), m_block.size());		// read block from file to vector
		std::streamsize dataSize = fin.gcount();		// get the number of characters by the last read operation
		if (!dataSize) break;

		int offset{ 0 };								// used for correct input
		while (m_block[dataSize - 2 + offset] != '|')
			offset--;
		fin.seekg(offset, fin.cur);

		unsigned int count{ 0 };						// number of equal chars
		std::stringstream ss;							// for convertion

		for (int i = 0; i < dataSize + offset - 1; ++i) {
			if (std::isdigit(m_block[i])) {				// if current char is digit
				ss << m_block[i];						// push it to stringstream
			}
			else {										// current char == '|'
				ss >> count;							// convert numbers
				for (int j = 0; j < count; ++j)
					fout.put(m_block[i + 1]);			// write char to the output file
				count = 0;
				ss.str("");
				ss.clear();
				++i;
			}
		}
	}

	fin.close();
	fout.close();
}