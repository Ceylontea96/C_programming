#include "main.h"

struct node
{
	
};

char* db_pointer;

int main() {
	init_stack();

	while (1) {
		printf("========== ���� �޴� ==========\n");
		if (loginMenu() == -1) exit(0);

		while (1) {
			int dbResult = DbMenu();	// db ���� �ڵ� �ϼ��ϱ�
			if (dbResult == 5) {	//�α׾ƿ�
				login_user = NULL;
				break;
			}
			else if (dbResult == -1) exit(0);	//���α׷� ����
			else if (dbResult == 0) {
				while (1) {
					int tableResult = tableMenu();

					if (tableResult == 5) {	// ���ư���
						using_db = NULL;
						break;
					}
					else if (tableResult == -1) exit(0);	//���α׷� ����
					else if (tableResult == 0) {

					}
				}
			}

		}
		
		
		


		printf("111\n");


	}
	
	return 0;
}

//create db, use db
// 
//create table tb1(id varchar(100), pwd varchar(100), no int(10));
//insert into tb1(id, pwd, no) values('user', 'user1234', 1);
//select * from tb1;
//select id,pwd from tb1;


void init_stack() {
	userTop = (userHead*)malloc(sizeof(userHead));
	userTop->link = NULL;
	dbTop = (dbHead*)malloc(sizeof(dbHead));
	dbTop->link = NULL;
	tableTop = (tableHead*)malloc(sizeof(tableHead));
	tableTop->link = NULL;
	columnTop = (columnHead*)malloc(sizeof(columnHead));
	columnTop->link = NULL;
}


