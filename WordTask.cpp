#include "WordTask.h"

using namespace std;
using namespace cocos2d;

#define KEYCODE_OFFSET 27

WordTask::WordTask()
{
	k = ' ';
	ta = " ";
	answ = " ";


}
WordTask::~WordTask()
{

}

/*int WordTask::init(char* t, char* ans, char k)
{
	this->key = k;
	this->answer = ans;
	this->task = t;
	return 0;
}*/

int WordTask::init(std::string t, std::string ans, char key)
{
	this->k = key;
	this->answ = ans;
	this->ta = t;
	return 0;
}


bool WordTask::isKeyCorrect(EventKeyboard::KeyCode keyCode)
{
	if ((EventKeyboard::KeyCode)(this->k + KEYCODE_OFFSET) == keyCode)
		return true;
	return false;
}

/*int WordTask::setAnswer(char* ans)
{
	this->answer = ans;
	return 0;
}

int WordTask::setKey(char k)
{
	this->key = k;
	return 0;
}

int WordTask::setTask(char* t)
{
	this->task = t;
	return 0;
}*/

void WordTask::setKey(char key)
{
	this->k = key;
}
void WordTask::setAnswer(std::string a)
{
	this->answ = a;
}
void WordTask::setTask(std::string t)
{
	this->ta = t;
}

