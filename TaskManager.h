


#pragma once

#include "WordTask.h"






class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	void setNumberTasks(int);
	int getNumberasks() const;
	void init(int = 3);

	WordTask getTask(int);
	WordTask getRandTask();

private:
	WordTask * listofTasks;
	int numTask;
};


