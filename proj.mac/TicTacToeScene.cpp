//
//  TicTacToeScene.cpp
//  TicTacToeOSX
//
//  Created by Ricard Pérez del Campo on 24/05/13.
//  Copyright (c) 2013 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "TicTacToeScene.h"
#include "AppMacros.h"
#include "constants.h"
#include "EndGameScene.h"
USING_NS_CC;

TicTacToeScene::TicTacToeScene() : gameBoard(), gameAI(HARD, this->gameBoard, CROSS)
{
	this->gameBoard.registerAI(this->gameAI);
	this->humanPlayer = (this->gameAI.getPlayer() == CROSS ? NOUGHT : CROSS);
	this->aiTurn = false;
}

CCScene* TicTacToeScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TicTacToeScene *layer = TicTacToeScene::create();
	
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TicTacToeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	if (!this->isTouchEnabled())
	{
		this->setTouchEnabled(true);
	}
	
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	CCSprite *backgroundSprite = CCSprite::create("square.png");
	backgroundSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	backgroundSprite->setScaleX(visibleSize.width / backgroundSprite->getContentSize().width);
	backgroundSprite->setScaleY(visibleSize.height / backgroundSprite->getContentSize().height);
	this->addChild(backgroundSprite, 0);
	
	this->currPlayerLabel = CCLabelTTF::create("Your turn", "Arial", 20);
	this->currPlayerLabel->setColor(ccc3(0, 0, 0));
	this->currPlayerLabel->setPositionX((visibleSize.width - this->currPlayerLabel->getContentSize().width)*0.5f + origin.x);
	this->currPlayerLabel->setPositionY(visibleSize.height - this->currPlayerLabel->getContentSize().height + 8.0f + origin.y);
	this->addChild(this->currPlayerLabel, 100);
	
	
	float size = ((MIN(visibleSize.width, visibleSize.height)*0.8f)/BOARD_SIZE);
	float startX = ((visibleSize.width - size*BOARD_SIZE) * 0.5f) + origin.x;
	float startY = (visibleSize.height - ((visibleSize.height - size*BOARD_SIZE) * 0.5f)) + origin.y;
	
	float currY = (startY - size*0.5f);
	for (int i=0; i<BOARD_SIZE; i++)
	{
		float currX = (startX + size*0.5f);
		for (int j=0; j<BOARD_SIZE; j++)
		{
			CCSprite *sqSprite = CCSprite::create("square.png");
			CCSize spriteSize = sqSprite->getContentSize();
			float scaleX = (size / spriteSize.width);
			float scaleY = (size / spriteSize.height);
			sqSprite->setScaleX(scaleX);
			sqSprite->setScaleY(scaleY);
			
			sqSprite->setPosition(ccp(currX, currY));
			this->addChild(sqSprite, 10);
			
			this->positionSprites.addObject(sqSprite);
			
			currX += size;
		}
		
		currY -= size;
	}
    
    return true;
}

bool TicTacToeScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return (!this->aiTurn);
}

void TicTacToeScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint touchPos = pTouch->getLocationInView();
	touchPos.y = (CCDirector::sharedDirector()->getVisibleSize().height - touchPos.y);
	
	bool found = false;
	int i = 0;
	while (!found && i<this->positionSprites.count())
	{
		CCSprite *next = (CCSprite *)this->positionSprites.objectAtIndex(i);
		found = next->boundingBox().containsPoint(touchPos);
		if (!found)
		{
			++i;
		}
	}
	
	if (found)
	{
		int row = (i / BOARD_SIZE);
		int column = (i % BOARD_SIZE);
		CCLog("Touched (%i, %i)", row, column);
		
		Position pos = MakePosition(row, column);
		if (!this->gameBoard.positionIsOccupied(pos))
		{
			this->gameBoard.setPlayerAtPosition(this->humanPlayer, pos);
			this->occupyPositionByPlayer(pos, this->humanPlayer);
			
			std::cout << this->gameBoard << std::endl;
			
			if (!this->checkGameIsFinished())
			{
				this->aiTurn = true;
				this->currPlayerLabel->setString("Computer turn");
				CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TicTacToeScene::makeAITurn), this, 0.0f, 0, 1.5f, false);
			}
			
		}
		
		
	}
}

void TicTacToeScene::makeAITurn()
{
	Position position = this->gameAI.nextMovement();
	std::cout << this->gameBoard << std::endl;
	this->occupyPositionByPlayer(position, this->gameAI.getPlayer());
	if (!this->checkGameIsFinished())
	{
		this->aiTurn = false;
		this->currPlayerLabel->setString("Your turn");
	}
	
}

bool TicTacToeScene::checkGameIsFinished()
{
	Player winner;
	bool completed = this->gameBoard.isCompleted();
	bool finished = this->gameBoard.isFinished(winner);
	if (finished)
	{
		this->scheduleChangeSceneToEndGame((winner == this->humanPlayer) ? WIN : LOSE);
	} else if (completed)
	{
		this->scheduleChangeSceneToEndGame(DRAW);
	}
	
	return (completed || finished);
}

void TicTacToeScene::scheduleChangeSceneToEndGame(GameStatus status)
{
	switch (status)
	{
		case WIN:
			CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TicTacToeScene::endGameWin), this, 0.0f, 0, 1.5f, false);
			break;
		case LOSE:
			CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TicTacToeScene::endGameLose), this, 0.0f, 0, 1.5f, false);
			break;
		case DRAW:
			CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(TicTacToeScene::endGameDraw), this, 0.0f, 0, 1.5f, false);
			break;
	}
}

void TicTacToeScene::endGameWin()
{
	CCDirector::sharedDirector()->replaceScene(EndGameScene::scene(WIN));
}

void TicTacToeScene::endGameLose()
{
	CCDirector::sharedDirector()->replaceScene(EndGameScene::scene(LOSE));
}

void TicTacToeScene::endGameDraw()
{
	CCDirector::sharedDirector()->replaceScene(EndGameScene::scene(DRAW));
}



void TicTacToeScene::occupyPositionByPlayer(const Position &pos, const Player &player)
{
	int index = pos.row * BOARD_SIZE + pos.column;
	CCSprite *posSprite = (CCSprite *)this->positionSprites.objectAtIndex(index);
	CCSprite *sprite = CCSprite::create((player == CROSS ? "cross.png" : "nought.png"));
	sprite->setScaleX(posSprite->getScaleX());
	sprite->setScaleY(posSprite->getScaleY());
	sprite->setPosition(posSprite->getPosition());
	this->addChild(sprite, 20);
}

void TicTacToeScene::registerWithTouchDispatcher()
{
//    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}