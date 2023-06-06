#include <cstring>
#include "sortFile.h"

fileListStat_t gFileListStat = fileListStat_idle;
sortList_t _fileList[MAX_LIST_SIZE];
uint16_t fileListInd = 0;
sortMethod_t gMethod = sortDownCount;

const char fileNameBase[] = FILE_NAME_BASE;
const char fileNameExt[]  = FILE_NAME_EXT;

// privet functions
listPriority_t fileList_compair(uint16_t first, uint16_t secound);
uint16_t makeNumFromStrUntillDot(char* Num);
bool fileList_decreaseCount(void);
uint8_t charToInt(char chr);


uint16_t fileList_getMaxInd(void) {
  uint16_t maxNum = 0;

  while(gFileListStat != fileListStat_idle);
  gFileListStat = fileListStat_processing;  
  for(int i=0; i<fileList_getCount(); i++){
      int curInd = _fileList[i].index;
      if(!maxNum)maxNum = curInd;
      // if(!minNum)minNum = curInd;
      maxNum = std::max((int)maxNum, curInd);
      // minNum = std::min((int)minNum, curInd);
  }
  gFileListStat = fileListStat_idle;
  return maxNum;
}
uint16_t fileList_getMinInd(void) {
  uint16_t minNum = 0;

  while(gFileListStat != fileListStat_idle);
  gFileListStat = fileListStat_processing;  
  for(int i=0; i<fileList_getCount(); i++){
      int curInd = _fileList[i].index;
      // if(!maxNum)maxNum = curInd;
      if(!minNum)minNum = curInd;
      // maxNum = std::max((int)maxNum, curInd);
      minNum = std::min((int)minNum, curInd);
  }
  gFileListStat = fileListStat_idle;
  return minNum;
}
removeStat_t fileList_removeByInd(uint16_t index) {
  removeStat_t retVal = removeFailed;
  if(index < fileList_getCount()){
    for(int i = index; i < fileList_getCount(); i++){
      _fileList[i] = _fileList[i+1];
    }
    fileList_decreaseCount();
    fileList_sort(gMethod);
    retVal = removeSuccessful;
  }
  return retVal;
}

void fileList_sort(sortMethod_t method) {

	size_t  i, j;
	sortList_t temp;
	listPriority_t priorityDependOnSortMethod;

  gMethod = method;

	switch (method)
	{
	case sortDownCount:
		priorityDependOnSortMethod = secoundHasPriority;
		break;
	/*case sortUpCount:
		priorityDependOnSortMethod = firstHasPriority;
		break;*/
	default:
		priorityDependOnSortMethod = firstHasPriority;
		break;
	}

  if(fileListInd){
    for (i = 1; i <= fileListInd - 1; i++) {
      for (j = 0; j < fileListInd - i; j++) {
        if (fileList_compair(_fileList[j].index, _fileList[j + 1].index) == priorityDependOnSortMethod) {
          temp = _fileList[j];
          _fileList[j] = _fileList[j + 1];
          _fileList[j + 1] = temp;
        }
      }
    }
  }
}

sortList_t fileList_getByInd(uint16_t index) {
	return _fileList[index];
}
char* fileList_getNameByInd(uint16_t index) {
	return _fileList[index].name;
}
size_t fileList_getSizeByInd(uint16_t index) {
	return _fileList[index].size;
}

uint16_t fileList_getCount(void) {
	return fileListInd;
}
bool fileList_decreaseCount(void) {
  bool retVal = false;
  if(fileListInd){
    fileListInd--;
    retVal = true;
  }
	return retVal;
}

listPriority_t fileList_compair(uint16_t first, uint16_t secound) {
	listPriority_t retVal = hasNotPriority;
	if (first > secound) {
		retVal = firstHasPriority;
	}
	else {
		retVal = secoundHasPriority;
	}
	return retVal;
}

addStat_t fileList_add(char* fileName, size_t fileSize) {
	addStat_t retVal = addFailed_listFilled;
  size_t fileNameBase_len = strlen(fileNameBase);
  if(std::memcmp(fileName, fileNameBase, fileNameBase_len) == 0){
    
    if (fileListInd < MAX_LIST_SIZE) {
      size_t size = strlen(fileName) + 1;
      _fileList[fileListInd].name = (char*)malloc(size);
      memcpy(_fileList[fileListInd].name, fileName, size);
      _fileList[fileListInd].size = fileSize;
      _fileList[fileListInd].index = makeNumFromStrUntillDot(&fileName[fileNameBase_len]);

      fileListInd++;
      retVal = addSuccessful;
    }
  }
  else{
    return addFailed_basicFileName;
  }
    
	return retVal;
}
void fileList_freeMemory(void) {
	uint16_t i = 0;
	for (i; i < fileListInd; i++) {
		free(_fileList[i].name);
		memset(&_fileList[i], 0, sizeof(sortList_t));
	}
	fileListInd = 0;
}

uint8_t charToInt(char chr) {
	uint8_t retValue = 0;
	if ((chr >= 48 /* '0' */) && (chr <= 57 /* '9' */))
	{
		retValue = (chr - 48);
	}
	return retValue;
}
uint16_t makeNumFromStrUntillDot(char* Num) {
	uint16_t retValue = 0;
	uint8_t i = 0;
	char curChar;
	while (true)
	{
		curChar = Num[i++];
		if (curChar != '.') {
			retValue = (charToInt(curChar) + (retValue * 10));
		}
		else {
			break;
		}
	}
	return retValue;
}
