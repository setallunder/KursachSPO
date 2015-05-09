#ifndef ABSTRACTDATATREE_H
#define ABSTRACTDATATREE_H

#include <iostream>

/****************************************************************
*	Класс упрощенного (вырожденного) абстрактного дерева данных	*
*****************************************************************
*	Хранит имена переменных с присвоенными им значениями		*
*	и позициями присваивания в порядке появления в коде.		*
*	Позволяет получить последнее присвоенное значение			*
*	переменной.													*
*****************************************************************
*	Особенности:												*
*	1. switch, if/else и любые циклы прерывают поток данных;	*
*	2. Пользовательский ввод тоже;								*
*	3. Как и применение указателей;								*
*	4. Тело цикла должно быть заключено в скобки.				*
*****************************************************************/

class AbstractDataTree{
private:
	//Узел дерева, для хранения данных
	class Node{
	private:
		Node(Node &);
	public:
		int pos;	//позиция в коде
		char *var;	//имя переменной
		char *val;	//значение переменной
		Node *next;	//следующий узел
		Node *prev;	//предудыщий узел
		Node(){
			pos = 0;
			var = val = NULL;
			next = prev = NULL;
		}
	}*root, *end;	//начало и конец дерева
public:
	AbstractDataTree(){
		root = new Node();
		root->pos = -1;
		end = root;
	}
	//Помещает новую переменную в дерево
	void Put(const char *var, const char *val, int pos){
		//создаем новый узел
		end->next = new Node();
		end->next->prev = end;
		end = end->next;

		//инициализируем имя переменной
		if(var){
			end->var = new char[strlen(var)+1];
			strcpy(end->var,var);
			end->var[strlen(var)] = '\0';
		}

		//инициализируем значение переменной
		if(val){
			end->val = new char[strlen(val)+1];
			strcpy(end->val,val);
			end->val[strlen(val)] = '\0';
		}

		//записываем позицию
		end->pos = pos;
	}

	void PutSeparator(){
		//создаем новый узел
		end->next = new Node();
		end->next->prev = end;
		end = end->next;

		//записываем позицию
		end->pos = -1;
	}

	void PutNamedSeparator(const char *name)
	{
		//создаем новый узел
		end->next = new Node();
		end->next->prev = end;
		end = end->next;

		//записываем позицию
		end->pos = -1;
		if(name != NULL && strlen(name))
		{
			end->var = new char[strlen(name)+1];
			strcpy(end->var, name);
			end->var[strlen(end->var)] = '\0';
		}
	}

	//Поиск последнего значения переменной
	//Возвращает NULL, если переменная ранее не встречалась
	char * Find(const char *var, int pos){
		if(root == end)
			return NULL;
		char *buffer = NULL;
		Node *go = root;


		//становимся на присваивание, последнее, встреченное перед переменной
		while(go->next && go->next->pos < pos){
			go = go->next;
		}

		//находим последнее присваивание искомой переменной
		//-1 прерывает поток данных
		while(go != root && (go->pos != -1 || go->var && strcmp(go->var, var))){
			if(go->var && !strcmp(go->var,var)){
				buffer = new char[strlen(go->val)+1];
				strcpy(buffer,go->val);
				buffer[strlen(go->val)] = '\0';
				break;
			}
			go = go->prev;
		}
		return buffer;
	}
	~AbstractDataTree(){
		Node *go = end;
		while(go != root){
			go = go->prev;
			if (go->next->val)
			{
				delete go->next->val;
			}
			if (go->next->var)
			{
				delete go->next->var;
			}
			if (go->next)
			{
				delete go->next;
			}
		}
		delete go; //root не хранит информации
	}
};

#endif