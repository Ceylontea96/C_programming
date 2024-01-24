#include "table.h"

table* tableTop = NULL;
table* using_tb = NULL;
column* columnTop = NULL;
data* dataTop = NULL;

table* createTable(char* tbName) {
	table* newTb = (table*)malloc(sizeof(table));
	if (newTb != NULL) {
		strcpy_s(newTb->tbname, MAX, tbName);
		newTb->clink = NULL;
		newTb->link = NULL;

		if (tableTop == NULL) tableTop = newTb;
		else {
			table* tb = tableTop;
			while (tb->link != NULL) {
				tb = tb->link;
			}
			tb->link = newTb;
		}
	}
	return newTb;
}

column* dropTable(char* tbName) {
	table* preTb = NULL;
	table* tb = tableTop;
	column* comlumn = NULL;
	while (tb != NULL) {
		if (strcmp(tbName, tb->tbname) == 0) {
			if (preTb == NULL) tableTop = tb->link;
			else				preTb->link = tb->link;
			comlumn = tb->clink;
			free(tb);
			break;
		}
		preTb = tb;
		tb = tb->link;
	}
	return comlumn;
}

column* dropTable(table* tb) {
	table* nowTb = tableTop;
	table* nextTb = tb->link;
	column* comlumn = tb->clink;
	free(nowTb);
	tableTop = nextTb;
	return comlumn;
}

void showTables() {
	printf("========== Table 목록 ==========\n");
	int index = 1;
	if (tableTop == NULL) printf("[Table 목록 없음]\n");
	else {
		table* tb = tableTop;
		while (tb != NULL && tb->tbname != NULL) {
			printf("%d. %s\n", index, tb->tbname);
			tb = tb->link;
			++index;
		}
	}
	printf("==================================\n");
}

table* getTableByName(char* tbName) {
	table* target_tb = NULL;
	if (tableTop != NULL) {
		table* tb = tableTop;
		while (tb != NULL && tb->tbname != NULL) {
			if (strcmp(tbName, tb->tbname) == 0) {
				target_tb = tb;
				break;
			}
			tb = tb->link;
		}
	}
	return target_tb;
}

void selectTable(char* tbName, char* targets, char* options) {
	table* tb = getTableByName(tbName);
	int i = 0, j = 0;
	
	if (tb != NULL) {
		columnTop = tb->clink;
		column* cl = columnTop;
		data* dt = cl->dlink;
		printf("================= SELECT %s =================\n", tbName);
		if (strcmp(targets, "*") == 0) {
			while (cl != NULL) {
				printf("%s\t", cl->field);
				cl = cl->link;
			}
		}
		else {
			char* targetArr[10] = { 0, };
			while (cl != NULL) {
				printf("%s\t", cl->field);
				cl = cl->link;
			}
		}
		
		printf("\n");

		cl = columnTop;
		while (1) {
			cl = columnTop;
			while (cl != NULL) {
				dt = cl->dlink;
				i = 0;
				while (i < j) {
					dt = dt->link;
					++i;
				}
				if (dt == NULL)
					break;

				printf("%s\t", dt->data);
				cl = cl->link;
			}
			printf("\n");
			++j;
			if (dt == NULL)
				break;
		}
		printf("===============================================\n");
	}
	else printf("존재하지 않는 테이블입니다[%s]\n", tbName);
}

int updateTb() {
	return 0;
}

int deleteTb(char* tbName, char* options) {
	table* tb = getTableByName(tbName);
	column* cl = tb->clink;
	int checkIdx = 0; 
	if (options != NULL) {
		// no = 1 and pwd = 'sfd.34';
		char *cmd = NULL, *context = NULL;
		for (int i = 0; i < 3; ++i) {
			
			
		}
		int i = 1;
		char *col = NULL, *oper = NULL, *value = NULL;
		do {
			if (i == 1) cmd = strtok_s(options, " ", &context);
			else		cmd = strtok_s(NULL, " ", &context);
			
			if (i % 4 == 1)			col = cmd;
			else if (i % 4 == 2)	oper = cmd;
			else if (i % 4 == 3)	value = cmd;
			else if (i % 4 == 0) {
				// 돌면서 check 올려주는 함수 추가하기


				if (cmd != NULL && context != NULL) {
					if (_strcmpi(cmd, "and") == 0) ++checkIdx;
				}
			}

			++i;
		} while (cmd != NULL);
		//cmd[no] context[= 1 and pwd = 'sfd.34'] index 1
		//cmd[=] context[1 and pwd = 'sfd.34'] index 2
		//cmd[1] context[and pwd = 'sfd.34'] index 3
		//cmd[and] context[pwd = 'sfd.34'] index 4
		//cmd[pwd] context[= 'sfd.34'] index 5
		//cmd[=] context['sfd.34'] index 6
		//cmd['sfd.34'] context[] index 7


	}
	


	while (cl != NULL) {
		data* dt = cl->dlink;
		while (dt != NULL) {
			data* temp = dt->link;
			if (dt->check == checkIdx) dropData(cl, dt);
			dt = temp;
		}
		cl = cl->link;
	}


	resetDataCheck(tb);
	return 0;
}

column* getColumnByName(table* tb, char* ColumnName) {
	if (tb == NULL) return NULL;
	column* cl = tb->clink;
	while (cl != NULL) {
		if (strcmp(ColumnName, cl->field) == 0) {
			return cl;
		}
		cl = cl->link;
	}
	
	return NULL;
}


column* createColumn(char* name, char* type, int size) {
	column* newColumn = (column*)malloc(sizeof(column));
	if (newColumn != NULL) {
		strcpy_s(newColumn->field, MAX, name);
		strcpy_s(newColumn->type, MAX, type);
		newColumn->size = size;
		newColumn->dlink = NULL;
		newColumn->link = NULL;

		if (columnTop == NULL) columnTop = newColumn;
		else {
			column* column = columnTop;
			while (column->link != NULL) {
				column = column->link;
			}
			column->link = newColumn;
		}
	}
	return newColumn;

}


data* dropColumn(table* tb, column* Column) {
	column* nowCl = columnTop;
	column* nextCl = Column->link;
	data* dt = Column->dlink;
	free(nowCl);
	columnTop = nextCl;
	return dt;
}

int createData(column* cl, char* value) {
	if (cl == NULL) return -1;
	int index = 0;
	data* newData = (data*)malloc(sizeof(data));
	if (newData != NULL) {
		if (value == NULL)	strcpy_s(newData->data, MAX, "NULL");
		else				strcpy_s(newData->data, MAX, value);
		newData->link = NULL;

		if (cl->dlink == NULL) {
			newData->index = 0;
			cl->dlink = newData;
		}
		else {
			++index;
			data* data = cl->dlink;
			while (data->link != NULL) {
				data = data->link;
				++index;
			}
			newData->index = index;
			data->link = newData;
		}
	}
	else return -2;

	return 1;
}


void dropData(column* cl, data* target_data) {
	data* preData = NULL;
	data* data = cl->dlink;
	while (data != NULL) {
		if (data == target_data) {
			if (preData == NULL)	cl->dlink = data->link;
			else					preData->link = data->link;
			free(data);
			break;
		}
		preData = data;
		data = data->link;
	}
}

void resetDataCheck(table* tb) {
	column* cl = tb->clink;
	while (cl != NULL) {
		data* dt = cl->dlink;
		while (dt != NULL) {
			dt->check = 0;
			dt = dt->link;
		}
		cl = cl->link;
	}
}

int getNumberOfDatas(column* cl) {
	int index = 0;
	data* dt = cl->dlink;
	while (dt != NULL) {
		++index;
		dt = dt->link;
	}
	return index;
}

void getTarget(char* targets, char** arr) { // 조건을 parsing하는 함수
	
	
}