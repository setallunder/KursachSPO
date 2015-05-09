#include <fstream>

class FileWriter{
private:
	std::ofstream file;
public:
	FileWriter(const char *filePath){
		file.open(filePath);
	}
	void WriteLine(const char *line){
		file.write(line,strlen(line));
		if(strlen(line) > 1)
			file << '\n';
	}
	~FileWriter(){
		if(file.is_open())
		{
			file.close();
		}
	}
	void ReOpen(const char *filePath){
		if(file.is_open())
			file.close();
		file.open(filePath);
	}
	void Close(){
		if(file.is_open())
			file.close();
	}
};