#pragma once
#include "user.h"
#include "database.h"
#include "table.h"
#include "column.h"

user* login_user;
userHead* userTop;
database* using_db;
dbHead* dbTop;
tableHead* tableTop;
columnHead* columnTop;

void init_stack();
