#include "TaskManager.h"



TaskManager::TaskManager()
{
	listofTasks = NULL;
	numTask = 3;

}


TaskManager::~TaskManager()
{
	delete[] listofTasks;
	listofTasks = NULL;
}

void TaskManager::setNumberTasks(int n)
{
	numTask = n;
}
int TaskManager::getNumberasks() const
{
	return numTask;
}
void TaskManager::init(int in)
{
	listofTasks = new WordTask[in];

	char *tasks[] = {"s_bary","b_w","n_ssan"};
	char *ans[] = {"subary", "bmw", "nissan"};
	char key[] = {'u', 'm', 'i'};

	for (int i = 0; i < in; ++i)
	{
		listofTasks[i].init(tasks[i], ans[i], key[i]);
	}
}

WordTask TaskManager::getTask(int count)
{
	return listofTasks[count];
}
WordTask TaskManager::getRandTask()
{
	int randC = random(0, numTask);
	return getTask(randC);
}