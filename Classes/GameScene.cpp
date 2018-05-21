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

    stoneLayer = Layer::create();
    stoneLayer->setPosition(Vec2(0, 0));
    stoneLayer->setAnchorPoint(Point(0, 0));
    this->addChild(stoneLayer, 2);

    mouseLayer = Layer::create();
    mouseLayer->setPosition(Vec2(0, visibleSize.height / 2 + origin.y));
    mouseLayer->setAnchorPoint(Point(0, 0));
    this->addChild(mouseLayer, 2);

    stone = Sprite::create("stone.png");
    stone->setPosition(Vec2(560, 480));
    stoneLayer->addChild(stone, 1);

    mouse = Sprite::createWithSpriteFrameName("pulled-gem-mouse-0.png");
    Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
    mouse->runAction(RepeatForever::create(mouseAnimate));
    mouse->setPosition(Vec2(visibleSize.width / 2 + origin.x, 0));
    mouseLayer->addChild(mouse, 1);

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
    mouse->stopAction(mouseMove);

    auto cheese = Sprite::create("cheese.png");
    cheese->setPosition(location);
    this->addChild(cheese, 1);
    auto fadeOut = FadeOut::create(5.0f);
    auto clearUp = CallFunc::create([cheese, this]() {
        this->removeChild(cheese);
    });
    cheese->runAction(Sequence::create(fadeOut, clearUp, nullptr));

    auto moveTime = location.getDistance(mouseLayer->convertToWorldSpace(mouse->getPosition())) / 100 * 0.5;
    mouseMove = MoveTo::create(moveTime, mouseLayer->convertToNodeSpace(location));
    mouse->runAction(mouseMove);
    return true;
}

void GameSence::Shoot(Ref* pSender) {
    /*auto diamond = Sprite::createWithSpriteFrameName("pulled-diamond-0.png");
    Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAnimation"));
    diamond->runAction(RepeatForever::create(diamondAnimate));*/
    auto diamond = Sprite::create("diamond.png");
    diamond->setPosition(mouseLayer->convertToWorldSpace(mouse->getPosition()));
    this->addChild(diamond, 2);

    auto stone2 = Sprite::create("stone.png");
    stone2->setPosition(stone->getPosition());
    stoneLayer->addChild(stone2, 1);
    auto moveTo = MoveTo::create(1, mouseLayer->convertToWorldSpace(mouse->getPosition()));
    auto fadeOut = FadeOut::create(0.5f);
    auto clearUp = CallFunc::create([stone2, this]() {
        this->stoneLayer->removeChild(stone2);
    });
    auto seq = Sequence::create(moveTo, fadeOut, clearUp, nullptr);
    stone2->runAction(seq);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto randomPosition = mouseLayer->convertToNodeSpace(
        Vec2(origin.x + random() % (int(visibleSize.width)),
             origin.y + random() % (int(visibleSize.height))));
    mouseMove = MoveTo::create(1, randomPosition);
    mouse->runAction(mouseMove);
}
