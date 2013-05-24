//
//  EndGameScene.cpp
//  TicTacToeOSX
//
//  Created by Ricard Pérez del Campo on 25/05/13.
//  Copyright (c) 2013 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EndGameScene.h"
#include "AppMacros.h"
#include "TicTacToeScene.h"
USING_NS_CC;

CCScene* EndGameScene::scene(GameStatus status)
{
    CCScene *scene = CCScene::create();
    
    EndGameScene *layer = EndGameScene::create();
	layer->gameStatus = status;
    scene->addChild(layer);
	
    return scene;
}

// on "init" you need to initialize your instance
bool EndGameScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	
	
	CCLabelTTF *pCloseItemLabel = CCLabelTTF::create("Exit", "Arial", 20);
	CCMenuItemLabel *pCloseItem = CCMenuItemLabel::create(pCloseItemLabel, this, menu_selector(EndGameScene::menuCloseCallback));
	CCLabelTTF *pPlayAgainItemLabel = CCLabelTTF::create("Play again", "Arial", 20);
	CCMenuItemLabel *pPlayAgainItem = CCMenuItemLabel::create(pPlayAgainItemLabel, this, menu_selector(EndGameScene::menuPlayAgainCallback));
    
    CCMenu* pMenu = CCMenu::create(pPlayAgainItem, pCloseItem, NULL);
	pMenu->alignItemsVertically();
    this->addChild(pMenu, 1);
    
    this->statusLabel = CCLabelTTF::create("You won", "Arial", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    this->statusLabel->setPosition(ccp(origin.x + visibleSize.width/2,
									   origin.y + visibleSize.height - this->statusLabel->getContentSize().height));
	
    // add the label as a child to this layer
    this->addChild(this->statusLabel, 1);
    
    return true;
}

void EndGameScene::onEnter()
{
	CCLayer::onEnter();
	this->setGameStatus(this->gameStatus);
}

void EndGameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    exit(0);
}


void EndGameScene::menuPlayAgainCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(TicTacToeScene::scene());
}


void EndGameScene::setGameStatus(GameStatus status)
{
	if (this->statusLabel != 0)
	{
		switch (status)
		{
			case WIN:
				this->statusLabel->setString("You won!");
				break;
			case LOSE:
				this->statusLabel->setString("You lost");
				break;
			case DRAW:
				this->statusLabel->setString("Draw");
				break;
		}
	}
	this->gameStatus = status;
}