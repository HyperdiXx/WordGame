#ifndef _WORDTASK_
#define _WORDTASK_

#include "cocos2d.h"
#include <fstream>
#include <vector>

#pragma once

using namespace cocos2d;

class WordTask
{
public:
	WordTask();
	~WordTask();

	//int init(char*, char*, char);
	int init(std::string, std::string, char);
	bool isKeyCorrect(EventKeyboard::KeyCode keyCode);

	//char getKey()  { return key; };
	//char *getAnswer()  { return answer; };
	//char *getTask()  { return task; };


	char getKey() const { return k; };
	std::string getAnswer() const { return answ; };
	std::string getTask() const { return ta; };

	//int setKey(char);
	//int setAnswer(char*);
	//int setTask(char*);

	void setKey(char);
	void setAnswer(std::string);
	void setTask(std::string);

private:
	/*char* task;
	char* answer;
	char key;*/

	std::string ta;
	std::string answ;
	char k;

};
#endif // !_WORDTASK_