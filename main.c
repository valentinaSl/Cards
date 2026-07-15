#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define Sec_in_day 86400
#define Sec_in_3h 10800
#define Sec_in_2days 172800
#define Sec_in_4days 345600
#define Sec_in_7days 604800
struct card {
	int number;
	int subnum;
	char front[600];
	char back[600];
	time_t time;
	int level;
};
void OutputError();//OutputError(V);
void Input(int* pointnumI, int* ptrbalance); //input new words mode
void Menu(int* pointnumI, int* ptrbalance, int*ptrmoney); //main choosing mode
void Studying(int* pointnumI, int*ptrbalance); //Study mode
struct card* Memory(int* pointnumI);//allocating memory
void CardSort(int* pointnumI, struct card* list);//sorting data to struct card
void WordsForStudy_subnum(int* pointnumI, int choice, int* ptr_subnum, struct card*list);//sorts words if they are for studying
void TheStudy(int* ptr_subnum, int* pointnumI, struct card* list, int*ptrbalance, char choice);//asks the words and changes level
void GameMenu(int* ptrbalance, int*ptrmoney);
void MoneyMenu(int* ptrmoney);
void Play(int* ptrmoney, int* ptrbalance);
void Data();

int main() {
	srand((unsigned int)time(NULL));
	int balance = 0; int* ptrbalance = &balance; int money = 0; int* ptrmoney = &money;
	char choice='1';
	FILE* iptr; FILE* fptr; FILE* fdataptr; FILE* gameptr;
	//creating file if there is no such
	{
		fptr = fopen("Vocabulary.txt", "a");
		iptr = fopen("LastNum.txt", "a");
		fdataptr = fopen("VocabularyData.txt", "a");
		gameptr = fopen("GameData.txt", "a");
	}
	if ((fptr == NULL) || (iptr == NULL) || (fdataptr == NULL) || (gameptr == NULL)) {//check if the the pointer points
		OutputError(); return 1;
	}
	fclose(fdataptr); fclose(iptr); fclose(fptr); fclose(gameptr);
	int is_voc_blank, is_data_blank, is_game_blank, i = 1;
	int* pointnumI = &i;
	fptr = fopen("Vocabulary.txt", "r");
	fdataptr = fopen("VocabularyData.txt", "r");
	iptr = fopen("LastNum.txt", "r");
	gameptr = fopen("GameData.txt", "r");
	if ((fptr == NULL) || (iptr == NULL) || (fdataptr == NULL) || (gameptr == NULL)) {//check if the the pointer points
		OutputError(); return 1;
	}
	is_voc_blank = fgetc(fptr);
	is_data_blank = fgetc(fdataptr);
	is_game_blank = fgetc(gameptr);
	//check if the data is workable, also first time use i
	if (fscanf(iptr, "%d", &i) == EOF) {
		if (is_voc_blank == EOF && is_data_blank == EOF && is_game_blank == EOF) {
			i = 1;
			*ptrbalance = 0; *ptrmoney = 0;
		}
		else {
			fclose(fdataptr); fclose(fptr); fclose(iptr); fclose(gameptr);
			OutputError();
		}
	}
	rewind(gameptr); rewind(fptr); rewind(fdataptr);
	fscanf(gameptr, "%d %d", &balance, &money);
	fclose(fdataptr); fclose(fptr); fclose(iptr); fclose(gameptr);
	Menu(pointnumI, ptrbalance, ptrmoney);
	gameptr = fopen("GameData.txt", "w");
	if (gameptr == NULL) {//check if the the pointer points
		OutputError(); return 1;
	}
	fprintf(gameptr, "%d %d\n", *ptrbalance, *ptrmoney);
	fclose(gameptr);
	return 0;
}

void Menu(int* pointnumI, int* ptrbalance, int* ptrmoney) {
	char choice = '1';
	do {
		printf("What would you like to do? Adding new words A, studying S, game menu G, info F, closing #\n");
		scanf(" %c", &choice);
		while (getchar() != '\n');
		if (choice == 'A') {
		    Input(pointnumI, ptrbalance);
		}
		else if (choice == 'S') {
			Studying(pointnumI, ptrbalance);
		}
		else if (choice == 'G') {
			GameMenu(ptrbalance, ptrmoney);
		}
		else if (choice == 'F') {
			Data(pointnumI);
		}
		else if (choice == '#') {
			break;
		}
		else {
			printf("Invalid input, please choose from the given list!\n");
		}
	}
		while (choice != '#');
}

void Data(int* pointnumI){
	FILE* fdataptr;
	fdataptr = fopen("VocabularyData.txt", "r");
	if (fdataptr == NULL) {//check if the the pointer points
		OutputError(); return;
	}
	int total_words, level1, level2, level3, level4, level5, level6;
	int temporary_level, dumb;
	long long dumb2;
	for (int i=0; i<*pointnumI; i++){
		fscanf(fdataptr, "%d %%%ld+%d", &dumb, &dumb2, &temporary_level);
		if (temporary_level == 1){
			level1++;
		}
		else if (temporary_level == 2){
			level2++;
		}
		else if (temporary_level == 3){
			level3++;
		}
		else if (temporary_level == 4){
			level4++;
		}
		else if (temporary_level == 5){
			level5++;
		}
		else if (temporary_level == 6){
			level6++;
		}
	}
	printf("Total words: %d\n", *pointnumI);
	printf("Level 1 words: %d\n", level1);
	printf("Level 2 words: %d\n", level2);
	printf("Level 3 words: %d\n", level3);
	printf("Level 4 words: %d\n", level4);
	printf("Level 5 words: %d\n", level5);
	printf("Level 6 words: %d\n", level6);
	fclose(fdataptr);

}

void Input(int *pointnumI, int *ptrbalance){
	printf("Type in the front side of the card, type the symbol \"~\", type the back side of the card, and then enter to save the card.\n");
	printf("á, é, í, ó, ú, ü, ñ, ¿, and ¡\n");
	FILE* iptr; FILE* fptr;	FILE* fdataptr;
	fptr = fopen("Vocabulary.txt", "a");
	fdataptr = fopen("VocabularyData.txt", "a");
	if ((fptr == NULL) || (fdataptr == NULL)) {//check if the the pointer points
		OutputError(); return;
	}
	char word[500];
	while (fgets(word, 500, stdin) != NULL) {
		char blankstring[500];
		if (word[0] == '#') {
				break;
		}
		if (sscanf(word, "%s", blankstring) == EOF) {
			continue;
		}
		if (strchr(word, '~') == NULL) {
			printf("The word cannot be saved, there is no ~ symbol!\n");
			continue;
		}
		word[strcspn(word, "\n")] = '\0';
		fprintf(fptr, "%d. %s\n", *pointnumI, word);
		long long word_add_time = time(NULL);
		fprintf(fdataptr, "%d. %s %%%lld +1\n", *pointnumI, word, (long long)word_add_time);
		(*pointnumI)++;
		(*ptrbalance) += 2;

	}
	fclose(fptr); fclose(fdataptr);
	iptr = fopen("LastNum.txt", "w");
	if (iptr == NULL) {//check if the the pointer points
		OutputError(); return;
	}
	fprintf(iptr, "%d", *pointnumI);
	fclose(iptr);
}

void Studying(int* pointnumI, int*ptrbalance) {
	printf("Please choose the studying mode. For hard mode enter H, for easy mode enter E, for old cards enter O (Letter O).\n");
	printf("If you know the given word word enter Y, if you don\'t know the word enter N.\n");
	char choice = '1';
	scanf(" %c", &choice);
	while (getchar() != '\n');
	while (choice != 'H' && choice != 'E' && choice != 'O' && choice != '#') {
		printf("Invalid input, please choose from the given list!\n");
		scanf(" %c", &choice);
		while (getchar() != '\n');
	}
	if (choice == '#') {
		return;
	}
	int subnum = 1; int* ptr_subnum = &subnum;
	struct card* list = Memory(pointnumI);
	CardSort(pointnumI, list);
	WordsForStudy_subnum(pointnumI, choice, ptr_subnum, list);
	TheStudy(ptr_subnum, pointnumI, list, ptrbalance, choice);
	FILE* fdataptr;
	fdataptr = fopen("VocabularyData.txt", "w");
	if (fdataptr == NULL) {//check if the the pointer points
		OutputError(); return;
	}
	for (int i = 0; i < ((*pointnumI) - 1); i++) {
		fprintf(fdataptr, "%d. %s~%s%%%ld+%d\n", list[i].number, list[i].front, list[i].back, list[i].time, list[i].level);
	}
	fclose(fdataptr);
	free(list);
	list = NULL;
	return;
}

void TheStudy(int* ptr_subnum, int *pointnumI, struct card*list, int* ptrbalance, char choice) {
	char progress = '0';
	int listindx = 0, listsubnum = 0, currentlvl;
	long long timeNow = time(NULL);
	while (1) {
		if ((*ptr_subnum) == 1) {
			printf("There is no words you can study right now. Try changing study mode or adding more words :)\n");
			return;
		}
		int i = rand() % ((*ptr_subnum) - 1) + 1;
		for (int j = 0; j < (*pointnumI); j++) {
			if (list[j].subnum == i) {
				listindx = j;
			}
		}
		for (int j = 0; j < (*pointnumI); j++) {
			if (list[j].subnum == (*ptr_subnum) - 1) {
				listsubnum = j;
			}
		}
		currentlvl = list[listindx].level;
		printf("%s", list[listindx].front);
		while (getchar() != '\n');
		printf("%s\n", list[listindx].back);
		scanf(" %c", &progress);
		while (getchar() != '\n');
		while (progress != 'N' && progress != 'Y' && progress != '#') {
			printf("Invalid input, please choose from the given list!\n");
			scanf(" %c", &progress);
			while (getchar() != '\n');
		}
		if (progress == '#') {
			return;
		}
		else if ((progress == 'Y')||(progress == 'N')) {
			list[listindx].time = timeNow;
			list[listindx].subnum = 0;
			(*ptr_subnum)--;
			list[listsubnum].subnum = i;
			if ((currentlvl != 6)&&(progress == 'Y')) {
				currentlvl++;
				list[listindx].level = currentlvl;
			}
			else if ((currentlvl != 1) && (progress == 'N')) {
				currentlvl--;
				list[listindx].level = currentlvl;
			}
		}
		if (choice == 'H') {
			(*ptrbalance) += 2;
		}
		else if ((choice == 'E') || (choice == 'O')) {
			(*ptrbalance) ++;
		}
	}
}
	
void WordsForStudy_subnum(int* pointnumI, int choice, int *ptr_subnum, struct card *list) {
	long long timeNow = time(NULL);
	for (int i = 0; i < (*pointnumI-1); i++) {
		long long interval = timeNow - list[i].time;
		if ((choice == 'H') && (((list[i].level == 1) && (interval >= Sec_in_3h)) || ((list[i].level == 2) && (interval >= Sec_in_day)))) {
			list[i].subnum = *ptr_subnum;
			(*ptr_subnum)++;
		}
		else if ((choice == 'E') && (((list[i].level == 3) && (interval >= Sec_in_2days)) || ((list[i].level == 4) && (interval >= Sec_in_4days))||((list[i].level == 5) && (interval >= Sec_in_7days)))) {
			list[i].subnum = *ptr_subnum;
			(*ptr_subnum)++;
		}
		else if ((choice == 'O') && (list[i].level == 6)) {
			list[i].subnum = *ptr_subnum;
			(*ptr_subnum)++;
		}
	}
}

void CardSort(int* pointnumI, struct card *list) {
	FILE* fdataptr;
	fdataptr = fopen("VocabularyData.txt", "r");
	if (fdataptr == NULL) {//check if the the pointer points
		OutputError(); return;
	}
	int temp1, temp2, current_index = 0;
	char whole_line[700];
	while (fgets(whole_line, 700, fdataptr) != NULL) {
		sscanf(whole_line, "%d", &list[current_index].number);
		list[current_index].subnum = 0;
		temp1 = (int)strcspn(whole_line, ".");
		temp2 = (int)strcspn(whole_line, "~");
		int l = 0;
		for (int j = temp1 + 2; j < temp2; j++) {
			list[current_index].front[l] = whole_line[j];
			l++;
		}
		list[current_index].front[l] = '\0';
		temp1 = temp2;
		temp2 = (int)strcspn(whole_line, "%");
		l = 0;
		for (int j = temp1 + 1; j < temp2; j++) {
			list[current_index].back[l] = whole_line[j];
			l++;
		}
		list[current_index].back[l] = '\0';
		sscanf(&whole_line[temp2 + 1], "%ld +%d", &list[current_index].time, &list[current_index].level);
		current_index++;
	}
	fclose(fdataptr);
}

struct card* Memory(int* pointnumI) {
	int capacity = 50;
	struct card* list = malloc(capacity * sizeof(struct card));
	if (list == NULL) {
		OutputError();
	}
	while (*pointnumI >= capacity) {
		capacity *= 2;
		struct card* temp = realloc(list, capacity * sizeof(struct card));
		if (temp != NULL) {
			list = temp;
		}
		else {
			OutputError();
		}
	}
	return list;
}

void GameMenu(int* ptrbalance, int*ptrmoney) {
	printf("This is Game Menu.\n Your balance is %d. What would you like to do now?\n", *ptrbalance);
	char choice='1';
	do {
		printf("For entering your money menu enter M, for playing game enter P, to go back to menu enter #\n");
		scanf(" %c", &choice);
		while (getchar() != '\n');
		if (choice == 'M') {
			MoneyMenu(ptrmoney);
		}
		else if (choice == 'P') {
			Play(ptrmoney, ptrbalance);
		}
		else if (choice == '#') {
			break;
		}
		else {
			printf("Invalid input, please choose from the given list!\n");
		}
	} while (choice != '#');
}

void MoneyMenu(int* ptrmoney) {
	printf("You are inside money menu.\n Your money balance is: %d. What would you like to do?\n", *ptrmoney);
	char choice2 = '1';
	do {
		printf("For updating money use enter B, to go back to game menu enter #\n");
		scanf(" %c", &choice2);
		while (getchar() != '\n');
		if (choice2 == 'B') {
			if ((*ptrmoney) < 0) {
				printf("Sorry, you are in debt. You cannot spend money :(\n");
				return;
			}
			printf("Please input the amount of money you spent\n");
			int use = 0;
			scanf(" %d", &use);
			while (getchar() != '\n');
			*ptrmoney -= use;
			printf("Your updated money balance is: %d\n", *ptrmoney);
		}
		else if (choice2 == '#') {
			break;
		}
		else {
			printf("Invalid input, please choose from the given list!\n");
		}
	} while (choice2 != '#');
}

void Play(int* ptrmoney, int* ptrbalance) {
	printf("Now you can play the game with points you earned while studying! Your balance: %d \n", *ptrbalance);
	printf("If you would like to continue enter Y\n");
	char choice = '1';
	scanf(" %c", &choice);
	while (getchar() != '\n');
	while (choice != 'Y' && choice != '#') {
		printf("Invalid input, please choose from the given list!\n");
		scanf(" %c", &choice);
		while (getchar() != '\n');
	}
	if (choice == '#') {
		return;
	}
	if (choice == 'Y') {
		if (*ptrbalance < 100) {
			printf("Unfortunately you have not enough points to play. Add some new words or study more!\n");
			return;
		}
		(*ptrbalance) -= 100;
		int i = rand() % (100);
		int guess;
		scanf("%d", &guess);
		int idecade = i / 10; int guess_decade = guess / 10;
		while (getchar() != '\n');
		if (guess == i) {
			printf("Congratulations! You guessed the number and get 100 moneys!");
			(*ptrmoney) += 100;
		}
		else if (idecade == guess_decade) {
			printf("Congratulations! You guessed the decade and get 10 moneys! The exact number was: %d", i);
			(*ptrmoney) += 10;
		}
		else {
			printf("Unfortunately you didnt guess... The picked number was: %d", i);
		}
	}
}

void OutputError() {
	char V[] = "+972-55992****";
	printf("Ooops... something is wrong, call %s", V);
	while (getchar() != '\n');
	exit(1);
}