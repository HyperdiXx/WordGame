#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "WordTask.h"
#include "TaskManager.h"
#include "FileReader.h"
#include "ui/CocosGUI.h"
#include <fstream>

using namespace cocos2d;

class WordGame : public cocos2d::Scene
{

public:
	
    static cocos2d::Scene* createScene();

    virtual bool init();

	virtual void update(float dt);
    
	Label *CreateTextLabel(char *text, char *ff, int fsize, Vec2 pos);
	Label *CreateScoreLabel(char *text, char *ff, int fsize, Vec2 pos);
	Label *CreateHPLabel(char *text, char *ff, int fsize, Vec2 pos);

	void MoveObject(Sprite *obj, Vec2 moveto, Vec2 scaleby);
	void SetReturnPosLabel(Sprite *obj, Vec2 scaleby);
	void ShowAnswer();
	void addScore(int s);
	void setScore(int s);
	int getScore() const;

	void createEndScene();
	void createWinScene();
	void createStartScene();
	void showEnd(float dt);
	void showWin(float dt);
	void detectHp();
	void addScorePer();
	int getScorePer() const;
	void minusHp();
	void eRror();
	void setBS(std::string);
	int getBS() const;

    void menuCloseCallback(Ref* pSender);
	void menuReplayBack(Ref* pSender);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
    CREATE_FUNC(WordGame);

private:
	Label *wordLabel;
	Label *scoreLabelPoint;
	Scene *endScene;
	Scene *winScene;
	Scene *startScene;
	WordTask wordTask;
	Sprite *collisionDetect;
	TaskManager tm;
	ui::LoadingBar *scoreI;
	ui::LoadingBar *lBar;
	int score = 0;
	int count = 0;
private:
	Vec2 setPos();
	void updateTask(float);
	void initPosMove();
	bool onCollisionEnter(PhysicsContact &contact);
	void readBestScore();
	void writeBestScore();
};

#endif // __HELLOWORLD_SCENE_H__