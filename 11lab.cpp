#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10u
char bufRus[256];
char* Rus(const char*text)
{
    CharToOem(text,bufRus);
    return bufRus;   
}
char path[1000] = "~";
struct Files{
	int id;
	int size;
	int type;
	char *name;
	char *path;
};
int fileId = 0;
struct Blocks{
	int id;
	int bitVector;
	int idFile;
	int place;
};
int blockId = 0;
struct Nodes{
	int id;
	int bitVector;
	int idFile;
};
int nodeId = 0;
struct SuperBlock{	
	char typeSystem[5];
	int countBlocks;
	int sizeBlock;
	int countNodes;
	
	Blocks *blocks;
	int countFreeBlocks;
	
	Nodes *nodes;
	int countFreeNodes;
	
	Files *files;
	int countFiles;	
};
int getpos(char *str);
void showInfo(SuperBlock superBlock);
void numerationNodes(SuperBlock *superBlock);
void numerationBlocks(SuperBlock *superBlock);
void printBlocks(SuperBlock superBlock);
void sortBlocks(SuperBlock *superBlock);
void printNodes(SuperBlock superBlock);
void sortNodes(SuperBlock *superBlock);
void addFile(SuperBlock *superBlock);
void showBlocks(SuperBlock superBlock);
void showNodes(SuperBlock superBlock);
void changeDirectory(SuperBlock superBlock);
void showFile(SuperBlock superBlock);
void deleteFile(SuperBlock *superBlock);
int main()
{
	SuperBlock superBlock;
	strcpy(superBlock.typeSystem,"NTFS");
	do{
    printf(Rus("Введите количество блоков диска:\n"));
	   scanf("%d",&superBlock.countBlocks);	
	   printf(Rus("\nВведите размер блока диска:\n"));
	   scanf("%d",&superBlock.sizeBlock);	
	   printf(Rus("\nВведите количество узлов диска:\n"));
	   scanf("%d",&superBlock.countNodes);	
	   system("cls");      
    } while(superBlock.countBlocks <= 0 || superBlock.sizeBlock <= 0 || superBlock.countNodes <= 0);
	
	
	superBlock.blocks = (struct Blocks*)malloc(superBlock.countBlocks * sizeof(Blocks));
	superBlock.countFreeBlocks = superBlock.countBlocks;
	numerationBlocks(&superBlock);
	superBlock.nodes = (struct Nodes*)malloc(superBlock.countNodes * sizeof(Nodes));
	superBlock.countFreeNodes = superBlock.countNodes;
	numerationNodes(&superBlock);
	
	superBlock.countFiles = 0;
	int action;
	while(true){
		printf(Rus("Директория: %s\n"),path);
		printf(Rus("              Menu\n"));
        printf(Rus(" ------------------------------\n"));
        printf(Rus("|1)Инофрмация о системе.       |\n"));
        printf(Rus("|2)Добавить файл.              |\n"));
        printf(Rus("|3)Удалить файл.               |\n"));
        printf(Rus("|4)Информация о файлах.        |\n"));
        printf(Rus("|5)Перейти в директорию.       |\n"));
        printf(Rus("|6)Информация о блоках.        |\n"));
        printf(Rus("|7)Информация о узлах.         |\n"));
        printf(Rus("|0)Выход.                      |\n"));
        printf(Rus(" ------------------------------\n"));
        
        printf(Rus("\n\nВыберите действие.\n"));
		scanf("%d",&action);
		system("cls");
		
		switch(action){
			case 1:
				showInfo(superBlock);
				break;
			case 2:
				addFile(&superBlock);		
				break;
			case 3:
				deleteFile(&superBlock);
				break;
			case 4:
				showFile(superBlock);
				break;
			case 5:
				changeDirectory(superBlock);
				break;
			case 6:
				showBlocks(superBlock);
				break;
			case 7:
				showNodes(superBlock);
				break;
			case 0:
				return 0;
			default:
				printf(Rus("Такого действия нет.\n"));
				printf(Rus("Нажмите любую клавишу для продолжения.\n"));
				getch();
		}
		fflush(stdin);
		system("cls");
	}
	getch();
	return 0;
}

void addFile(SuperBlock *superBlock){
	char name[101], choose[2];
	int size, new_node = 1, count_block;
	if(strcmp(path,"~") == 0) new_node = 0;
	int flag = -1;		
	printf(Rus("1. Создание каталога.\n2. Создание файла\n"));
	scanf("%1s",&choose);
	
	
	if(strcmp(choose,"1") == 0){
		printf(Rus("Введите имя каталога:\n"));
		scanf("%100s",&name);
		if(getpos(name) == 0 && strlen(name) != 0){
			flag = 0;			
		}  
		else printf(Rus("Имя введено неверно\n"));
				
	} 
	if(strcmp(choose,"2") == 0){
		printf(Rus("Введите имя файла:\n"));
		scanf("%100s",&name);
		if(getpos(name) == 0 && strlen(name) != 0){
			printf(Rus("Введите размер файла:\n"));
			scanf("%d",&size);
			if(size >= 0){
				if(size / superBlock->sizeBlock * superBlock->sizeBlock == size){
					count_block = size / superBlock->sizeBlock;
					if(!count_block) count_block++;
				} 
				else count_block = size / superBlock->sizeBlock + 1;
				if(count_block <= superBlock->countFreeBlocks){
					flag = 1;				
				} else printf(Rus("Недостаточно памяти\n"));
			} else printf(Rus("Размер введён неверно\n"));
		} else printf(Rus("Имя введено неверно\n"));						
	}
	if(flag >= 0){
		if(superBlock->countFreeNodes > 0 || strcmp(path,"~") != 0){
			if(superBlock->countFiles == 0)
				superBlock->files = (struct Files*)malloc(1 * sizeof(Files));
			else 
				superBlock->files = (struct Files*)realloc(superBlock->files,(superBlock->countFiles + 1) * sizeof(Files));
				
			superBlock->files[superBlock->countFiles].id = ++fileId;
			if(flag){
				superBlock->files[superBlock->countFiles].type = 1;
				superBlock->files[superBlock->countFiles].size = size;
			} else {
				superBlock->files[superBlock->countFiles].type = 0;
				superBlock->files[superBlock->countFiles].size = 0;
			}						
			superBlock->files[superBlock->countFiles].name = new char[strlen(name)+1];
			strcpy(superBlock->files[superBlock->countFiles].name,name);
			superBlock->files[superBlock->countFiles].path = new char[strlen(path)+1];
			strcpy(superBlock->files[superBlock->countFiles].path,path);
			strcat(superBlock->files[superBlock->countFiles].path,"\\");			
			superBlock->countFiles++;
			if(flag){
				for(int i = 0; i < count_block; i++){
					superBlock->blocks[i].bitVector = 1;
					superBlock->blocks[i].idFile = fileId;
					superBlock->blocks[i].place = i;
					superBlock->countFreeBlocks--;
				}
				sortBlocks(superBlock);	
			}				
			if(!new_node){
				superBlock->nodes[0].bitVector = 1;
				superBlock->nodes[0].idFile = fileId;
				superBlock->countFreeNodes--;
				sortNodes(superBlock);
			}								
		} else printf(Rus("Нет свободных узлов\n"));	
	}
	printf(Rus("\nНажмите любую кнопку для продолжения\n"));
	getch();
}
void showInfo(SuperBlock superBlock){
	printf(Rus("Тип системы:      %s\n"),superBlock.typeSystem);
	printf(Rus("Всего узлов:      %d\n"),superBlock.countNodes);
	printf(Rus("Свободных узлов:  %d\n"),superBlock.countFreeNodes);
	printf(Rus("Всего блоков:     %d\n"),superBlock.countBlocks);
	printf(Rus("Свободных блоков: %d\n"),superBlock.countFreeBlocks);
	printf(Rus("Размер блока:     %d\n"),superBlock.sizeBlock);
	printf(Rus("Файлов:           %d\n"),superBlock.countFiles);
	printf(Rus("\n\nНажмите любую клавишу для продолжения.\n"));
	getch();
	system("cls");
}
void numerationNodes(SuperBlock *superBlock){
	for(int i = 0; i < superBlock->countNodes; i++){
		nodeId++;
		superBlock->nodes[i].id = nodeId;
		superBlock->nodes[i].bitVector = 0;
		superBlock->nodes[i].idFile = NULL;
	}
}
void numerationBlocks(SuperBlock *superBlock){
	for(int i = 0; i < superBlock->countBlocks; i++){
		blockId++;
		superBlock->blocks[i].id = blockId;
		superBlock->blocks[i].bitVector = 0;
		superBlock->blocks[i].idFile = NULL;
		superBlock->blocks[i].place = NULL;
	}
}
void sortBlocks(SuperBlock *superBlock){
	Blocks block;
	for(int i = 0; i < superBlock->countBlocks-1; i++){
		for(int j = i+1; j < superBlock->countBlocks; j++){
			if(superBlock->blocks[j].bitVector < superBlock->blocks[i].bitVector){
				block = superBlock->blocks[i];
				superBlock->blocks[i] = superBlock->blocks[j];
				superBlock->blocks[j] = block;
			}
		}	
	}
	for(int i = superBlock->countBlocks-1; i >= 1 && superBlock->blocks[i].bitVector != 0; i--){
		for(int j = i - 1; j >= 0 && superBlock->blocks[j].bitVector != 0; j--){
			if(superBlock->blocks[j].idFile > superBlock->blocks[i].idFile){
				block = superBlock->blocks[i];
				superBlock->blocks[i] = superBlock->blocks[j];
				superBlock->blocks[j] = block;
			}
		}
	}
	for(int i = superBlock->countBlocks-1; i >= 1 && superBlock->blocks[i].bitVector != 0; i--){
		for(int j = i - 1; j >= 0 && superBlock->blocks[j].bitVector != 0; j--){
			if(superBlock->blocks[i].idFile == superBlock->blocks[j].idFile && superBlock->blocks[j].place > superBlock->blocks[i].place){
				block = superBlock->blocks[i];
				superBlock->blocks[i] = superBlock->blocks[j];
				superBlock->blocks[j] = block;
			}	
		}
	}
}
void sortNodes(SuperBlock *superBlock){
	Nodes nodes;
	for(int i = 0; i < superBlock->countNodes-1; i++){
		for(int j = i+1; j < superBlock->countNodes; j++){
			if(superBlock->nodes[j].bitVector < superBlock->nodes[i].bitVector){
				nodes = superBlock->nodes[i];
				superBlock->nodes[i] = superBlock->nodes[j];
				superBlock->nodes[j] = nodes;
			}
		}	
	}
	for(int i = superBlock->countNodes-1; i >= 1 && superBlock->nodes[i].bitVector != 0; i--){
		for(int j = i - 1; j >= 0 && superBlock->nodes[j].bitVector != 0; j--){
			if(superBlock->nodes[i].id < superBlock->nodes[j].id){
				nodes = superBlock->nodes[i];
				superBlock->nodes[i] = superBlock->nodes[j];
				superBlock->nodes[j] = nodes;
			}
		}
	}
	for(int i = 0; i < superBlock->countNodes-1 && superBlock->nodes[i].bitVector == 0; i++){
		for(int j = i + 1; j < superBlock->countNodes && superBlock->nodes[j].bitVector == 0; j++){
			if(superBlock->nodes[i].id > superBlock->nodes[j].id){
				nodes = superBlock->nodes[i];
				superBlock->nodes[i] = superBlock->nodes[j];
				superBlock->nodes[j] = nodes;
			}
		}
	}
}
void showBlocks(SuperBlock superBlock){
	printf(" id   bitVector  idFile  Place\n");
	for(int i = 0; i < superBlock.countBlocks; i++){
		printf("%-4d  %-9d  %-6d  %-5d\n",superBlock.blocks[i].id,superBlock.blocks[i].bitVector,superBlock.blocks[i].idFile,superBlock.blocks[i].place);
	}
	printf(Rus("\nНажмите любую кнопку для продолжения\n"));
	getch();
}
void showNodes(SuperBlock superBlock){
	printf(" id   bitVector  idFile\n");
	for(int i = 0; i < superBlock.countNodes; i++){
		printf("%-4d  %-9d  %-6d\n",superBlock.nodes[i].id,superBlock.nodes[i].bitVector,superBlock.nodes[i].idFile);
	}
	printf(Rus("\nНажмите любую кнопку для продолжения\n"));
	getch();
}
void showFile(SuperBlock superBlock){
	for(int i = 0; i < superBlock.countFiles; i++){
		printf("%s%s",superBlock.files[i].path, superBlock.files[i].name);
		if(superBlock.files[i].type) printf(" (file)\n");
		else printf(" (katalog)\n");
	}
	printf(Rus("\nНажмите любую кнопку для продолжения\n"));
	getch();
}

void changeDirectory(SuperBlock superBlock){
	char _path[1000], _path2[1000];
	printf(Rus("Перейти в директорию:\n"));
	scanf("%s",&_path);
	if(strcmp("~",_path) != 0){
		for(int i = 0; i < superBlock.countFiles; i++){
			strcpy(_path2,superBlock.files[i].path);
			strcat(_path2,superBlock.files[i].name);
			if(strcmp(_path2,_path) == 0 && superBlock.files[i].type == 0){
				strcpy(path,_path);
			}
		}	
	} else{
		strcpy(path,"~");
	} 
}
void deleteFile(SuperBlock *superBlock){
	char _path[1000], _path2[1000];
	int node_flag = 0;
	int flag_del = 1;
	int count = 0, flag = 0;
	if(strcmp(path,"~") == 0) node_flag = 1;
	
	printf(Rus("Введите имя удаляемого файла:\n"));
	scanf("%s",&_path2);
	strcpy(_path,path);
	strcat(_path,"\\");
	strcat(_path,_path2);
	
	for(int i = 0; i < superBlock->countFiles; i++){
		if(flag_del){
			strcpy(_path2,superBlock->files[i].path);
			strcat(_path2,superBlock->files[i].name);
			if(strcmp(_path,_path2) == 0){
				flag_del = 0;
				flag = 1;
			}
		} else {
			strcpy(_path2,superBlock->files[i].path);
			if(strncmp(_path,_path2,strlen(_path)) == 0) flag = 1;
		}
		if(flag){
			if(superBlock->files[i].type == 1){
				for(int j = superBlock->countBlocks - 1; j >= superBlock->countFreeBlocks; j--){
					if(superBlock->blocks[j].idFile == superBlock->files[i].id){
						superBlock->blocks[j].bitVector = 0;
						superBlock->blocks[j].idFile = 0;
						superBlock->blocks[j].place = 0;
						count++;						
					}
				}
			    superBlock->countFreeBlocks += count;
			    count = 0;
			    sortBlocks(superBlock);
			}
			if(node_flag == 1){
				for(int j = superBlock->countNodes - 1; j >= superBlock->countFreeNodes; j--){
					if(superBlock->nodes[j].idFile == superBlock->files[i].id){
						superBlock->nodes[j].bitVector = 0;
						superBlock->nodes[j].idFile = 0;
						superBlock->countFreeNodes++;
						node_flag == 0;
						break;						
					}
				}
				sortNodes(superBlock);
			}
			for(int j = i+1; j < superBlock->countFiles; j++){
				superBlock->files[j-1] = superBlock->files[j];
			}
			superBlock->files = (struct Files*)realloc(superBlock->files,(--superBlock->countFiles) * sizeof(Files));
			i--;
			flag = 0;
		}		
	}
}
void fileSort2(SuperBlock *superBlock, int *count, char *path_file){
	Files files;
	char path[1000]; 
	for(int i = *count; i < superBlock->countFiles; i++){
		
	}
}
int getpos(char *str){
	int pos = 0;
	if(strchr(str,'\\')) pos = strchr(str,'\\')-str+1;
	return pos;
}

