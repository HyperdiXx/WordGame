#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


#define LABEL_FONTNAME "fonts/Marker Felt.ttf"
#define LABEL_FONTSIZE 24
#define LABEL_FONTSIZESCOREHP 36
#define SCORE_CHANGE 10
#define WORD_COLLISION_BITMASK 0x000001
#define OBSTACKLE_COLLISION_BITMASK 0x000002
#define LEN 15



Scene* WordGame::createScene()
{
    return WordGame::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}



bool WordGame::init()
{

    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(WordGame::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	wordLabel = CreateTextLabel("HII", LABEL_FONTNAME, LABEL_FONTSIZE, Vec2(0,0));
	
	Vec2 hp_pos = Vec2(origin.x + visibleSize.width - 90, origin.y + visibleSize.height - 40);

	Vec2 score_pos = Vec2(origin.x + visibleSize.width - 390, origin.y + visibleSize.height - 80);
	auto scoreLabel = CreateScoreLabel("Best score", LABEL_FONTNAME, LABEL_FONTSIZESCOREHP, score_pos);

	scoreI = ui::LoadingBar::create("bp.png");
	scoreI->setPosition(Vec2(origin.x + visibleSize.width - 350, origin.y + visibleSize.height - 40));
	scoreI->setScale(0.25,0.2);
	scoreI->setPercent(0);
	this->addChild(scoreI,3);

	auto spriteS = Sprite::create("bp_pure.png");
	spriteS->setPosition(Vec2(origin.x + visibleSize.width - 350, origin.y + visibleSize.height - 40));
	spriteS->setScale(0.25, 0.2);
	this->addChild(spriteS, 3);

	lBar = ui::LoadingBar::create("hp.png");
	lBar->setScale(0.5, 0.5);
	lBar->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 40));
	lBar->setPercent(100);
	this->addChild(lBar, 3);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(WordGame::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	tm.init(LEN);
	updateTask(LEN);

	wordLabel->setString(wordTask.getTask());


	Vec2 score_posPoints = Vec2(origin.x + 190, origin.y + visibleSize.height - 80);
	scoreLabelPoint = CreateTextLabel("100", LABEL_FONTNAME, 2 * LABEL_FONTSIZE, score_posPoints);


	auto bgI = Sprite::create("back.png");
	bgI->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bgI);

	collisionDetect = Sprite::create("bp.png");
	collisionDetect->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 325));
	collisionDetect->setScale(0.01, 0.01);
	collisionDetect->setVisible(false);
	this->addChild(collisionDetect);
	
	auto collision = EventListenerPhysicsContact::create();
	collision->onContactBegin = CC_CALLBACK_1(WordGame::onCollisionEnter, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(collision, this);

	//score init
	readBestScore();

	//update func
	scheduleUpdate();

	//background music
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("back.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("back.mp3", true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2f);

    return true;
}


//update func called every frame
void WordGame::update(float dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Rect rect1 = wordLabel->getBoundingBox();
	Rect rect2 = collisionDetect->getBoundingBox();
	Vec2 point = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 325);
	Vec2 point1 = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 325);
	float pointtest = wordLabel->getPositionY();
	if (rect1.intersectsRect(rect2))
	{
		minusHp(); 
		scheduleOnce(schedule_selector(WordGame::updateTask), 1);
		
		//showAnswer();
	}
	if (rect1.intersectsRect(rect2) && lBar->getPercent() < 1)
	{
		scheduleOnce(schedule_selector(WordGame::showEnd), 1);
		
		writeBestScore();
	}


}

//label creation text
Label *WordGame::CreateTextLabel(char *text, char *ff, int fsize, Vec2 pos)
{
	Label *label = Label::createWithTTF(text, ff, fsize);
	label->setPosition(pos);
	this->addChild(label, 1, "text_label");
	return label;
}

//score label creation
Label *WordGame::CreateScoreLabel(char *text, char *ff, int fsize, Vec2 pos)
{
	Label *labelScore = Label::createWithTTF(text, ff, fsize);
	labelScore->setPosition(pos);
	this->addChild(labelScore,2, "score_label");
	return labelScore;
}
//hp label creation
Label *WordGame::CreateHPLabel(char *text, char *ff, int fsize, Vec2 pos)
{
	Label *labelHP = Label::createWithTTF(text, ff, fsize);
	labelHP->setPosition(pos);
	this->addChild(labelHP, 2, "hp_label");
	return labelHP;
}

//moving word
void WordGame::MoveObject(Sprite *obj, Vec2 moveto, Vec2 scaleby)
{
	auto acMoveTo = MoveTo::create(4, moveto);
	auto acScaleBy = ScaleBy::create(2, scaleby.x, scaleby.y);
	obj->runAction(acMoveTo);
	obj->runAction(acScaleBy);
	return;
}

/*void WordGame::detectHp()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (wordLabel->getPosition() == origin + Vec2(visibleSize.width / 2, -visibleSize.height / 4))
	{
		wordLabel->setScale(3, 3);
	}
}*/


void WordGame::SetReturnPosLabel(Sprite *obj, Vec2 scaleby)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 task_pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 200);
	Vec2 moveTo = origin + Vec2(visibleSize.width / 2, -visibleSize.height / 4);
	auto mb = MoveTo::create(0.1, task_pos);
	auto scale = ScaleBy::create(0.1, scaleby.x, scaleby.y);
	obj->runAction(mb);
	MoveObject((Sprite*)wordLabel, moveTo, Vec2(1, 1));
	return;
}

//showing correct answer
void WordGame::ShowAnswer()
{
	wordLabel->setString(wordTask.getAnswer());
	wordLabel->stopAllActions();
}

//new word 
void WordGame::updateTask(float dt)
{
	wordTask = tm.getRandTask();
	wordLabel->setString(wordTask.getTask());
	initPosMove();
}

void WordGame::initPosMove()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 task_pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 20);

	wordLabel->setPosition(task_pos);
	wordLabel->setScale(1);

	Vec2 taskToMove = origin + Vec2(visibleSize.width / 2, -visibleSize.height / 4 );

	MoveObject((Sprite*)wordLabel, taskToMove, Vec2(3,3));
}

void WordGame::addScorePer()
{
	scoreI->setPercent(scoreI->getPercent() + 10);
}

int WordGame::getScorePer() const
{
	int res = scoreI->getPercent();
	return res;
}

void WordGame::minusHp()
{
	lBar->setPercent(lBar->getPercent() - 1.51);
}
void WordGame::eRror()
{
	lBar->setPercent(lBar->getPercent() - 34);
}

void WordGame::setBS(std::string n)
{
	scoreLabelPoint->setString(n);
}
int WordGame::getBS() const
{
	int value = atoi(scoreLabelPoint->getString().c_str());
	return value;
}

void WordGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
	if (wordTask.isKeyCorrect(keyCode))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music1.wav");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music1.wav");
		if (scoreI->getPercent() == 100)
		{
			ShowAnswer();
			scheduleOnce(schedule_selector(WordGame::showWin), 1);
			CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("winmusic.wav");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("winmusic.wav");
			writeBestScore();
		}
		else
		{
			ShowAnswer();
			addScorePer();
			scheduleOnce(schedule_selector(WordGame::updateTask), 1);
			
		}
	}

	else if (EventKeyboard::KeyCode::KEY_A == keyCode)
	{
		SetReturnPosLabel((Sprite*)wordLabel, Vec2(3, 3));
		
	}

	else if (EventKeyboard::KeyCode::KEY_B == keyCode)
	{
		
	}
	else if (EventKeyboard::KeyCode::KEY_ESCAPE == keyCode)
	{
		exit(0);
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("losemusic.wav");
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("losemusic.wav");
		if (lBar->getPercent() <= 35)
		{
			ShowAnswer();
			scheduleOnce(schedule_selector(WordGame::showEnd), 1);
			writeBestScore();
		}
		else
		{
			eRror();
			ShowAnswer();
			scheduleOnce(schedule_selector(WordGame::updateTask), 1);
		}
	}

}

Vec2 setPos(Vec2 p)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 moveTo = origin + Vec2(visibleSize.width / 2, -visibleSize.height / 4);

	return p;
}

void WordGame::addScore(int s)
{
	setScore(score + s);
}

void WordGame::setScore(int s)
{
	score = s;
	scoreI->setPercent(0);
}

int WordGame::getScore() const
{
	return score;
}

void WordGame::createEndScene()
{
	endScene = Scene::create();

	Label *endFont = Label::createWithTTF("GameOver!!!", LABEL_FONTNAME, 3 * LABEL_FONTSIZE);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 60);

	endFont->setPosition(pos);
	endScene->addChild(endFont, 1);

	auto replayBut = MenuItemImage::create("button_replay_new.png", "button_replay_new.png", 
		CC_CALLBACK_1(WordGame::menuReplayBack, this));

	replayBut->setPosition(Vec2(pos.x, pos.y - endFont->getContentSize().height - 200));
	replayBut->setScale(0.5, 0.5);

	auto menu = Menu::create(replayBut, NULL);
	menu->setPosition(Vec2::ZERO);
	endScene->addChild(menu, 1);
	auto bgI = Sprite::create("back.png");
	bgI->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	endScene->addChild(bgI);
	readBestScore();
	lBar->setPercent(100);

}

void WordGame::createWinScene()
{
	winScene = Scene::create();

	Label *winner = Label::createWithTTF("WINNER!!!", LABEL_FONTNAME, 3 * LABEL_FONTSIZE);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 60);

	winner->setPosition(pos);
	winScene->addChild(winner, 1);

	auto startBut = MenuItemImage::create("start.png", "start.png",
		CC_CALLBACK_1(WordGame::menuReplayBack, this));

	startBut->setPosition(Vec2(pos.x, pos.y - winner->getContentSize().height - 200));

	auto start = Menu::create(startBut, NULL);
	start->setPosition(Vec2::ZERO);
	winScene->addChild(start, 1);
	auto bgI = Sprite::create("back.png");
	bgI->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	winScene->addChild(bgI);
	readBestScore();
	lBar->setPercent(100);
}

void WordGame::createStartScene()
{
	startScene = Scene::create();

	Label *start = Label::createWithTTF("WordGame", LABEL_FONTNAME, 3 * LABEL_FONTSIZE);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 60);

	start->setPosition(pos);
	winScene->addChild(start, 1);

	auto startBut = MenuItemImage::create("start.png", "start.png",
		CC_CALLBACK_1(WordGame::menuReplayBack, this));

	startBut->setPosition(Vec2(pos.x, pos.y - start->getContentSize().height - 200));

	auto startGame = Menu::create(startBut, NULL);
	start->setPosition(Vec2::ZERO);
	startScene->addChild(start, 1);
	auto bgI = Sprite::create("back.png");
	bgI->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	winScene->addChild(bgI);
}

void WordGame::showEnd(float dt)
{
	createEndScene();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("gameover.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gameover.wav");
	Director::getInstance()->pushScene(endScene);
}

void WordGame::showWin(float dt)
{
	createWinScene();
	Director::getInstance()->pushScene(winScene);
}

void WordGame::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void WordGame::menuReplayBack(Ref* pSender)
{
	Director::getInstance()->popScene();
	setScore(0);
	updateTask(0);
}


bool WordGame::onCollisionEnter(PhysicsContact &contact)
{
	PhysicsBody *bodyA = contact.getShapeA()->getBody();
	PhysicsBody *bodyB = contact.getShapeB()->getBody();

	
	Rect rect1 = wordLabel->getBoundingBox();
	Rect rect2 = collisionDetect->getBoundingBox();
	if (rect1.intersectsRect(rect2))
	{
		minusHp();
	}
	return true;
}

void WordGame::readBestScore()
{
	std::string scoreB;
	std::ifstream inFile("bestscore.txt");

	if (inFile.fail())
	{
		exit(1);
	}

	inFile >> scoreB;
	setBS(scoreB);
	inFile.close();
}

void WordGame::writeBestScore()
{
	std::string scoreB;
	std::ifstream inFile("bestscore.txt");
	std::ofstream outFile("bestscore.txt");
	std::string smth = "Done";
	
	if (inFile.fail())
	{
		exit(1);
	}

	inFile >> scoreB;
	inFile.clear();
	inFile.seekg(0);
	int a = atoi(scoreB.c_str());
	if (getScorePer() > a)
	{
		outFile << getScorePer();
	}
	else 
	{
		return;
	}
	inFile.close();
	outFile.close();
}