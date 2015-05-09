#include <iostream>
#include <fstream>
#include <exception>

class FileReader{
private:
	bool end;
	int lastLineStart;
	int lastLineSize;
	std::ifstream file;
public:
	FileReader(const char *filePath){
		end = lastLineStart = lastLineSize = 0;
		file.open(filePath);
		if(!file){
			throw std::exception("File not found!\n");
		}
	}
	~FileReader(){
		if(file.is_open())
		{
			file.close();
		}
	}
	char * ReadLine(){
		char *buffer = NULL;
		lastLineStart = file.tellg();
		while (file.get() != '\n'){
			if(file.eof()){
				end = true;
				file.clear();
				break;
			}
			lastLineSize++;
		}
		file.seekg(lastLineStart);
		buffer = new char[lastLineSize+1];
		file.getline(buffer,lastLineSize+1);
		buffer[lastLineSize] = '\0';
		return buffer;
	}
	bool isEnd(){
		return end;
	}
	void ReOpen(const char *filePath){
		end = false;
		lastLineStart = 0;
		lastLineSize = 0;
		if(file.is_open())
			file.close();
		file.open(filePath);
	}
	void Close(){
		end = false;
		lastLineStart = 0;
		lastLineSize = 0;
		if(file.is_open())
			file.close();
	}
};