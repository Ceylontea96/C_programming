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

void selectTable(char* tbName, char* option) {
	
	table* tb = getTableByName(tbName);
	int i = 0, j = 0;
	
	
	if (tb != NULL) {
		columnTop = tb->clink;
		column* cl = columnTop;
		data* dt = cl->dlink;
		printf("================= SELECT %s =================\n", tbName);
		while (cl != NULL) {
			printf("%s ", cl->field);
			cl = cl->link;
		}
		printf("\n");
		//printf("--------------------------------------------\n");

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
		//printf("--------------------------------------------\n");
		printf("===============================================\n");
	}
	else printf("존재하지 않는 테이블입니다[%s]\n", tbName);
	

	

}

int insertTb() {


	return 0;
}

int updateTb() {
	return 0;
}

int deleteTb() {
	return 0;
}

int selectTb() {
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


data* dropColumn(column* Column) {
	column* nowCl = columnTop;
	column* nextCl = Column->link;
	data* dt = Column->dlink;
	free(nowCl);
	columnTop = nextCl;
	return dt;
}





int createData(column* cl, char* value) {
	if (cl == NULL) return -1;
	data* newData = (data*)malloc(sizeof(data));
	if (newData != NULL) {
		strcpy_s(newData->data, MAX, value);
		newData->link = NULL;

		if (cl->dlink == NULL) cl->dlink = newData;
		else {
			data* data = cl->dlink;
			while (data->link != NULL) {
				data = data->link;
			}
			data->link = newData;
		}
	}
	else return -2;

	return 1;
}



void dropData(data* dt) {
	data* nowDt = dataTop;
	data* nextDt = dt->link;
	free(nowDt);
	dataTop = nextDt;
}