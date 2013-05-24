//
//  EndGameScene.h
//  TicTacToeOSX
//
//  Created by Ricard Pérez del Campo on 25/05/13.
//  Copyright (c) 2013 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __TicTacToeOSX__EndGameScene__
#define __TicTacToeOSX__EndGameScene__

#include "cocos2d.h"
#include "constants.h"
class EndGameScene : public cocos2d::CCLayer
{
private:
	cocos2d::CCLabelTTF *statusLabel;
	GameStatus gameStatus;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(GameStatus status);
    
    // implement the "static node()" method manually
    CREATE_FUNC(EndGameScene);
	
	void menuCloseCallback(CCObject* pSender);
	void menuPlayAgainCallback(CCObject* pSender);
	
	
	void setGameStatus(GameStatus status);
	
	virtual void onEnter();
};

#endif /* defined(__TicTacToeOSX__EndGameScene__) */
