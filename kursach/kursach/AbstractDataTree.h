#ifndef ABSTRACTDATATREE_H
#define ABSTRACTDATATREE_H

#include <iostream>

/****************************************************************
*	����� ����������� (������������) ������������ ������ ������	*
*****************************************************************
*	������ ����� ���������� � ������������ �� ����������		*
*	� ��������� ������������ � ������� ��������� � ����.		*
*	��������� �������� ��������� ����������� ��������			*
*	����������.													*
*****************************************************************
*	�����������:												*
*	1. switch, if/else � ����� ����� ��������� ����� ������;	*
*	2. ���������������� ���� ����;								*
*	3. ��� � ���������� ����������;								*
*	4. ���� ����� ������ ���� ��������� � ������.				*
*****************************************************************/

class AbstractDataTree{
private:
	//���� ������, ��� �������� ������
	class Node{
	private:
		Node(Node &);
	public:
		int pos;	//������� � ����
		char *var;	//��� ����������
		char *val;	//�������� ����������
		Node *next;	//��������� ����
		Node *prev;	//���������� ����
		Node(){
			pos = 0;
			var = val = NULL;
			next = prev = NULL;
		}
	}*root, *end;	//������ � ����� ������
public:
	AbstractDataTree(){
		root = new Node();
		root->pos = -1;
		end = root;
	}
	//�������� ����� ���������� � ������
	void Put(const char *var, const char *val, int pos){
		//������� ����� ����
		end->next = new Node();
		end->next->prev = end;
		end = end->next;

		//�������������� ��� ����������
		if(var){
			end->var = new char[strlen(var)+1];
			strcpy(end->var,var);
			end->var[strlen(var)] = '\0';
		}

		//�������������� �������� ����������
		if(val){
			end->val = new char[strlen(val)+1];
			strcpy(end->val,val);
			end->val[strlen(val)] = '\0';
		}

		//���������� �������
		end->pos = pos;
	}

	void PutSeparator(){
		//������� ����� ����
		end->next = new Node();
		end->next->prev = end;
		end = end->next;

		//���������� �������
		end->pos = -1;
	}

	void PutNamedSeparator(const char *name)
	{
		//������� ����� ����
		end->next = new Node();
		end->next->prev = end;
		end = end->next;

		//���������� �������
		end->pos = -1;
		if(name != NULL && strlen(name))
		{
			end->var = new char[strlen(name)+1];
			strcpy(end->var, name);
			end->var[strlen(end->var)] = '\0';
		}
	}

	//����� ���������� �������� ����������
	//���������� NULL, ���� ���������� ����� �� �����������
	char * Find(const char *var, int pos){
		if(root == end)
			return NULL;
		char *buffer = NULL;
		Node *go = root;


		//���������� �� ������������, ���������, ����������� ����� ����������
		while(go->next && go->next->pos < pos){
			go = go->next;
		}

		//������� ��������� ������������ ������� ����������
		//-1 ��������� ����� ������
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
		delete go; //root �� ������ ����������
	}
};

#endif