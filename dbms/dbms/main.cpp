#include "main.h"



int main() {

	while (1) {
		printf("========== ���� �޴� ==========\n");
		if (loginMenu() == -1) exit(0);

		while (1) {
			int dbResult = DBMenu();	// db ���� �ڵ� �ϼ��ϱ�
			if (dbResult == -1) {	//�α׾ƿ�
				login_user = NULL;
				break;
			}
			else if (dbResult == -1) exit(0);	//���α׷� ����
			else {

				/*
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
				*/
				
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





