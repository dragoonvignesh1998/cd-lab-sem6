#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TableLength 30

typedef struct node{
	char dtype[10];
	char rtype[10];
	char name[20];
	char scope[10];
	int type;
	int index;
	int num;
	int argsList[10];
	char size;
	struct node* next;
}Node;

Node* Table[TableLength]; 	

void Initialize(){
	for (int i = 0; i < TableLength; ++i)
	{
		Table[i] = NULL;
	}
}

char* show(int type){
	if(type == 0)
		return "Identifier";
	else
		return "Function";
}

char* indToString(int ind){

		char* ind_str = (char*)malloc(10*sizeof(char));

		int num = ind;
		int len = 0;

		while(num > 0){
			len++;
			num /= 10;
		}

		for(int i = 0; i < len; i++){
			int digit = ind % 10;
			ind_str[len - i - 1] = digit + '0';
			ind /= 10;
		}

		ind_str[len] = '\0';

		return ind_str;
}

void printArgs(Node* temp){
	char sep = '|'; 
	char id[10];
	char buf[100] = "<"; 			
	if(temp->num == 0){
		strcpy(buf,"<None>");
		printf("%15s", buf);
		printf("\t\t%c",sep);
		return;
	}
	
	for (int i = 0; i < temp->num; ++i)
	{
		strcpy(id," id");
		strcat(id,indToString(temp->argsList[i]));
		strcat(buf,id);
	}

	buf[strlen(buf)] = '>';
	buf[strlen(buf)] = '\0';
	printf("%15s", buf);
	printf("\t\t%c",sep);
	
}

void display()
{
	//char sep = '|';
	//printf("------------------------------------------------------------------------------------------------------------------------------\n");
	//printf("\t\t\t\t\t\tSYMBOL TABLE\n");
//	printf("------------------------------------------------------------------------------------------------------------------------------\n\n");
	printf(" Index        Name        Size    Return type   Data type    Type      ArgCount              Args              Scope \n");
	for(int i = 0; i < TableLength; i++) {
		Node *temp = Table[i];
		while(temp) {
			printf("%7d  %14s  %6c  %13s  %9s  %10s  %11d  ",temp->index,temp->name,temp->size,temp->rtype,temp->dtype,show(temp->type), temp-> num);
			printArgs(temp);
			printf("%6s  \n", temp->scope);
			temp = temp->next;
		}
	}


}

int hash_function(char *str) 
{
	unsigned long hash = 612;
	int c;	
	while(c = *str++) 
		hash  = hash*19 + c;

	return hash % TableLength;
}


int search(char *str)
{
	unsigned long hash_address = hash_function(str);
	Node *temp = Table[hash_address];

	while(temp) {
		if(strcmp(temp->name,str) == 0)
			return 1;
		temp = temp->next;
	}
	
	return 0;
}

void insert(Node* n)
{
	if(n->type == 0) {		//Identifier
		strcpy(n->rtype,"--");
		n->num = 0;
	}
	else{					//Function
		strcpy(n->dtype,"--");
		n->scope[0] = 'G';
	}


	if(search(n->name) && n->scope[0] == 'G')
		return;

	unsigned long hash_address = hash_function(n->name);
	
	hash_address = hash_address % 30;			//as Table length is 30
	
	
	n->next = NULL;
	
	
	if(Table[hash_address] == NULL) 
		Table[hash_address] = n;	
	
	else {
		Node *temp = Table[hash_address];
		
		while(temp->next != NULL) 
			temp = temp->next;
		
		temp->next = n;
	
	}
}
