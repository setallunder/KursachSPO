#include "Optimazer.h"
#include "FileReader.h"
#include "FileWriter.h"
#include <fstream>
#include <stdlib.h>
#include <iostream>

int multiplyAndDivideImprovement;
int replaceMultiplicationImprovement;
int replaceVariablesImprovement;

void Optimazer::Optimaze(const char *filePath){
	multiplyAndDivideImprovement = 0;
	replaceMultiplicationImprovement = 0;
	replaceVariablesImprovement = 0;
	AbstractDataTree variablesTree;
	int baseIndex = 0; 
	char *line = NULL;
	FileReader read(filePath);
	FileWriter write("temp.txt");
	while(!read.isEnd()){
		line = read.ReadLine();		//читаем строку исходного файла
		ReplaceVariablesWithTheirValue(&line,baseIndex,variablesTree);
		MutiplyAndDivide(&line);			//оптимизируем
		AddVariablesInDataTree(line,baseIndex,variablesTree);
		baseIndex += strlen(line)+1;
		ReplaceMultiplication(&line);
		write.WriteLine(line);		//записываем оптимизированную строку во временный файл
		delete line;				//удаляем строку
	}
	//перезаписываем исходный файл
	read.Close();
	write.Close();
	write.ReOpen(filePath);
	read.ReOpen("temp.txt");
	while(!read.isEnd())
		write.WriteLine(read.ReadLine());
	read.Close();
	write.Close();
	remove("temp.txt");
	std::cout << "\nImptovements:\n -By multiplying and dividing constants: " << 
		multiplyAndDivideImprovement << " ticks saved;\n" <<
		" -By replacing multiplications: " << replaceMultiplicationImprovement << " ticks;\n" <<
		" -By replacing variables: " << replaceVariablesImprovement << " ticks." << std::endl;
}

void Optimazer::MutiplyAndDivide(char **line){
	int i = 0, j = 0, k = 0, count = 1;
	float result = 0, op1 = 0, fop1 = 0, op2 = 0, fop2 = 0;
	char buf[20], *tempLine;
	bool fp = false;
	int countSars = 0;
	for(int l=0; (*line)[l] != '\0'; l++)
	{
		if((*line)[l] == '*')
		{
			countSars++;
		}
	}
	tempLine = new char[strlen((*line))+10+5*countSars];
	strcpy(tempLine,(*line));
	delete (*line);
	(*line) = new char[strlen(tempLine)+10+5*countSars];
	strcpy((*line),tempLine);
	for(i = 0;(*line)[i] != '\0'; i++){
		if((*line)[i] == '*' || (*line)[i] == '/'){	//числа начинающиеся с нуля не обрабатываются
			for(j = i+1; (*line)[j] == ' '; j++);
			if(!isdigit((*line)[j]))
			{
				tempLine[k++] = (*line)[i];
				continue;
			}
			for(j = i-1; (*line)[j] == ' '; j--, k--); //пропускаем пробелы
			//становимся в начало левого операнда, смещаемся также в дополнительной строке
			for(j; (*line)[j] >= '0' && (*line)[j] <= '9' && j >= 0; j--, k--);
			//если оказались на букве, то это была переменная, на скобку тоже умножать не будем
			if((*line)[j] >= 'a' && (*line)[j] <= 'z' || (*line)[j] >= 'A' && (*line)[j] <= 'Z' || (*line)[j] == ')'){	
				while(j != i-1){	//восстанавливаем k
					j++, k++;
				}
				tempLine[k++] = (*line)[i];	//записываем знак умножения
				continue;					//продолжаем основной цикл по поиску '*' и '/'
			}
			if((*line)[j] == '.'){	//если дробное
				fp = true;		//устанавливаем флаг дробного числа
				j--, k--;		//пропускаем десятичную точку
				for(j; (*line)[j] >= '0' && (*line)[j] <= '9' && j >= 0; j--, k--);	//становимся в начало целой части
				j++;			//становимся на первую цифру					//смещаемся также в дополнительной строке
				for(j; (*line)[j] != '.'; j++){									//записываем целую часть в fop1
					fop1 *= 10;
					fop1 += (*line)[j] - '0';
				}
			}
			j++;				//пропускаем десятичную точку
			for(j; (*line)[j] >= '0' && (*line)[j] <= '9' && j!=i; j++){//записываем дробную или целую часть в op1
				op1 *= 10;
				op1 += (*line)[j] - '0';
				count *= 10;	//считаем разряды
			}
			if(fp){				//если число было дробным, сохраняем в op1 сумму целой части и дробной, делёной на разряды
				fp = false;		//сбрасываем флаг
				op1 = fop1 + op1/count;
			}
			count = 1;			//сбрасываем посчитанные разряды
			for(j = i+1; (*line)[j] == ' '; j++);
			for(j; (*line)[j] >= '0' && (*line)[j] <= '9' && (*line)[j] != '\0'; j++){	//записываем целую часть правого операнда в op2
				op2 *= 10;
				op2 += (*line)[j] - '0';
			}
			if((*line)[j] == '.'){	//если число дробное
				j++;			//пропускаем десятичную точку
				for(j; (*line)[j] >= '0' && (*line)[j] <= '9' && (*line)[j] != '\0'; j++){		//записываем дробную часть в fop2
					fop2 *= 10;
					fop2 += (*line)[j] - '0';
					count *= 10;
				}
				op2 = op2 + fop2/count;	//получаем само число из суммы дробной и целой частей
				count = 1;		//сбрасываем счетчик разрядов
			}
			if((*line)[i] == '*')	//если было умножение
				result = op1 * op2;
			else				//иначе деление
				result = op1 / op2;
			itoa((int)(result*1000),buf,10);//результат сохраняем в buf
			op1 = fop1 = op2 = fop2 = 0;//сбрасываем переменные
			for(int z = 0; buf[z] >= '0' && buf[z] <= '9'; z++, k++){	//переписываемновое посчитанное значение во временную строку
				tempLine[k] = buf[z];
				if(z+4 == strlen(buf))
				{
					tempLine[++k] = '.';
				}
			}
			multiplyAndDivideImprovement++;
			i = j;
			while((*line)[i] != '\0'){
				tempLine[k++] = (*line)[i++];
			}
			tempLine[k] = '\0';
			strcpy((*line),tempLine);
			j = k = result = 0;
			i = -1;
			continue;
		}
		else
		{
			tempLine[k++] = (*line)[i];//копируем оснувную строку в дополнительную
		}
	}
	tempLine[k] = '\0';			//завершаем доп. строку
	strcpy((*line),tempLine);		//перезаписываем основную
	delete tempLine;			//удаляем доп. строку
}

void Optimazer::ReplaceMultiplication(char **line)
{
	char *buffer = NULL, *result = NULL;
	char leftVar[257], rightVar[257];
	int sourceInd = 0, varInd = 0, searchInd = 0, resultInd = 0;
	int leftIsDigit = 1, rightIsDigit = 1;
	int amountOfReps = 0, beginOfParamsInResultLine = 0;
	buffer = new char[strlen(*line)+1];
	result = new char[strlen(*line)+1];
	for (sourceInd = 0; (*line)[sourceInd] != '\0'; sourceInd++)
	{
		result[resultInd++] = (*line)[sourceInd];
		if ((*line)[sourceInd] == '*')
		{
			beginOfParamsInResultLine = resultInd-2;
			//пропускаем пробелы, идущие после левого операнда
			for (searchInd = sourceInd - 1; (*line)[searchInd] == ' '; searchInd--, beginOfParamsInResultLine--);
			//смещаемся в начало левого операнда
			for (searchInd; searchInd >= 0 && (isalpha((*line)[searchInd]) || isdigit((*line)[searchInd])); searchInd--, beginOfParamsInResultLine--);
			searchInd++;				//становимся на первую цифру
			beginOfParamsInResultLine++;
			//записываем имя левого операнда в соответствующий буфер
			for (varInd = 0; (*line)[searchInd] != '*'; searchInd++, varInd++)
			{
				leftVar[varInd] = (*line)[searchInd];
				if (isalpha((*line)[searchInd]))
					leftIsDigit = 0;
			}
			leftVar[varInd] = '\0';
			searchInd++;			//смещаемся за знак умножения
			//пропускаем пробелы
			for (; (*line)[searchInd] == ' '; searchInd++);
			//записываем имя правого операнда в соответствующий буфер
			for (varInd = 0; isdigit((*line)[searchInd]) || isalpha((*line)[searchInd]); searchInd++, varInd++) 
			{
				rightVar[varInd] = (*line)[searchInd];
				if (isalpha((*line)[searchInd]))
					rightIsDigit = 0;
			}
			rightVar[varInd] = '\0';
			if (!leftIsDigit && !rightIsDigit)
				continue;
			if (leftIsDigit && rightIsDigit)
				continue;
			if (leftIsDigit)
				amountOfReps = atoi(leftVar);
			if (rightIsDigit)
			{
				amountOfReps = atoi(rightVar);
				strcpy(rightVar,leftVar);		//имя помещается в правый операнд
			}
			if (amountOfReps > 5 || amountOfReps == 0)
				continue;
			//расширяем буфер результата и промежуточный буфер
			strcpy(buffer, result);
			delete result;
			result = new char[strlen(buffer)+amountOfReps*strlen(rightVar)+1];
			strcpy(result,buffer);
			delete buffer;
			buffer = new char[strlen(result)+amountOfReps*strlen(rightVar)+1];
			resultInd = beginOfParamsInResultLine;
			//записываем результат, берем его в скобки
			result[resultInd++] = '(';
			replaceMultiplicationImprovement += amountOfReps;
			while (amountOfReps != 1)
			{
				for (int i = 0; rightVar[i] != '\0'; i++, resultInd++)
					result[resultInd] = rightVar[i];
				result[resultInd++] = '+';
				amountOfReps--;
			}
			for (int i = 0; rightVar[i] != '\0'; i++, resultInd++)
				result[resultInd] = rightVar[i];
			result[resultInd++] = ')';
			sourceInd = searchInd-1;
		}
	}
	result[resultInd] = '\0';
	//расширяем исходную строку, помещаем туда результат
	delete (*line);
	(*line) = new char[strlen(result)+1];
	strcpy((*line),result);
	delete buffer;
	delete result;
}

void Optimazer::AddVariablesInDataTree(char *line, int baseInd, AbstractDataTree &variablesTree)
{
	char variable[257], value[257];
	for (int i = 0; line[i] != '\0'; i++, baseInd++)
	{
		if (line[i] == '{' || line[i] == '}')	//прерываем данные на границах областей видимости
		{
			variablesTree.PutSeparator();
		}
		if (line[i] == '*')						//прерываем данные при встрече указателей
		{
			if (isalpha(line[i+1]))
			{
				variablesTree.PutSeparator();
			}
		}
		if(line[i] == '&' && (line[i+1] == '(' || isalpha(line[i+1])))
		{
			for(int j = i+1, k = 0; isalpha(line[j]); j++, k++)
			{
				variable[k] = line[j];
				variable[k+1] = '\0';
			}
			variablesTree.PutNamedSeparator(variable);
		}
		if (line[i] == '=' && line[i+1] != '=' && i-1 >= 0 && line[i-1] != '=')	//помещаем в дерево переменную с её значением
		{

			int j = 0;
			int k = 0;
			for (j = i-1; line[j] == ' '; j--);
			for (;j>=0 && (isalpha(line[j]) || isdigit(line[j])); j--);
			j++;
			for (k = 0;line[j] != ' ' && line[j] != '='; j++, k++)	//читаем имя переменной
			{
				variable[k] = line[j];
			}
			variable[k] = '\0';
			for (;line[j] != '='; j++);
			j++;
			for(;line[j] == ' '; j++);
			int countBrackets = 0;
			for(k = 0, countBrackets = 0;isalpha(line[j]) || isdigit(line[j]) || line[j] == '*' || line[j] == '/' || 
				    line[j] == '+' || line[j] == '-' || line[j] == '%' || line[j] == '(' || line[j] == ')' || line[j] == ' '; j++, k++)
			{
				if (line[j] == '(') countBrackets++;
				else if (line[j] == ')')
				{
					countBrackets--;
					if (countBrackets < 0) break;
				}
				value[k] = line[j];
			}
			value[k] = '\0';
			variablesTree.Put(variable,value,baseInd);
		}
	}
}

void Optimazer::ReplaceVariablesWithTheirValue(char **line,int baseIndex, AbstractDataTree &variablesTree)
{
	bool isValueSimple = true;
	char variable[257];
	char *value, *buffer, *result;
	int sourseIndex = 0, searchIndex = 0, resultIndex = 0, lenthOfFinalString = 0, countBrackets = 0;
	result = new char[strlen((*line))*10+1];
	lenthOfFinalString = strlen((*line)) + 1;
	for(sourseIndex = 0, resultIndex = 0; (*line)[sourseIndex] != '\0'; sourseIndex++, resultIndex++)
	{
		result[resultIndex] = (*line)[sourseIndex];
		if((*line)[sourseIndex] == '=' && (*line)[sourseIndex+1] != '=' && sourseIndex - 1 >= 0 && (*line)[sourseIndex-1] != '=')
		{
			searchIndex = sourseIndex + 1;
			resultIndex++;
			countBrackets = 0;
			while (true)
			{
				for(; (*line)[searchIndex] == ' ' || isdigit((*line)[searchIndex]) || (*line)[searchIndex] == '*' || 
						(*line)[searchIndex] == '/' || (*line)[searchIndex] == '+' || (*line)[searchIndex] == '-' || 
							(*line)[searchIndex] == '%' || (*line)[searchIndex] == '(' || (*line)[searchIndex] == ')'; searchIndex++, resultIndex++)
				{
					if((*line)[searchIndex] == '(') countBrackets++;
					else if((*line)[searchIndex] == ')')
					{
						countBrackets--;
						if(countBrackets < 0) break;
					}
					result[resultIndex] = (*line)[searchIndex];
				}
				if(!isalpha((*line)[searchIndex]))
				{
					sourseIndex = searchIndex - 1;
					resultIndex--;
					break;
				}
				for(int i = 0, j = searchIndex; isalpha((*line)[j]); j++, i++)
				{
					variable[i] = (*line)[j];
					variable[i+1] = '\0';
				}
				value = variablesTree.Find(variable, sourseIndex + baseIndex);
				if(value == NULL)
				{
					sourseIndex = searchIndex - 1;
					resultIndex--;
					break;
				}
				for(int i = 0; value[i] != '\0'; i++)
				{
					if(!isdigit(value[i]))
					{
						isValueSimple = false;
					}
				}
				searchIndex += strlen(variable);
				lenthOfFinalString += (strlen(value)+2);
				if(!isValueSimple)
				{
					result[resultIndex++] = '(';
				}
				for(int i = 0; value[i] != '\0'; i++, resultIndex++)
				{
					result[resultIndex] = value[i];
				}
				if(!isValueSimple)
				{
					result[resultIndex++] = ')';
				}
				replaceVariablesImprovement++;
			}
		}
	}
	result[resultIndex] = '\0';
	delete (*line);
	(*line) = new char[lenthOfFinalString+1];
	strcpy((*line),result);
}

