#include "SingleMemory.h"

// ��Ҫ��cpp�ļ��ж����е�static��������ʼ������Ȼ�ᱨ��
SingleMemory *SingleMemory::uniqueInstance = nullptr;
std::mutex SingleMemory::mMutex;

