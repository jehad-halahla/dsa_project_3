#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct data{
    char key[50];
    int credit;
    char courseCode[50];
    char departmentName[50];
    char topics[500];
}data;

typedef struct course{
    struct data* info;
    int state;
    //-1 == DELETED
    // 0 == EMPTY
    // 1 == OCCUPIED

}hashTableItem;
//needed global variables
int capacity = 10;
int doubleCapacity =10;
int size1 = 0;
int numberOfCollisions=0;
int numberOfCollisionsDouble = 0 ;
int size2 = 0;

hashTableItem * hashTable1;//this table will use the linear probing method
hashTableItem * hashTable2;//this will use the double hashing method

data* makeDataBlock(data* newData,char key[],int credit,char courseCode[],char departmentName[],char topics[]);
float getLoadFactor(hashTableItem *hashTable);
hashTableItem* reHash(hashTableItem* hashTable);
//prime methods
int ifPrime(int n);
int getPrime(int n);
//methods that will be used
void readFile();
int generateKey(char courseName[50]);//using ascii sum of characters
int hashCode(int key);
void printMenu();
void initiateArrays();
void insert(hashTableItem* hashTable, data* newData);
void printTable(hashTableItem* hashTable);
void printTableDouble(hashTableItem* hashTable);
int linearProbe(int n);
void insertFromUser();
int searchSpecificTable();
void deleteRecord(char courseName []);
int hashCode2(int key);
int hashCode1(int key);
int getPrimeLess(int n);
void insertDouble(hashTableItem* hashTable, data* newData);
void deleteRecordDouble(char courseName[]);
float getLoadFactorDouble(hashTableItem *hashTable);
hashTableItem* reHashDouble(hashTableItem* hashTable);
void printOnFile();


int main(){
    initiateArrays();
     readFile();
    do{
        printMenu();
        int choice;
        scanf("%d",&choice);
        switch (choice) {
            case 1:{
                printf("\nTABLE 1 (LINEAR_PROBE)\n");
                printTable(hashTable1);
                printf("\nTABLE 2 (DOUBLE_HASHING)\n");
                printTableDouble(hashTable2);
                break;
            }
            case 2:{
                printf("\nTABLE 1 (LINEAR_PROBE)\n");
                printf("CAPACITY: %d \t LOAD FACTOR: %.2f\n",capacity, getLoadFactor(hashTable1));
                printf("\nTABLE 2 (DOUBLE_HASHING)\n");
                printf("CAPACITY: %d \t LOAD FACTOR: %.2f\n",doubleCapacity, getLoadFactorDouble(hashTable2));
                break;
            }
            case 3:{
                printf("\t\t\tLINEAR_PROBING\t\t\tDOUBLE_HASHING");
                printf("\n=======================================================================\n");
                printf("initial hashing\t\tkey MOD capacity\t\tkey MOD capacity\n");
                printf("probing function\th2 = prime - key MOD prime\t(n+1) MOD capacity\n");



                break;
            }
            case 4:{
                insertFromUser();
                if(getLoadFactor(hashTable1)> 0.69){
                    hashTable1 = reHash(hashTable1);
                }
                if(getLoadFactorDouble(hashTable2)> 0.69){
                    hashTable2 = reHashDouble(hashTable2);
                }
                break;
            }
            case 5:{
                printf("%d",searchSpecificTable());
                break;
            }
            case 6:{
                char courseName[50];
                printf("please enter course name you want to delete\n");
                getchar();
                gets(courseName);
                deleteRecord(courseName);
                deleteRecordDouble(courseName);
                break;
            }
            case 7:{
                if(numberOfCollisions > numberOfCollisionsDouble){
                    printf("DOUBLE_HASHING >>> LINEAR_PROBING\n");
                    printf("===============================================\n");
                    printf("\t%d\t\t\t%d",numberOfCollisionsDouble,numberOfCollisions);
                }
                else{
                    printf("DOUBLE_HASHING <<< LINEAR_PROBING\n");
                    printf("===============================================\n");
                    printf("\t%d\t\t\t%d",numberOfCollisionsDouble,numberOfCollisions);

                }
                break;
            }
            case 8:{
                printOnFile();
                break;
            }
            case 9:{
                printf("thanks for using this program %c",1);
                exit(0);
                break;
            }

        }
    }while(1);
    return 0;
    }

void printMenu(){
    printf("\n====================================================================================\n"
           "1. Print hashed tables (including empty spots).\n"
           "2. Print out table size and the load factor.\n"
           "3. Print out the used hash functions.\n"
           "4. Insert a new record to hash table (insertion will be done on both hash tables).\n"
           "5. Search for a specific word (specify which table to search in).\n"
           "6. Delete a specific record (from both tables).\n"
           "7. Compare between the two methods in terms of number of collisions occurred.\n"
           "8. Save hash table back to a file named saved_courses.txt (of the double hashing)\n"
           "9. Exit Program\n"
           "====================================================================================\n");
}
int hashCode(int key){
    //the used hash code for table 1
    return (key%capacity);
}
int generateKey(char courseName []){
    int key = 0 ;
    for(char *temp= courseName ; *temp!= '\0'; temp++){
        key += *temp;
    }
    return key;
}
void initiateArrays(){
    int i;
    //allocate memory space dynamically, important for re-hashing
    capacity = getPrime(capacity);
    doubleCapacity = getPrime(doubleCapacity);
    hashTable1 = (hashTableItem*) malloc(capacity * sizeof(hashTableItem));
    hashTable2 = (hashTableItem*) malloc(doubleCapacity * sizeof(hashTableItem));

    for (i = 0; i < capacity; i++){
        hashTable1[i].state = 0;
        hashTable1[i].info = NULL;
    }
        for(i = 0 ; i < doubleCapacity ; i++ ){
            hashTable2[i].state = 0;
            hashTable2[i].info = NULL;
        }

}
//gets the next prime number after given n
int getPrime(int n){
    if (n % 2 == 0){
        n++;
    }
    for (; !ifPrime(n); n += 2);
    return n;
}

/* to check if given input (i.e n) is prime or not */
int ifPrime(int n){
    int i;
    if ( n == 1  ||  n == 0){
        return 0;
    }
    for (i = 2; i < n; i++){
        if (n % i == 0){
            return 0;
        }
    }
    return 1;
}




void readFile(){
        FILE* inFile = fopen("offered_courses.txt","r");
        char line [1000];
        char key[50];
        int credit;
        char courseCode[50];
        char departmentName[50];
        char topics[500];
        while(fgets(line, 1000 , inFile)){
            sscanf(line,"%[^:]:%d#%[^#]#%[^/]/%[^\n]",key,&credit,courseCode,departmentName,topics);
            data *newData = NULL;
            newData = makeDataBlock(newData,key,credit,courseCode,departmentName,topics);
            //rehash if the load factor exceeds desired max for both tables
            if(getLoadFactor(hashTable1) > 0.69)
                hashTable1 = reHash(hashTable1);
            if(getLoadFactorDouble(hashTable2) > 0.69)
                hashTable2 = reHashDouble(hashTable2);
            insert(hashTable1,newData);
            insertDouble(hashTable2,newData);
        }
}
data* makeDataBlock(data* newData, char key[],int credit,char courseCode[],char departmentName[],char topics[]){
    newData =(data*)malloc(sizeof(data));
    strcpy(newData->key,key);
    strcpy(newData->courseCode,courseCode);
    strcpy(newData->departmentName,departmentName);
    strcpy(newData->topics,topics);
    newData->credit = credit;
   return  newData;
}
void insert(hashTableItem* hashTable ,data* newData){
    //insert, and probe at table one and using linear probing to resolve collisions
int index = hashCode(generateKey(newData->key));
int i = index;
while(hashTable[i].state == 1){
    if(strcmp(hashTable[i].info->key,newData->key)==0){
        hashTable[i].info = newData;//update already existing value
        return;
    }
    numberOfCollisions++;
    i = linearProbe(i);//linear probing
   if(i == index){
      printf("cannot insert, the table is full\n");
       return;
   }

    }
//if it wasn't inserted at the original index, then a  collision happened
//if(i != index)
    //numberOfCollisions++;
    hashTable[i].state = 1;
    hashTable[i].info = newData;
    size1++;
}
//print empty,deleted, and Occupied buckets
void printTable(hashTableItem* hashTable){
    for(int i = 0 ; i < capacity ; i++){
        if(hashTable[i].state == 1 )
            printf("%02d)\t%s %d %s %s\n",i,hashTable[i].info->key,hashTable[i].info->credit,hashTable[i].info->courseCode,hashTable[i].info->departmentName);
        else if (hashTable[i].state == 0)
            printf("%02d)\t-----\n",i);
        else if(hashTable[i].state == -1)
            printf("%02d)\tDEL\n",i);
    }
    printf("\nnumber of collisions: %d \n",numberOfCollisions);
}
void printTableDouble(hashTableItem* hashTable){
    for(int i = 0 ; i < doubleCapacity ; i++){
        if(hashTable[i].state == 1 )
            printf("%02d)\t%s %d %s %s\n",i,hashTable[i].info->key,hashTable[i].info->credit,hashTable[i].info->courseCode,hashTable[i].info->departmentName);
        else if (hashTable[i].state == 0)
            printf("%02d)\t-----\n",i);
        else if(hashTable[i].state == -1)
            printf("%02d)\tDEL\n",i);
    }
    printf("\nnumber of collisions: %d \n",numberOfCollisionsDouble);
}
int linearProbe(int n){
    return (n+1)%capacity;
}
void insertFromUser(){
    char line[1000];
    char key[50];
    int credit;
    char courseCode[50];
    char departmentName[50];
    char topics[500];
    printf("please enter record info in this form:\n");
    printf("course name:credit#course code#department/topic1,topic2,...topicN\n");
    getchar();
    fgets(line,1000,stdin);
    sscanf(line,"%[^:]:%d#%[^#]#%[^/]/%[^\n]",key,&credit,courseCode,departmentName,topics);
    data* newData;
    newData = makeDataBlock(newData,key,credit,courseCode,departmentName,topics);
    insert(hashTable1,newData);
    insertDouble(hashTable2,newData);
}
int searchSpecificTable(){
    int choice;
    char courseName [50];
    printf("choose table:\n");
    printf("1 for table 1, any other number for table 2:\n");
    scanf("%d",&choice);
    switch (choice) {
        case 1: {
            printf("please enter the course name you want to find\n");
            getchar();
            gets(courseName);
            int index = hashCode(generateKey(courseName));
            int i = index;
            while (hashTable1[i].state == 1) {
                if (strcmp(hashTable1[i].info->key,courseName) == 0) {
                    return i;
                }
                i = linearProbe(i);//linear probing
            }
            return -1;
        }
        default:{
            printf("please enter the course name you want to find\n");
            getchar();
            gets(courseName);
            int hash1 = hashCode1(generateKey(courseName));
            int hash2 = hashCode2(generateKey(courseName));
            int index = hash1;
            while (hashTable2[index].state == 1) {
                if (strcmp(hashTable2[index].info->key,courseName) == 0) {
                    return index;
                }
                index = (index + hash2) % doubleCapacity;
            }
            return -1;

        }

    }
}
void deleteRecord(char courseName[]){

    int index = hashCode(generateKey(courseName));
    int i = index;
    while (hashTable1[i].state != 0) {
        if (hashTable1[i].state == 1 && strcmp(hashTable1[i].info->key,courseName) == 0) {
            hashTable1[i].state = -1;
            hashTable1[i].info = NULL;
            size1--;
           // printf("\n%d\n",hashTable1[i].state);
            return;
        }
        i = linearProbe(i);//linear probing
    }
        printf("record not found\n");
}
//lazy delete node with course name given by user
void deleteRecordDouble(char courseName[]){
    int hash1 = hashCode1(generateKey(courseName));
    int hash2 = hashCode2(generateKey(courseName));
    int i = hash1;
    while (hashTable2[i].state != 0) {
        if (hashTable2[i].state == 1 && strcmp(hashTable2[i].info->key,courseName) == 0) {
            hashTable2[i].state = -1;
            hashTable2[i].info = NULL;
            size2--;
            // printf("\n%d\n",hashTable1[i].state);
            return;
        }
        i = (i + hash2) % doubleCapacity;
    }
    printf("record not found\n");
}

float getLoadFactor(hashTableItem *hashTable){
    return ((float)size1/capacity);
}
float getLoadFactorDouble(hashTableItem *hashTable){
    return ((float)size2/doubleCapacity);
}

hashTableItem* reHash(hashTableItem* hashTable){
    hashTableItem *copy;
    copy = (hashTableItem*) malloc(capacity * sizeof(hashTableItem));
    for(int i = 0 ; i < capacity ; i++){
        copy[i] = hashTable[i];
        hashTable[i].state = 0;
        hashTable[i].info = NULL;
    }
    int cap = capacity;
    capacity = getPrime(capacity*2);
    hashTable  =(hashTableItem*) malloc(capacity * sizeof(hashTableItem));

    size1 = 0 ;
    numberOfCollisions = 0;
    for(int i = 0 ; i < capacity ;i++){
        hashTable[i].info =NULL;
        hashTable[i].state =0;

    }
    for(int i = 0 ; i < cap ; i++){
        if(copy[i].info != NULL){
            insert(hashTable,copy[i].info);
        }
    }
    return hashTable;
}

hashTableItem* reHashDouble(hashTableItem* hashTable){
    hashTableItem *copy;
    copy = (hashTableItem*) malloc(doubleCapacity * sizeof(hashTableItem));
    for(int i = 0 ; i < doubleCapacity ; i++){
        copy[i] = hashTable[i];
        hashTable[i].state = 0;
        hashTable[i].info = NULL;
    }
    int cap = doubleCapacity;
    doubleCapacity = getPrime(doubleCapacity*2);
    hashTable  =(hashTableItem*) malloc(doubleCapacity * sizeof(hashTableItem));
    //as we re-insert they will fill up again
    size2 = 0 ;
    numberOfCollisionsDouble = 0;

    for(int i = 0 ; i < doubleCapacity ;i++){
        hashTable[i].info =NULL;
        hashTable[i].state =0;

    }
    for(int i = 0 ; i < cap ; i++){
        if(copy[i].info != NULL){
            insertDouble(hashTable,copy[i].info);
        }
    }
    return hashTable;
}

int hashCode1(int key){
    return (key % doubleCapacity);
}

int hashCode2(int key){
    return (getPrimeLess(doubleCapacity-1) - (key % getPrimeLess(doubleCapacity-1)));
}
int getPrimeLess(int n){
    if (n % 2 == 0){
        n--;
    }
    for (; !ifPrime(n); n -= 2);
    return n;
}
void insertDouble(hashTableItem* hashTable, data* newData){
    int hash1 = hashCode1(generateKey(newData->key));
    int hash2 = hashCode2(generateKey(newData->key));
    int index = hash1;

    /* probing through other array elements */
    while (hashTable[index].state == 1) {
        if (strcmp(hashTable[index].info->key , newData->key)==0){
            hashTable[index].info = newData;
            return;
        }
        numberOfCollisionsDouble++;
        index = (index + hash2) % doubleCapacity;//the double probing function that i used is hashCode2
        if (index == hash1){
            printf("add failed!\n");
            return;
        }
    }
   //if(index != hash1)
      // numberOfCollisionsDouble++;
    hashTable[index].info = newData;
    hashTable[index].state = 1;
    size2++;

}
void printOnFile(){
    FILE* outFile = fopen("saved_courses.txt","w");
    for(int i = 0 ; i < doubleCapacity ; i++){
        if(hashTable2[i].state == 1 )
            fprintf(outFile,"%02d)\t%s %d %s %s\n",i,hashTable2[i].info->key,hashTable2[i].info->credit,hashTable2[i].info->courseCode,hashTable2[i].info->departmentName);
        else if (hashTable2[i].state == 0)
            fprintf(outFile,"%02d)\t-----\n",i);
        else if(hashTable2[i].state == -1)
            fprintf("%02d)\tDEL\n",i);
    }
    fprintf(outFile,"\nnumber of collisions: %d \n",numberOfCollisionsDouble);
    fclose(outFile);
}

