//
//  TicTacToeScene.h
//  TicTacToeOSX
//
//  Created by Ricard Pérez del Campo on 24/05/13.
//  Copyright (c) 2013 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __TicTacToeOSX__TicTacToeScene__
#define __TicTacToeOSX__TicTacToeScene__

#include "cocos2d.h"
#include "ai.h"
#include "board.h"

class TicTacToeScene : public cocos2d::CCLayer
{
private:
	Player humanPlayer;
	Board gameBoard;
	AI gameAI;
	bool aiTurn;
	
	cocos2d::CCArray positionSprites;
	cocos2d::CCLabelTTF *currPlayerLabel;
	
	void makeAITurn();
	void occupyPositionByPlayer(const Position &pos, const Player &player);
	bool checkGameIsFinished();
	
	void scheduleChangeSceneToEndGame(GameStatus status);
	void endGameWin();
	void endGameLose();
	void endGameDraw();
	
public:
	TicTacToeScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(TicTacToeScene);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void registerWithTouchDispatcher();

};

#endif /* defined(__TicTacToeOSX__TicTacToeScene__) */
