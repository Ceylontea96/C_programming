#include "main.h"

int main() {
	while (1) {
		if (SYSTEM_STATUS == EXIT) exit(0);
		else if (SYSTEM_STATUS == LOGIN) loginMenu();
		else if (SYSTEM_STATUS == DATABASE) DBMenu();
		else if (SYSTEM_STATUS == TABLE) tableMenu();
	}
	
	return 0;
}







