#include "SingleMemory.h"

// 需要在cpp文件中对类中的static变量做初始化，不然会报错
SingleMemory *SingleMemory::uniqueInstance = nullptr;
std::mutex SingleMemory::mMutex;

