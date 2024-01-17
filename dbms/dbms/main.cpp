#include "main.h"


int main() {
	
	
	while (1) {
		
		
		
		if (SYSTEM_STATUS == EXIT) exit(0);
		else if (SYSTEM_STATUS == LOGIN) loginMenu();
		else if (SYSTEM_STATUS == DATABASE) {
			DBMenu();
		}
		else if (SYSTEM_STATUS == TABLE) {

		}
		
	
	}
	
	return 0;
}

//create db, use db
// 
//create table tb1(id varchar(100), pwd varchar(100), no int(10));
//insert into tb1(id, pwd, no) values('user', 'user1234', 1);
//select * from tb1;
//select id,pwd from tb1;





