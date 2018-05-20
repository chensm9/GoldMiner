#include "GameScene.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	return GameSence::create();
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bg = Sprite::create("level-background-0.jpg");
    bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(bg, 0);

    stone = Sprite::create("stone.png");
    stone->setPosition(Vec2(visibleSize.width / 2 + origin.x + 80, visibleSize.height + origin.y - 160));
    this->addChild(stone, 1);

    mouse = Sprite::createWithSpriteFrameName("pulled-gem-mouse-0.png");
    Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
    mouse->runAction(RepeatForever::create(mouseAnimate));
    mouse->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 20));
    this->addChild(mouse, 2);

    auto shoot = MenuItemLabel::create(
            Label::createWithTTF("Shoot", "fonts/arial.ttf", 60),
            CC_CALLBACK_1(GameSence::Shoot, this));
    shoot->setPosition(Vec2(visibleSize.width / 2 + origin.x + 300, visibleSize.height + origin.y - 160));
    auto menu = Menu::create(shoot, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);
	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {
	auto location = touch->getLocation();

    auto cheese = Sprite::create("cheese.png");
    cheese->setPosition(location);
    this->addChild(cheese, 1);
    auto fadeOut = FadeOut::create(5.0f);
    auto clearUp = CallFunc::create([cheese, this]() {
        this->removeChild(cheese);
    });
    cheese->runAction(Sequence::create(fadeOut, clearUp, nullptr));

    auto moveTime = location.getDistance(mouse->getPosition()) / 100 * 0.5;
    auto mouseMoveTo = MoveTo::create(moveTime, location);
    mouse->runAction(mouseMoveTo);
	return true;
}

void GameSence::Shoot(Ref* pSender) {
    auto diamond = Sprite::createWithSpriteFrameName("pulled-diamond-0.png");
    Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAnimation"));
    diamond->runAction(RepeatForever::create(diamondAnimate));
    diamond->setPosition(mouse->getPosition());
    this->addChild(diamond, 2);

    auto stone2 = Sprite::create("stone.png");
    stone2->setPosition(stone->getPosition());
    this->addChild(stone2, 1);
    auto moveTo = MoveTo::create(1, mouse->getPosition());
    auto fadeOut = FadeOut::create(0.5f);
    auto clearUp = CallFunc::create([stone2, this]() {
        this->removeChild(stone2);
    });
    auto seq = Sequence::create(moveTo, fadeOut, clearUp, nullptr);
    stone2->runAction(seq);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    MoveTo* moveTo2 = MoveTo::create(1, Vec2(origin.x + random() % (int(visibleSize.width)),
                                         origin.y + random() % (int(visibleSize.height))));
    mouse->runAction(moveTo2);
}
