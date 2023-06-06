#ifndef _SORT_FILE_H
#define _SORT_FILE_H

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIST_SIZE 1666 //12*1666~20kB

#define FILE_NAME_BASE "record"
#define FILE_NAME_EXT  ".wav"

typedef enum {
	fileListStat_scaning = 0,
  fileListStat_processing,
  fileListStat_displaying,
  fileListStat_idle,
}fileListStat_t;

typedef enum {
	hasNotPriority = 0,
	firstHasPriority,
	secoundHasPriority,
}listPriority_t;
typedef enum {
	addFailed_listFilled = 0,
  addFailed_basicFileName,
	addSuccessful,
}addStat_t;
typedef enum {
	sortDownCount = 0,
	sortUpCount,
}sortMethod_t;
typedef enum {
	removeFailed = 0,
	removeSuccessful,
}removeStat_t;

typedef struct {
  char*    name;
	size_t   size;
	uint16_t index;
}sortList_t;
// sizeof(sortList_t) = 12

addStat_t    fileList_add(char* fileName, size_t fileSize);
uint16_t     fileList_getCount(void);
sortList_t   fileList_getByInd(uint16_t index);
char*        fileList_getNameByInd(uint16_t index);
size_t       fileList_getSizeByInd(uint16_t index);
void         fileList_sort(sortMethod_t method);
void         fileList_freeMemory(void);
uint16_t     fileList_getMaxInd(void);
uint16_t     fileList_getMinInd(void);
removeStat_t fileList_removeByInd(uint16_t index);

extern fileListStat_t gFileListStat;

#endif //_SORT_FILE_H
