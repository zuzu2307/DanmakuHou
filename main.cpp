#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
#include<string>
#include<time.h> 
#include<algorithm>
#include"Player.h"
#include"Platform.h"
#include"enemy.h"
#include"Animation.h"


using namespace sf;
static const float VIEW_HEIGHT = 1080.0f;
static const float VIEW_WIDTH = 1920.0f;
static const float maxhp = 2.0f, maxbosshp = 5000.0f,Jirenmaxhp = 10000;
float playerdamage = 1, playerboomdamage = 5, playerlife = 3, playerboom = 5;
float enemyshp[8] = { maxhp ,maxhp ,maxhp ,maxhp, maxhp ,maxhp ,maxhp ,maxhp };
float enemyrhp[8] = { maxhp ,maxhp ,maxhp ,maxhp, maxhp ,maxhp ,maxhp ,maxhp };
float enemys2hp[8] = { maxhp ,maxhp ,maxhp ,maxhp, maxhp ,maxhp ,maxhp ,maxhp };
float enemyr2hp[8] = { maxhp ,maxhp ,maxhp ,maxhp, maxhp ,maxhp ,maxhp ,maxhp };
float bosshp = maxbosshp;
float Jirenhp = Jirenmaxhp;
float deltaTime = 0.0f;
bool faceRight = true, visible = false, canBoom = true, canBoomDamage, iFrame, iFramehit = false, canshoot = true,
CutScene = false, Marisa = false, Cirno = false, Aya = false, Jiren = false, Over9000 = false, End = false, lastfight = false;
unsigned int row;
float bulletspeed;
int Score,Scorecount, mode, showLife, showBoom, framecount,modecheck,modecheck2,turncheck, turncheck2,turncheck3, turncheck4;
Vector2f BulletVelocity, ItemVelocity;
int timecount, timecounts,CUTSECENE, CUTSECENESTATE,GO = 0;


void ReSizeView(const RenderWindow& window, View& view)
{
	float aspectRatio = float(window.getSize().x) / float(window.getSize().y);

	view.setSize(VIEW_WIDTH*aspectRatio, VIEW_HEIGHT);
}

void HighScore(RenderWindow &window, Font font, Text hs, Text a, Text b)
{
	std::vector<std::pair<int, std::string>> scoreboard;
	std::ofstream myFile;
	std::ifstream loadFile;
	loadFile.open("HighScore.txt");
	hs.setCharacterSize(50);
	hs.setFont(font);
	hs.setPosition(VIEW_WIDTH / 2 -600, 100);
	hs.setFillColor(Color::White);
	scoreboard.clear();
	while (!loadFile.eof())
	{
		std::string tempName;
		int tempScore;
		loadFile >> tempScore >> tempName;
		scoreboard.push_back({ tempScore,tempName });
	}
	loadFile.close();
	std::sort(scoreboard.begin(), scoreboard.end());
	for (int i = 5; i > 0; i--)
	{
		a.setString(std::to_string(scoreboard[i].first));
		a.setCharacterSize(50);
		a.setFont(font);
		a.setPosition(VIEW_WIDTH / 2 , 570 - (80 * i));
		window.draw(a);
		b.setString(scoreboard[i].second);
		b.setCharacterSize(50);
		b.setFont(font);
		b.setPosition(VIEW_WIDTH / 2-600, 570 - (80 * i));
		window.draw(b);
	}
	window.draw(hs);
}

int SetHighScore(std::string name, int score)
{
	std::vector<std::pair<int, std::string>> scoreboard;
	std::ofstream myFile;
	std::ifstream loadFile;
	myFile.open("HighScore.txt", std::ios::out | std::ios::app);
	myFile << "\n" << score << " " << name;
	myFile.close();
	loadFile.open("HighScore.txt", std::ios::out);
	while (!loadFile.eof())
	{
		std::string tempName;
		int tempScore;
		loadFile >> tempScore >> tempName;
		scoreboard.push_back({ tempScore,tempName });
	}
	std::sort(scoreboard.begin(), scoreboard.end());
	loadFile.close();
	myFile.open("HighScore.txt", std::ios::out);
	int count = 5;
	for (int i = scoreboard.size() - 1; count > 0; i--)
	{
		myFile << scoreboard[i].first << " " << scoreboard[i].second;
		if (count >= 1)
		{
			myFile << "\n";
		}
		count--;
	}
	myFile.close();
	return 0;
}


int main()
{
	RenderWindow window(VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "DanmakuHou", Style::Close | Style::Resize /*| Style::Fullscreen*/);

	View view;
	Texture MenuBG;
	Texture ScoreBG;
	Texture playerTexture;
	Texture playeropTexture;
	Texture playerTexturetran;
	Texture BGTexture1;
	Texture enemy01Texture;
	Texture enemy02Texture;
	Texture BulletTexture;
	Texture BoomTexture;
	Texture SmallRed;
	Texture BigBlue;
	Texture BigGreen;
	Texture Black;
	Texture BossTexture1;
	Texture BossTexture2;
	Texture BossTexture3;
	Texture JirenTexture;
	Texture playerfaceTexture;
	Texture playeropfaceTexture;
	Texture Boss1faceTexture;
	Texture Boss2faceTexture;
	Texture Boss3faceTexture;
	Texture JirenfaceTexture;
	Texture PowerTexture;
	Texture LifeTexture;
	Texture SPCardTexture;
	Texture LifeTextureText;
	Texture SPCardTextureText;
	Texture Marisabullet1;
	Texture Marisabullet2;
	Texture Cirnobullet1;
	Texture Cirnobullet2;
	Texture Ayabullet1;
	Texture Ayabullet2;
	Texture JirenBeam;
	Texture ReimuBeam;
	
	ScoreBG.loadFromFile("Picture/Menu.png");
	MenuBG.loadFromFile("Picture/MenuBG.png");
	BulletTexture.loadFromFile("Picture/bulletplayer.png");
	BoomTexture.loadFromFile("Picture/boom.png");
	playerTexture.loadFromFile("Picture/player.png");
	playeropTexture.loadFromFile("Picture/playerop.png");
	playerTexturetran.loadFromFile("Picture/transparent.png");
	BGTexture1.loadFromFile("Picture/BG1.png");
	enemy01Texture.loadFromFile("Picture/Enemy01.png");
	enemy02Texture.loadFromFile("Picture/Enemy02.png");
	SmallRed.loadFromFile("Picture/bull1.png");
	BigBlue.loadFromFile("Picture/bull3.png");
	BigGreen.loadFromFile("Picture/bull2.png");
	Black.loadFromFile("Picture/black.png");
	BossTexture1.loadFromFile("Picture/boss1.png");
	BossTexture2.loadFromFile("Picture/boss2.png");
	BossTexture3.loadFromFile("Picture/boss3.png");
	JirenTexture.loadFromFile("Picture/Jirens.png");
	playerfaceTexture.loadFromFile("Picture/Reimu.png");
	playeropfaceTexture.loadFromFile("Picture/Reimuop.png");
	Boss1faceTexture.loadFromFile("Picture/Marisa.png");
	Boss2faceTexture.loadFromFile("Picture/Cirno.png");
	Boss3faceTexture.loadFromFile("Picture/Shameimaru.png");
	JirenfaceTexture.loadFromFile("Picture/Jiren.png");
	PowerTexture.loadFromFile("Picture/Power.png");
	LifeTexture.loadFromFile("Picture/LIFE.png");
	SPCardTexture.loadFromFile("Picture/SpellCard.png");
	LifeTextureText.loadFromFile("Picture/LIFETEXT.png");
	SPCardTextureText.loadFromFile("Picture/SpellCardTEXT.png");
	Marisabullet1.loadFromFile("Picture/MarisaBullet.png");
	Marisabullet2.loadFromFile("Picture/MarisaBullet2.png");
	Cirnobullet1.loadFromFile("Picture/CirnoBullet.png");
	Cirnobullet2.loadFromFile("Picture/CirnoBullet2.png");
	Ayabullet1.loadFromFile("Picture/Shameimarubullet.png");
	Ayabullet2.loadFromFile("Picture/Shameimarubullet2.png");
	JirenBeam.loadFromFile("Picture/JirenBeam.png");
	ReimuBeam.loadFromFile("Picture/ReimuBeam.png");

	Font font;
	font.loadFromFile("Picture/THSarabunNew.ttf");
	

	Text Danmakuhou;
	Text score, BOOM, life,POW;
	Text ScoreText, BoomText, lifeText, POWText;
	Text Mode1, Mode2, Mode3;
	Text Back;
	Text hs, a, b;
	Text playername, entername;
	Text PAUSE;
	Text CutsceneText;
	String yourname;
	Text DEV;
	Text Howto;

	Danmakuhou.setCharacterSize(100);
	Danmakuhou.setFont(font);
	Danmakuhou.setPosition(window.getSize().x / 2 - 700, window.getSize().y / 2 - 200);
	Danmakuhou.setFillColor(Color::White);

	DEV.setCharacterSize(50);
	DEV.setFont(font);
	DEV.setPosition(window.getSize().x / 2 - 700, window.getSize().y / 2 - 100);
	DEV.setFillColor(Color::White);

	score.setCharacterSize(50);
	score.setFont(font);
	score.setPosition(1500, 100);
	score.setFillColor(Color::White);

	ScoreText.setCharacterSize(50);
	ScoreText.setFont(font);
	ScoreText.setPosition(1100, 100);
	ScoreText.setFillColor(Color::White);

	BOOM.setCharacterSize(50);
	BOOM.setFont(font);
	BOOM.setPosition(1500, 300);
	BOOM.setFillColor(Color::White);

	BoomText.setCharacterSize(50);
	BoomText.setFont(font);
	BoomText.setPosition(1100, 300);
	BoomText.setFillColor(Color::White);

	life.setCharacterSize(50);
	life.setFont(font);
	life.setPosition(1500, 200);
	life.setFillColor(Color::White);

	lifeText.setCharacterSize(50);
	lifeText.setFont(font);
	lifeText.setPosition(1100, 200);
	lifeText.setFillColor(Color::White);

	POW.setCharacterSize(50);
	POW.setFont(font);
	POW.setPosition(1500, 400);
	POW.setFillColor(Color::White);

	POWText.setCharacterSize(50);
	POWText.setFont(font);
	POWText.setPosition(1100, 400);
	POWText.setFillColor(Color::White);

	Howto.setCharacterSize(50);
	Howto.setFont(font);
	Howto.setPosition(1100, 600);
	Howto.setFillColor(Color::White);

	Mode1.setCharacterSize(50);
	Mode1.setFont(font);
	Mode1.setPosition(window.getSize().x / 2 - 600, window.getSize().y / 2 + 100);
	Mode1.setFillColor(Color::White);

	Mode2.setCharacterSize(50);
	Mode2.setFont(font);
	Mode2.setPosition(window.getSize().x / 2 - 600, window.getSize().y / 2 + 200);
	Mode2.setFillColor(Color::White);

	Mode3.setCharacterSize(50);
	Mode3.setFont(font);
	Mode3.setPosition(window.getSize().x / 2 - 600, window.getSize().y / 2 + 300);
	Mode3.setFillColor(Color::White);

	Back.setCharacterSize(50);
	Back.setFont(font);
	Back.setPosition(window.getSize().x / 2 - 800, window.getSize().y / 2 + 400);
	Back.setFillColor(Color::White);

	entername.setCharacterSize(50);
	entername.setFont(font);
	entername.setPosition(window.getSize().x / 2-300, window.getSize().y / 3);
	entername.setFillColor(Color::White);
	entername.setString("Enter Your Name");

	CutsceneText.setCharacterSize(40);
	CutsceneText.setFont(font);
	CutsceneText.setPosition(450,800);
	CutsceneText.setFillColor(Color::White);

	Clock clock, clock1, clockE, clockBoompush,clockFrame;
	Time timeElapsed = clock.restart();

	Platform platform1(&Black, Vector2f(100, 2000), Vector2f(0, 0));
	Platform platform2(&Black, Vector2f(1000.0f, 2000), Vector2f(1000, 0));

	Player player(Vector2f(15.0f, 60.0f), &BulletTexture, &playerTexture, Vector2f(8, 5.4), 0.2f, 300.0f, VIEW_HEIGHT, VIEW_WIDTH, clock);

	std::vector<Bullet> bulletmove;
	std::vector<Bullet> boommove;
	Bullet b1(&BulletTexture, Vector2f(25.0f, 30.0f));
	Bullet b2(&BulletTexture, Vector2f(25.0f, 30.0f));
	Bullet b3(&BulletTexture, Vector2f(25.0f, 30.0f));
	Bullet b4(&BulletTexture, Vector2f(25.0f, 30.0f));
	Bullet boom(&BoomTexture, Vector2f(700.0f, 700.0f));
	std::vector<Bullet> enemy01BulletMove;
	std::vector<Bullet> enemy02BulletMove;
	Bullet enemyb01(&BigBlue, Vector2f(30.0f, 30.0f));
	Bullet enemyb02(&SmallRed, Vector2f(30.0f, 30.0f));
	std::vector<Item> ItemMove;
	Item Power(Vector2f(20.0f, 20.0f));
	Power.Itemshape.setTexture(&PowerTexture);
	


	RectangleShape bg1;
	RectangleShape bg2;
	RectangleShape Menu;
	RectangleShape ScoreShape;
	RectangleShape TextBox;
	RectangleShape Playerface;
	RectangleShape Bossface;
	RectangleShape LifeText;
	RectangleShape SpellCardText;
	RectangleShape Beam;
	bg1.setTexture(&BGTexture1);
	bg2.setTexture(&BGTexture1);
	Menu.setTexture(&MenuBG);
	Playerface.setTexture(&playerfaceTexture);
	ScoreShape.setTexture(&ScoreBG);
	TextBox.setFillColor(Color(0, 0, 0, 0));
	LifeText.setTexture(&LifeTextureText);
	SpellCardText.setTexture(&SPCardTextureText);

	bg1.setSize(Vector2f(900, 2000.0f));
	bg2.setSize(Vector2f(900, 2000.0f));
	Menu.setSize(Vector2f(window.getSize().x, window.getSize().y));
	ScoreShape.setSize(Vector2f(window.getSize().x, window.getSize().y));
	TextBox.setSize(Vector2f(900,400));
	Playerface.setSize(Vector2f(400, 900));
	Bossface.setSize(Vector2f(400, 400));
	LifeText.setSize(Vector2f(50, 50));
	SpellCardText.setSize(Vector2f(50,50));
	Beam.setSize(Vector2f(700,1300));

	bg1.setPosition(Vector2f(100.0f, 0.0f));
	bg2.setPosition(Vector2f(100.0f, bg1.getPosition().y - 2000.0f));
	Menu.setPosition(Vector2f(0, 0));
	ScoreShape.setPosition(Vector2f(0, 0));
	TextBox.setPosition(Vector2f(100,800));
	Playerface.setPosition(0,800);
	Bossface.setPosition(900, 800);
	LifeText.setPosition(1200, 210);
	SpellCardText.setPosition(1200, 310);
	Beam.setPosition(1500, 1500);

	RectangleShape playerhit;

	PAUSE.setCharacterSize(100);
	PAUSE.setFont(font);
	PAUSE.setPosition(500, 100);
	PAUSE.setFillColor(Color::Black);
	PAUSE.setString("PAUSE");

	sf::Vector2f velocity, velocitys, velocityEnemy, velocityEnemyr, velocityBoss, velocityBoss2, velocityEnemy2, velocityEnemyr2;
	float speed = 0.1f, speeds = 300;
	Vector2f direction, boomPosition;
	Vector2i mouse;
	int randomenemys, randomenemyr, randomBoss,randomItem, randombullet1, randombullet2, randombullet3, randombullet4, showplayerdmage;
	srand(time(0));
	
	std::vector<enemy> enemys;
	std::vector<enemy> enemyr;
	std::vector<enemy> enemys2;
	std::vector<enemy> enemyr2;
	std::vector<enemy> Boss1;
	std::vector<enemy> Boss2;
	
	sf::Music MenuMusic, GameplayMusic, Over9000music, JirenMusic;
	MenuMusic.openFromFile("Picture/soundmenu.ogg");
	GameplayMusic.openFromFile("Picture/soundgameplay.ogg");
	Over9000music.openFromFile("Picture/soundOver9000.ogg");
	JirenMusic.openFromFile("Picture/soundJirenarrive.ogg");

	MenuMusic.setVolume(50);
	GameplayMusic.setVolume(15);
	Over9000music.setVolume(15);
	JirenMusic.setVolume(15);

	MenuMusic.setLoop(true);
	GameplayMusic.setLoop(true);
	Over9000music.setLoop(true);
	JirenMusic.setLoop(true);
	MenuMusic.play();

	SoundBuffer bufferboom, buffershoot, bufferenemyshoot, bufferitem, bufferhit, bufferdamage, bufferbeam;
	Sound soundboom, soundshoot, soundenemyshoot, sounditem, soundhit, sounddamage, soundbeam;

	bufferboom.loadFromFile("Picture/soundboom.wav");
	buffershoot.loadFromFile("Picture/soundplayershoot.wav");
	bufferenemyshoot.loadFromFile("Picture/soundenemyshoot.wav");
	bufferitem.loadFromFile("Picture/soundItem.wav");
	bufferhit.loadFromFile("Picture/soundhit.wav");
	bufferdamage.loadFromFile("Picture/sounddamage.wav");
	bufferbeam.loadFromFile("Picture/SoundBeam.wav");

	soundboom.setBuffer(bufferboom);
	soundshoot.setBuffer(buffershoot);
	soundenemyshoot.setBuffer(bufferenemyshoot);
	sounditem.setBuffer(bufferitem);
	soundhit.setBuffer(bufferhit);
	sounddamage.setBuffer(bufferdamage);
	soundbeam.setBuffer(bufferbeam);

	soundshoot.setVolume(20);
	soundboom.setVolume(50);
	soundenemyshoot.setVolume(20);
	sounditem.setVolume(20);
	soundhit.setVolume(10);
	sounddamage.setVolume(20);
	soundbeam.setVolume(3);

	float goY = 900;
	float goX = 550;

	while (window.isOpen())
	{
		window.setFramerateLimit(60.0f);
		deltaTime = clock.restart().asSeconds();
		if (deltaTime > 1.0f / timeElapsed.asSeconds())
			deltaTime = 1.0f / timeElapsed.asSeconds();

		//	window.clear();
		if (mode == 0)
		{
			
			GameplayMusic.stop();
			window.clear(Color::Red);
			Playerface.setTexture(&playerfaceTexture);
			Marisa = false;
			Cirno = false; 
			Aya = false;
			Jiren = false;
			Over9000 = false;
			End = false;
			lastfight = false;
			playerdamage = 1;
			playerboomdamage = 5;
			playerlife = 3;
			playerboom = 5;
			timecount = 0;
			timecounts = 0;
			GO = 0;
			Score = 0;
			CUTSECENE = 0;
			CUTSECENESTATE = 0;
			velocityEnemy = Vector2f(0,0);
			velocityEnemyr = Vector2f(0, 0);
			velocityEnemy2 = Vector2f(0, 0);
			velocityEnemyr2 = Vector2f(0, 0);
			velocityBoss = Vector2f(0, 0);
			velocityBoss2 = Vector2f(0, 0);
			enemyb02.shape.setSize(Vector2f(30, 30));
			Beam.setPosition(Vector2f(1500, 1500));
			enemyb01.shape.setTexture(&BigBlue);
			enemyb02.shape.setTexture(&SmallRed);
			turncheck = 0;
			turncheck2 = 0;
			turncheck3 = 0;
			CutScene = false;
			randomenemys = rand() % 3;
			randomenemyr = rand() % 3;
			randomBoss = rand() % 3;
			randombullet1 = rand() % 3;
			randombullet2 = rand() % 3;
			randombullet3 = rand() % 3;
			randombullet4 = rand() % 3;

			Event evnt;
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::Resized:
					ReSizeView(window, view);
					break;
				}
				if (evnt.type == sf::Event::KeyPressed)
				{
					if (evnt.key.code == sf::Keyboard::Escape)
						window.close();

					if (evnt.key.code == sf::Keyboard::Up)
						modecheck--;
					if (evnt.key.code == sf::Keyboard::Down)
						modecheck++;

					if (evnt.key.code == sf::Keyboard::Return)
					{
						if (modecheck == 1)
						{
							mode = 1;
							modecheck = 0;
							clock.restart();
							clock1.restart();
							clockE.restart();
							enemys.clear();
							enemyr.clear();
							enemys2.clear();
							enemyr2.clear();
							Boss1.clear();
							Boss2.clear();
							enemy01BulletMove.clear();
							enemy02BulletMove.clear();
							ItemMove.clear();
							bulletmove.clear();
							boommove.clear();
							MenuMusic.stop();
							GameplayMusic.play();
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 + 50, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 150, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 250, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 + 150, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 50, -600), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 200 , -600), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 300 , -600), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemys.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 + 200 , -600), &enemy01Texture, Vector2f(4, 6), 0.3f));

							enemyr.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 + 50, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemyr.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 150, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemyr.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 - 250, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));
							enemyr.push_back(enemy(Vector2f(40.0f, 60.0f), Vector2f(550 + 150, -500), &enemy01Texture, Vector2f(4, 6), 0.3f));

							enemys2.push_back(enemy(Vector2f(50.0f, 80.0f), Vector2f(125, -500), &enemy02Texture, Vector2f(4, 6), 0.3f));
							enemys2.push_back(enemy(Vector2f(50.0f, 80.0f), Vector2f(975 , -500), &enemy02Texture, Vector2f(4, 6), 0.3f));


							enemyr2.push_back(enemy(Vector2f(50.0f, 80.0f), Vector2f(-100,20), &enemy02Texture, Vector2f(4, 6), 0.3f));
							enemyr2.push_back(enemy(Vector2f(50.0f, 80.0f), Vector2f(-100,1025), &enemy02Texture, Vector2f(4, 6), 0.3f));
							


							if (randomBoss == 0)
							{
								Boss1.push_back(enemy(Vector2f(70.0f, 100.0f), Vector2f(550 - 100, -500), &BossTexture1, Vector2f(4, 3), 0.3f));
								
								Marisa = true;
							}
							if (randomBoss == 1)
							{
								Boss1.push_back(enemy(Vector2f(70.0f, 100.0f), Vector2f(550 - 100, -500), &BossTexture2, Vector2f(4, 3.8), 0.3f));
								
								Cirno = true;
							}
							if (randomBoss == 2)
							{
								Boss1.push_back(enemy(Vector2f(70.0f, 100.0f), Vector2f(550 - 100, -500), &BossTexture3, Vector2f(4, 3.8), 0.3f));
								Aya = true;
							}
							player.playerbody.setPosition(goX, goY);

							Boss2.push_back(enemy(Vector2f(70.0f, 100.0f), Vector2f(550 +100, -500), &JirenTexture, Vector2f(1, 1), 0.3f));

						}
						else if (modecheck == 2)
						{
							mode = 2;
							modecheck = 0;

						}
						else if (modecheck == 3)
						{
							mode = 3;
							modecheck = 0;

						}
					}
				}
			}
			if (modecheck < 0)
			{
				modecheck = 3;
			}
			else if (modecheck > 3)
			{
				modecheck = 1;
			}
			
			if (modecheck == 0)
			{
				Mode1.setFillColor(Color::White);
				Mode2.setFillColor(Color::White);
				Mode3.setFillColor(Color::White);
			}
			else if (modecheck == 1)
			{
				Mode1.setFillColor(Color::Red);
				Mode2.setFillColor(Color::White);
				Mode3.setFillColor(Color::White);
			}
			else if (modecheck == 2)
			{
				Mode2.setFillColor(Color::Red);
				Mode1.setFillColor(Color::White);
				Mode3.setFillColor(Color::White);
			}
			else if (modecheck == 3)
			{
				Mode3.setFillColor(Color::Red);
				Mode1.setFillColor(Color::White);
				Mode2.setFillColor(Color::White);
			}
			Mode1.setString("Start");
			Mode2.setString("Score");
			Mode3.setString("Exit");
			Danmakuhou.setString("DanmakuHou");
			DEV.setString(L"by นายกรธวัช วิชชุวาณิชย์ 61010023");
			window.draw(Menu);
			window.draw(Mode1);
			window.draw(Mode2);
			window.draw(Mode3);
			window.draw(Danmakuhou);
			window.draw(DEV);

		}
		else if (mode == 1)
		{
			window.clear();
			BulletVelocity.y += bulletspeed;

			Event evnt;
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::Resized:
					ReSizeView(window, view);
					break;
				}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::Escape)
					{
						mode = 5;
						GameplayMusic.pause();
						JirenMusic.pause();
						Over9000music.pause();
					}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::P)
					{
						mode = 0;
						GameplayMusic.stop();
						JirenMusic.stop();
						Over9000music.stop();
						MenuMusic.play();
					}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::U)
					{
						playerdamage+=100;
					}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::L)
					{
						playerlife++;
					}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::B)
					{
						playerboom++;
					}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::Backspace)
					{
						End = true;
					}
			}
			Clock ITEMCLOCK;
			Time Itemtime = ITEMCLOCK.getElapsedTime();
			if (randomBoss == 0)
			{
				Bossface.setTexture(&Boss1faceTexture);
			}
			if (randomBoss == 1)
			{
				Bossface.setTexture(&Boss2faceTexture);
			}
			if (randomBoss == 2)
			{
				Bossface.setTexture(&Boss3faceTexture);
			}
			if (randomBoss == 3)
			{
				Bossface.setTexture(&JirenfaceTexture);
			}
			if (CUTSECENESTATE == 5 && Jiren)
			{
				Beam.setTexture(&JirenBeam);
				Beam.setPosition(200,300);
				soundbeam.play();
			}
			if (CUTSECENESTATE == 6 && Jiren)
			{
				Beam.setPosition(1500, 1500);
			}
			if (CUTSECENESTATE == 9 && Jiren)
			{
				Beam.setTexture(&JirenBeam);
				Beam.setPosition(200, 300);
				soundbeam.play();
			}
			if (CUTSECENESTATE == 10 && Jiren)
			{
				Beam.setPosition(1500, 1500);
			}
			if (CUTSECENESTATE == 12 && Jiren)
			{
				Beam.setTexture(&ReimuBeam);
				Beam.setPosition(200, 0);
				soundbeam.play();
			}
			if (CUTSECENESTATE == 13 && Jiren)
			{
				Beam.setPosition(1500, 1500);
			}
			if (Over9000 && CUTSECENESTATE > 5)
			{
				Playerface.setTexture(&playeropfaceTexture);
				player.playerbody.setTexture(&playeropTexture);
			}
			Clock clockcount;
			Time Count = clockcount.getElapsedTime();
			Time elapsed1 = clock1.getElapsedTime();
			if (!CutScene)
			{
				Playerface.setPosition(1500, 1500);
				Bossface.setPosition(1500, 1500);
				TextBox.setFillColor(Color(0, 0, 0, 0));
				CutsceneText.setPosition(1500, 1500);
				if (Count.asMilliseconds() <= 100)
				{
					timecount++;
					clockcount.restart();
				}
				if (timecount >= 40)
				{
					timecounts++;

					timecount = 0;
				}
			}
			else
			{
				soundenemyshoot.stop();
				TextBox.setFillColor(Color(0, 0, 0, 200));
				CutsceneText.setPosition(400, 850);
				if (elapsed1.asMilliseconds() >= 100)
				{
					clock1.restart().asMilliseconds();
					if (Keyboard::isKeyPressed(Keyboard::Space))
					{
						CUTSECENE++;
						CUTSECENESTATE++;
					}
				}
				if (CUTSECENE % 2 == 0)
				{
					Bossface.setPosition(0, 800);
					Playerface.setPosition(1500, 1500);
					if (CUTSECENESTATE == 0)
					{
						if (Marisa)
						{
							CutsceneText.setString(L"หยุดนะเรย์มุ!!!");
						}
						if (Cirno)
						{
							CutsceneText.setString(L"เธอจะทำอะไรกันน่ะ เรย์มุ");
						}
						if (Aya)
						{
							CutsceneText.setString(L"เรย์มุ เกิดอะไรขึ้นกับเธอกันแน่");
						}
						if (Jiren)
						{
							CutsceneText.setString(L"ที่นี่สินะเก็นโซเคียว");
						}
					}
					if (CUTSECENESTATE == 2)
					{
						if (Marisa)
						{
							CutsceneText.setString(L"ทำไมเธอถึงต้องการที่จะปลดผนึกประตูของฮาคุเรย์\nกันล่ะ");
						}
						if (Cirno)
						{
							CutsceneText.setString(L"กล้าได้ยังไงถึงได้พูดแบบนั้นออกมา \nวันนี้ฉันแช่แข็งมันไปแล้วไม่โดนกินแน่นอน");
						}
						if (Aya)
						{
							CutsceneText.setString(L"ตอนแรกก็แค่ว่าจะมาทำข่าวแต่ดูเหมือนว่าจะอยู่เฉยๆ\nไม่ได้แล้วสินะ");
						}
						if (Jiren)
						{
							CutsceneText.setString(L"ต่อให้เป็นมิโกะแห่งฮาคุเรย์ก็ทำอะไรข้าไม่ได้หรอกนะยัยหนู");
						}
					}
					if (CUTSECENESTATE == 4)
					{
						if (Marisa)
						{
							CutsceneText.setString(L"ไม่ว่ายังไงฉันก็ไม่ยอมให้เธอทำสำเร็จหลอกนะ");
						}
						if (Cirno)
						{
							CutsceneText.setString(L"โง่? ถึงจะยังงงๆอยู่ก็เถอะแต่รูสึกว่าถ้าปล่อยให้ไปแล้วละก็\nจะต้องเกิดเรื่องแน่ๆเลย เพราะงั้นไม่ยอมหลอก");
						}
						if (Aya)
						{
							CutsceneText.setString(L"ข่าวที่จะลงก็คือ\n\"มิโกะแห่งฮาคุเรย์เจ็บหนักคาดทำตัวเองแท้ๆ\"\nเพราะฉะนั้นฉันจะหยุดเธอเอง");
						}
						if (Jiren)
						{
							CutsceneText.setString(L"ไร้สาระ");
						}
					}
					if (CUTSECENESTATE == 6)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"!!! \nป้องกันไว้ได้งั้นเหรอ...หรือว่าพลังของมันมีมากกว่า9000!!!");
						}
					}
					if (CUTSECENESTATE == 8)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"เอาไปอีกรอบ!!!");
						}
					}
					if (CUTSECENESTATE == 10)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"บ้าน่ะ!!!นี่ข้าแพ้อย่างนั้นรึ");
						}
					}
					if (CUTSECENESTATE == 12)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"อ้ากกกกกกกกก!!!");
						}
					}
				}
				else
				{
					Playerface.setPosition(0, 800);
					Bossface.setPosition(1500, 1500);
					if (CUTSECENESTATE == 1)
					{
						if (Marisa)
						{
							CutsceneText.setString(L"ถอยไป มาริสะ");
						}
						if (Cirno)
						{
							CutsceneText.setString(L"กลับเขาไปในปากคางคกไป ยัยโง่ชิลโน่");
						}
						if (Aya)
						{
							CutsceneText.setString(L"อายะ อย่ามาขวางทางฉัน");
						}
						if (Jiren)
						{
							CutsceneText.setString(L"ฉันเห็นจากนิมิตแล้ว นายคือจิเร็นสินะ\nฉันไม่ยอมให้นายทำลายที่นี่อย่างแน่นอน");
						}
					}
					if (CUTSECENESTATE == 3)
					{
						if (Marisa)
						{
							CutsceneText.setString(L"ก็เพราะว่าขี้เกียจแล้วยังไงละการเป็นมิโกะเฝ้าศาลนั้นน่ะ");
						}
						if (Cirno)
						{
							CutsceneText.setString(L"ยัยโง่ก็ยังเป็นแค่ยัยโง่อยู่วันยังค่ำสินะ ความรู้สึกช้าจริงๆ\nเอาล่ะหลีกไปได้แล้ว");
						}
						if (Aya)
						{
							CutsceneText.setString(L"ถ้าอย่างงั้นก็ช่วยทำข่าวหน่อยละกันนะว่า\n\"ประตูแห่งเก็นโซเคียว\"ได้ถูกเปิดแล้ว ");
						}
						if (Jiren)
						{
							CutsceneText.setString(L"ไม่ลองก็ไม่รู้หรอก ฉันจะหยุดแกเอง!!!");
						}
						
					}
					if (CUTSECENESTATE == 5)
					{
						if (Marisa)
						{
							CutsceneText.setString(L"ถ้าอย่านั้นก็อย่ามาเสียใจทีหลังละกัน...มาริสะ!!!");
						}
						if (Cirno)
						{
							CutsceneText.setString(L"กลับไปลงท้องคากคกไปยัยชิลโง่");
						}
						if (Aya)
						{
							CutsceneText.setString(L"คนที่จะต้องขึ้นหน้าหนึ่งคือเธอต่างหาก\nอายะ ชาเมมารุ");
						}
						if (Jiren)
						{
							CutsceneText.setString(L"อึก!!!\n(บ้าจริงมาได้แค่นี้หรอเนี่ยขอโทษนะทุกค...)");
						}
					}
					if (CUTSECENESTATE == 7)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"...");
							JirenMusic.stop();
							Over9000music.play();
						}
					}
					if (CUTSECENESTATE == 9)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"... ถึงตาของทางนี้บ้างละนะ");
						}
					}
					if (CUTSECENESTATE == 11)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"ลาขาดละนะ เก็นโซเคียวแห่งนี้จะไม่มวันถูกทำลายโดยเด็ดขาด");
						}
					}
					if (CUTSECENESTATE == 13)
					{
						if (Jiren)
						{
							CutsceneText.setString(L"ฟู่~~~จบซะที");
						}
					}
					
				}

				if (CUTSECENE >= 2 && GO == 0)
				{
					CutScene = false;
					CUTSECENESTATE = 2;
					GO++;
				}
				if (CUTSECENE > 5  && GO == 1)
				{
					CutScene = false;
					CUTSECENE = 0;
					CUTSECENESTATE = 0;
					GO++;
				}
				if (CUTSECENE > 5 && GO == 2 && !Over9000)
				{
					CutScene = false;
					End = true;
					CUTSECENE = 0;
					GO++;
				}
				if (CUTSECENE > 9 && GO == 2 && Over9000)
				{
					CutScene = false;
					lastfight = true;
					GO++;
				}
				if (CUTSECENE > 13 && GO == 3)
				{
					CutScene = false;
					CUTSECENE = 0;
					End = true;
					GO++;
				}
				
			}
			/*std::cout << timecount;
			std::cout << " ";
			std::cout << timecounts;
			std::cout << " ";
			std::cout << GO;
			std::cout << " ";
			std::cout << CUTSECENE;
			std::cout << " ";
			std::cout << CUTSECENESTATE << std::endl;*/
			//Player
			player.Update(canshoot,deltaTime, VIEW_HEIGHT, VIEW_WIDTH, window, timeElapsed);
			if (canshoot && !CutScene)
			{
				if (elapsed1.asMilliseconds() >= 90)
				{
					clock1.restart().asMilliseconds();
					if (Keyboard::isKeyPressed(Keyboard::Z)||Mouse::isButtonPressed(Mouse::Left))
					{
						soundshoot.play();
						if (playerdamage < 3)
						{
							b1.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
							b1.currVelocity.y = -30;
							bulletmove.push_back(Bullet(b1));
						}
						else if (playerdamage < 6)
						{
							b1.shape.setPosition(player.GetPosition().x - 20, player.GetPosition().y - b1.shape.getSize().y);
							b2.shape.setPosition(player.GetPosition().x + 20, player.GetPosition().y - b1.shape.getSize().y);
							b1.currVelocity.y = -30;
							b2.currVelocity.y = -30;
							b2.currVelocity.x = 0;
							bulletmove.push_back(Bullet(b1));
							bulletmove.push_back(Bullet(b2));
						}
						else if (playerdamage < 9)
						{
							if (visible)
							{
								b1.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
								b2.shape.setPosition(player.GetPosition().x-20, player.GetPosition().y - b1.shape.getSize().y);
								b3.shape.setPosition(player.GetPosition().x+20, player.GetPosition().y - b1.shape.getSize().y);
								b1.currVelocity.y = -30;
								b2.currVelocity.y = -30;
								b2.currVelocity.x = 0;
								b3.currVelocity.y = -30;
								b3.currVelocity.x = 0;
							}
							else
							{
								b1.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
								b2.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
								b3.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
								b1.currVelocity.y = -30;
								b2.currVelocity.y = -30;
								b2.currVelocity.x = -5;
								b3.currVelocity.y = -30;
								b3.currVelocity.x = 5;
							}
							bulletmove.push_back(Bullet(b1));
							bulletmove.push_back(Bullet(b2));
							bulletmove.push_back(Bullet(b3));
						}
						else
						{
							if (visible)
							{
								b1.shape.setPosition(player.GetPosition().x - 20, player.GetPosition().y - b1.shape.getSize().y);
								b2.shape.setPosition(player.GetPosition().x + 20, player.GetPosition().y - b1.shape.getSize().y);
								b3.shape.setPosition(player.GetPosition().x - 40, player.GetPosition().y - b1.shape.getSize().y);
								b4.shape.setPosition(player.GetPosition().x + 40, player.GetPosition().y - b1.shape.getSize().y);
								b1.currVelocity.y = -30;
								b2.currVelocity.y = -30;
								b2.currVelocity.x = 0;
								b3.currVelocity.y = -30;
								b3.currVelocity.x = 0;
								b4.currVelocity.y = -30;
								b4.currVelocity.x = 0;
							}
							else 
							{
								b1.shape.setPosition(player.GetPosition().x - 20, player.GetPosition().y - b1.shape.getSize().y);
								b2.shape.setPosition(player.GetPosition().x + 20, player.GetPosition().y - b1.shape.getSize().y);
								b3.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
								b4.shape.setPosition(player.GetPosition().x, player.GetPosition().y - b1.shape.getSize().y);
								b1.currVelocity.y = -30;
								b2.currVelocity.y = -30;
								b2.currVelocity.x = 0;
								b3.currVelocity.y = -30;
								b3.currVelocity.x = 7;
								b4.currVelocity.y = -30;
								b4.currVelocity.x = -7;
							}							
							bulletmove.push_back(Bullet(b1));
							bulletmove.push_back(Bullet(b2));
							bulletmove.push_back(Bullet(b3));
							bulletmove.push_back(Bullet(b4));
						}
						

					}
				}
				Time boomdlapsed = clockBoompush.getElapsedTime();
				if (boomdlapsed.asMilliseconds() >= 90)
				{
					
					clockBoompush.restart().asMilliseconds();
					if ((Keyboard::isKeyPressed(Keyboard::X) || Mouse::isButtonPressed(Mouse::Right) )&& canBoom)
					{
						soundboom.play();
						playerboom--;
						canBoomDamage = true;
						boom.currVelocity.y = -5;
						boom.shape.setPosition(player.GetPosition().x, player.GetPosition().y);
						boommove.push_back(Bullet(boom));
					}
				}
			}
			if (playerboom < 1)
			{
				canBoom = false;
			}
			else
			{
				canBoom = true;
			}

			for (size_t i = 0; i < bulletmove.size(); i++)
			{
				bulletmove[i].shape.move(bulletmove[i].currVelocity);
				bulletmove[i].shape.rotate(6);
				if (bulletmove[i].shape.getPosition().x < 0 || bulletmove[i].shape.getPosition().x > window.getSize().x
					|| bulletmove[i].shape.getPosition().y < 0 || bulletmove[i].shape.getPosition().y > window.getSize().y)
				{
					bulletmove.erase(bulletmove.begin() + i);
				}
			}
			for (size_t i = 0; i < boommove.size(); i++)
			{
				boommove[i].shape.rotate(1);
				boommove[i].shape.move(boommove[i].currVelocity);
				if (boommove[i].shape.getPosition().x < 0 || boommove[i].shape.getPosition().x > window.getSize().x
					|| boommove[i].shape.getPosition().y < 0 || boommove[i].shape.getPosition().y > window.getSize().y||CutScene)
				{
					boommove.erase(boommove.begin() + i);
				}
			}


			if (visible)
			{
				playerhit.setFillColor(sf::Color::Green);
			}
			else
			{
				playerhit.setFillColor(sf::Color::Transparent);
			}
			playerhit.setSize(Vector2f(5, 5));
			playerhit.setOrigin(playerhit.getSize() / 2.0f);
			playerhit.setPosition(player.GetPosition().x, player.GetPosition().y);
			if (Keyboard::isKeyPressed(Keyboard::LControl))
			{
				visible = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::LShift))
			{
				visible = false;

			}
			if (playerdamage < 1)
			{
				playerdamage = 1;
			}
			//enemy


			int m;
			Collider playercol(playerhit);
			Collider playernearcol = player.GetCollider();
			const double Pi = 3.14159265358979323846;
			double r = 20;
			Time elapsedE = clockE.getElapsedTime();
			Time elapsedES = clockE.getElapsedTime();
			for (size_t i = 0; i < enemys.size(); i++)
			{
				row = 0;
				if (timecounts <= 3)
					{
						velocityEnemy.x = 0;
						velocityEnemy.y = 5;
					}
				if (timecounts <= 8 && timecounts > 3)
					{
						velocityEnemy.y = 0;
						for (size_t j = 0; j < 5; j++)
						{
							float y ;
							float x ;
							if (randombullet1 == 0)
							{
								y = (r * cos((size_t)j/Pi))*0.1;
								x = (r * sin((size_t)j/Pi))*0.1;
							}
							else if (randombullet1 == 1)
							{
								y = 8;
								x = 0;
							}
							else if (randombullet1 == 2)
							{
								y = (r * cos((size_t)j / (-Pi)))*0.1;
								x = (r * sin((size_t)j / (-Pi)))*0.1;
							}
							if (elapsedE.asMilliseconds() >= 500)
							{
								clockE.restart().asMilliseconds();
								soundenemyshoot.play();
								enemyb01.shape.setPosition(enemys[i].GetPosition().x, enemys[i].GetPosition().y + 30.0f - enemyb01.shape.getSize().y);
								enemyb01.currVelocity.x = x ;
								enemyb01.currVelocity.y = y ;
								enemy01BulletMove.push_back(Bullet(enemyb01));
							}
						}
					}
				if (timecounts > 8)
					{
						if (enemys[i].GetPosition().x <= 300)
						{
							velocityEnemy.x = -2;
							faceRight = false;
						}
						else
						{
							velocityEnemy.x = 2;
							faceRight = true;
						}
						row = 2;
					}
				enemys[i].Update(velocityEnemy, faceRight, row, deltaTime);
	
			}
			for (size_t k = 0; k < enemyr.size(); k++)
			{
				row = 3;
				if (timecounts <= 13 && timecounts > 10)
				{
					velocityEnemyr.x = 0;
					velocityEnemyr.y = 6;
				}
				if (timecounts <= 18 && timecounts > 13)
				{
					velocityEnemyr.y = 0;
					for (size_t l = 0; l < 10; l++)
					{
						float y;
						float x;
						if (randombullet2 == 0)
						{
							y = (r * cos((size_t)l / (Pi / 2)))*0.1;
							x = (r * sin((size_t)l / (Pi / 2)))*0.1;
						}
						else if (randombullet2 == 1)
						{
							y = 8;
							x = 0;
						}
						else if (randombullet2 == 2)
						{
							y = (r*2 * cos((size_t)l / (-Pi)))*0.1;
							x = (r*2 * sin((size_t)l / (-Pi)))*0.1;
						}
						if (elapsedES.asMilliseconds() >= 700)
						{
							clockE.restart().asMilliseconds();
							soundenemyshoot.play();

							enemyb02.shape.setPosition(enemyr[k].GetPosition().x, enemyr[k].GetPosition().y);
							enemyb02.currVelocity.x = x ;
							enemyb02.currVelocity.y = y ;
							enemy02BulletMove.push_back(Bullet(enemyb02));

						}
					}
				}
				if (timecounts > 18)
				{

					if (enemyr[k].GetPosition().x <= 300)
					{
						velocityEnemyr.x = -2;
						faceRight = false;
					}
					else
					{
						velocityEnemyr.x = 2;
						faceRight = true;
					}

					row = 5;
				}
				enemyr[k].Update(velocityEnemyr, faceRight, row, deltaTime);
			}
			for (size_t i = 0; i < enemys2.size(); i++)
			{
				row = 0;
				if (timecounts >= 20)
				{
					if (enemys2[i].GetPosition().y <= 1100 && turncheck == 0)
					{
						velocityEnemy2.y = 6;
						velocityEnemy2.x = 0;
					}
					else if (enemys2[i].GetPosition().y >= 0 && turncheck == 1)
					{
						velocityEnemy2.y = -6;
						velocityEnemy2.x = 0;
					}
					if (enemys2[i].GetPosition().y >= 1100 && turncheck == 0)
					{
						turncheck = 1;
					}
					else if (enemys2[i].GetPosition().y <= 0 && turncheck == 1)
					{
						turncheck = 0;
					}
				}
				if (timecounts <= 40 && timecounts > 25)
				{
					for (size_t j = 0; j < 10; j++)
					{
						float y ;
						float x ;
						enemyb01.shape.setTexture(&BigGreen);
						if (randombullet3 == 0)
						{
							if (enemys2[i].GetPosition().x <= 300)
							{
								y = 0;
								x = 2;
							}
							else
							{
								y = 0;
								x = -2;
							}
						}
						if (randombullet3 == 1)
						{
							if (enemys2[i].GetPosition().x <= 300)
							{
								y = 0;
								x = 4;
							}
							else
							{
								y = 0;
								x = -4;
							}
						}
						if (randombullet3 == 2)
						{
							if (enemys2[i].GetPosition().x <= 300)
							{
								y = 0;
								x = 6;
							}
							else
							{
								y = 0;
								x = -6;
							}
						}
						
						if (elapsedE.asMilliseconds() >= 200)
						{
							clockE.restart().asMilliseconds();
							soundenemyshoot.play();

							enemyb01.shape.setPosition(enemys2[i].GetPosition().x, enemys2[i].GetPosition().y + 30.0f - enemyb01.shape.getSize().y);
							enemyb01.currVelocity.x = x;
							enemyb01.currVelocity.y = y;
							enemy01BulletMove.push_back(Bullet(enemyb01));
						}
					}
				}
				if (timecounts > 40)
				{
					if (enemys2[i].GetPosition().x <= 300)
					{
						velocityEnemy2.x = -2;
						faceRight = false;
					}
					else
					{
						velocityEnemy2.x = 2;
						faceRight = true;
					}
					row = 2;
				}
				enemys2[i].Update(velocityEnemy2, faceRight, row, deltaTime);

			}
			for (size_t i = 0; i < enemyr2.size(); i++)
			{
				row = 5;
				if (timecounts >= 20)
				{
					if (enemyr2[i].GetPosition().x <= 975 && turncheck2 == 0)
					{
						velocityEnemyr2.y = 0;
						velocityEnemyr2.x = 6; 
						faceRight = true;
					}
					else if (enemyr2[i].GetPosition().x >= 100 && turncheck2 == 1)
					{
						velocityEnemyr2.y = 0;
						velocityEnemyr2.x = -6;
						faceRight = false;
					}
					if (enemyr2[i].GetPosition().x >= 975 && turncheck2 == 0)
					{
						turncheck2 = 1;
					}
					else if (enemyr2[i].GetPosition().x <= 100 && turncheck2 == 1)
					{
						turncheck2 = 0;
					}
				}
				if (timecounts <= 40 && timecounts > 25)
				{
					for (size_t j = 0; j < 10; j++)
					{
						float y;
						float x;
						enemyb02.shape.setTexture(&BigBlue);
						if (randombullet4 == 0)
						{
							if (enemyr2[i].GetPosition().y <= 300)
							{
								y = 2;
								x = 0;
							}
							else
							{
								y = -2;
								x = 0;
							}
						}
						if (randombullet4 == 1)
						{
							if (enemyr2[i].GetPosition().y <= 300)
							{
								y = 4;
								x = 0;
							}
							else
							{
								y = -4;
								x = 0;
							}
						}
						if (randombullet4 == 2)
						{
							if (enemyr2[i].GetPosition().y <= 300)
							{
								y = 6;
								x = 0;
							}
							else
							{
								y = -6;
								x = 0;
							}
						}

						if (elapsedE.asMilliseconds() >= 200)
						{
							clockE.restart().asMilliseconds();
							soundenemyshoot.play();

							enemyb02.shape.setPosition(enemyr2[i].GetPosition().x, enemyr2[i].GetPosition().y + 30.0f - enemyb02.shape.getSize().y);
							enemyb02.currVelocity.x = x;
							enemyb02.currVelocity.y = y;
							enemy02BulletMove.push_back(Bullet(enemyb02));
						}
					}
				}
				if (timecounts > 40)
				{
					if (enemyr2[i].GetPosition().x <= 300)
					{
						velocityEnemyr2.x = -2;
						faceRight = false;
					}
					else
					{
						velocityEnemyr2.x = 2;
						faceRight = true;
					}
					row = 5;
				}
				enemyr2[i].Update(velocityEnemyr2, faceRight, row, deltaTime);
			}
			for (size_t i = 0; i < Boss1.size(); i++)
			{
				row = 0;
				if (timecounts >= 45)
				{
					velocityBoss.x = 0;
					velocityBoss.y = 9;
				}
				if (timecounts > 46)
				{
					velocityBoss.y = 0;
					if (GO == 0)
					{
						CutScene = true;
					}
				}
				if (timecounts > 49)
				{
					
					row = 1;
					if (Marisa)
					{
						if (bosshp >= 2500)
						{
							enemyb02.shape.setSize(Vector2f(35, 35));
							enemyb02.shape.setTexture(&Marisabullet1);
						}
						else
						{
							enemyb02.shape.setSize(Vector2f(100, 100));
							enemyb02.shape.setTexture(&Marisabullet2);
						}
						
					}
					else if (Cirno)
					{
						if (bosshp >= 2500)
						{
							enemyb02.shape.setSize(Vector2f(35, 35));
							enemyb02.shape.setTexture(&Cirnobullet1);
						}
						else
						{
							enemyb02.shape.setSize(Vector2f(35, 35));
							enemyb02.shape.setTexture(&Cirnobullet2);

						}
					}
					else if (Aya)
					{
						if (bosshp >= 2500)
						{
							enemyb02.shape.setSize(Vector2f(35, 35));
							enemyb02.shape.setTexture(&Ayabullet1);
						}
						else
						{
							enemyb02.shape.setSize(Vector2f(35,35));
							enemyb02.shape.setTexture(&Ayabullet2);
						}
						
					}
					
					if (Boss1[i].GetPosition().x <= 975 && turncheck3 == 0)
					{
						velocityBoss.y = 0;
						velocityBoss.x = 2;
						faceRight = true;
						if (Aya)
						{
							faceRight = false;
						}
					}
					else if (Boss1[i].GetPosition().x >= 100 && turncheck3 == 1)
					{
						velocityBoss.y = 0;
						velocityBoss.x = -2;
						faceRight = false;
						if (Aya)
						{
							faceRight = true;
						}
					}
					if (Boss1[i].GetPosition().x >= 975 && turncheck3 == 0)
					{
						turncheck3 = 1;
					}
					else if (Boss1[i].GetPosition().x <= 100 && turncheck3 == 1)
					{
						turncheck3 = 0;
					}
					for (size_t l = 0; l < 10; l++)
					{
						float y;
						float x;
						
						if (Marisa)
							{
								if (bosshp >= 2500)
								{
									y = r * cos((size_t)l / (Pi / 2)) * 0.09;
									x = r * sin((size_t)l / (Pi / 2)) * 0.09;
								}
								else
								{
									y = 9;
									x = 0;
								}
							}
						else if (Cirno)
							{
								if (bosshp >= 2500)
								{
									y = 9;
									x = 0;
								}
								else
								{
									y = 9;
									x = sin((size_t)l / (Pi / 2));
								}
						}
						else if (Aya)
							{
								if (bosshp >= 2500)
								{
									y = 6;
									x = r / 2 * sin((size_t)l / (Pi / 2)) * 0.1;
								}
								else
								{
									y = r * cos((size_t)l / (Pi )) * 0.09;
									x = r * sin((size_t)l / (Pi )) * 0.09;
								}
						}
						
						
						if (elapsedES.asMilliseconds() >= 500)
						{
							clockE.restart().asMilliseconds();
							soundenemyshoot.play();

							enemyb02.shape.setPosition(Boss1[i].GetPosition().x, Boss1[i].GetPosition().y);
							enemyb02.currVelocity.x = x;
							enemyb02.currVelocity.y = y;
							enemy02BulletMove.push_back(Bullet(enemyb02));
						}
					}
				}
				if (bosshp < 2500)
				{
				
					if (GO == 1)
					{
						velocityBoss.x = 0;
						CutScene = true;
					}
				}
				Boss1[i].Update(velocityBoss, faceRight, row, deltaTime);

			}
			for (size_t i = 0; i < Boss2.size(); i++)
			{
				if (Jiren)
				{
					enemyb02.shape.setSize(Vector2f(40, 40));
					enemyb02.shape.setTexture(&SmallRed);
					enemyb01.shape.setSize(Vector2f(40, 40));
					enemyb01.shape.setTexture(&BigBlue);

					row = 0;
					if (Boss2[i].GetPosition().y < 300)
					{
						velocityBoss2.x = 0;
						velocityBoss2.y = 9;
					}
					if (Boss2[i].GetPosition().y >= 300)
					{
						velocityBoss2.y = 0;
						if (GO == 2)
						{
							CutScene = true;
						}
					}
					if (lastfight)
					{
						if (Boss2[i].GetPosition().x <= 975 && turncheck4 == 0)
						{
							velocityBoss2.y = 0;
							velocityBoss2.x = 2;
							faceRight = true;
							
						}
						else if (Boss2[i].GetPosition().x >= 100 && turncheck4 == 1)
						{
							velocityBoss2.y = 0;
							velocityBoss2.x = -2;
							faceRight = false;
							
						}
						if (Boss2[i].GetPosition().x >= 975 && turncheck4 == 0)
						{
							turncheck4 = 1;
						}
						else if (Boss2[i].GetPosition().x <= 100 && turncheck4 == 1)
						{
							turncheck4 = 0;
						}
						for (size_t l = 0; l < 10; l++)
						{
							float y,yy;
							float x, xx;

							y = r * cos((size_t)rand() / (Pi)) * 0.09;
							x = r * sin((size_t)rand() / (Pi)) * 0.09;
							yy = r * cos((size_t)l / (Pi/2)) * 0.09;
							xx = r * sin((size_t)l / (Pi/2)) * 0.09;

							if (elapsedES.asMilliseconds() >= 100)
							{
								clockE.restart().asMilliseconds();
								soundenemyshoot.play();

								enemyb02.shape.setPosition(Boss2[i].GetPosition().x, Boss2[i].GetPosition().y);
								enemyb02.currVelocity.x = x;
								enemyb02.currVelocity.y = y;
								enemy02BulletMove.push_back(Bullet(enemyb02));

								enemyb01.shape.setPosition(Boss2[i].GetPosition().x, Boss2[i].GetPosition().y);
								enemyb01.currVelocity.x = xx;
								enemyb01.currVelocity.y = yy;
								enemy01BulletMove.push_back(Bullet(enemyb01));
							}
						}
					}
					if (Jirenhp <= 0)
					{
						velocityBoss2.x = 0;
					}
				
					Boss2[i].Update(velocityBoss2, faceRight, row, deltaTime);
				}
			}


			for (size_t j = 0; j < enemy01BulletMove.size(); j++)
			{
				enemy01BulletMove[j].shape.move(enemy01BulletMove[j].currVelocity);
				if (enemy01BulletMove[j].shape.getPosition().x < 100 || enemy01BulletMove[j].shape.getPosition().x > 1000
					|| enemy01BulletMove[j].shape.getPosition().y < 0 || enemy01BulletMove[j].shape.getPosition().y > window.getSize().y
					|| CutScene)
				{
					enemy01BulletMove.erase(enemy01BulletMove.begin() + j);
				}
			}
			for (size_t j = 0; j < enemy02BulletMove.size(); j++)
			{
				enemy02BulletMove[j].shape.move(enemy02BulletMove[j].currVelocity);
				if (enemy02BulletMove[j].shape.getPosition().x < 100 || enemy02BulletMove[j].shape.getPosition().x > 1000
					|| enemy02BulletMove[j].shape.getPosition().y < 0 || enemy02BulletMove[j].shape.getPosition().y > window.getSize().y
					|| CutScene)
				{
					enemy02BulletMove.erase(enemy02BulletMove.begin() + j);
				}
			}


			//ชน
			for (enemy& Enemy : enemys)
			{
				if (Enemy.GetCollider().CheckCollision(playercol, direction, 0.7f) && !iFramehit)
				{
					player.playerbody.setPosition(goX, goY);
					sounddamage.play();
					iFramehit = true;
					canshoot = false;
					playerlife--;
				}
			}
			for (enemy& Enemy : enemyr)
			{
				if (Enemy.GetCollider().CheckCollision(playercol, direction, 0.7f) && !iFramehit)
				{
					player.playerbody.setPosition(goX, goY);
					sounddamage.play();
					iFramehit = true;
					canshoot = false;
					playerlife--;
				}
			}
			for (enemy& Enemy : Boss1)
			{
				if (Enemy.GetCollider().CheckCollision(playercol, direction, 0.7f) && !iFramehit)
				{
					player.playerbody.setPosition(goX, goY);
					sounddamage.play();
					iFramehit = true;
					canshoot = false;
					playerlife--;
				}
			}
			for (enemy& Enemy : Boss2)
			{
				if (Enemy.GetCollider().CheckCollision(playercol, direction, 0.7f) && !iFramehit)
				{
					player.playerbody.setPosition(goX, goY);
					sounddamage.play();
					iFramehit = true;
					canshoot = false;
					playerlife--;
				}
			}
			for (Bullet& Enemy : enemy01BulletMove)
			{
				if (Enemy.GetCollider().CheckCollision(playercol, direction, 0.7f) && !iFramehit)
				{
					player.playerbody.setPosition(goX, goY);
					sounddamage.play();
					iFramehit = true;
					canshoot = false;
					playerlife--;
					Enemy.shape.setPosition(-1500, -1500);
				}
				if (Enemy.GetCollider().CheckCollision(playernearcol, direction, 0.7f))
				{
					Score++;
				}
			}
			for (Bullet& Enemy : enemy02BulletMove)
			{
				if (Enemy.GetCollider().CheckCollision(playercol, direction, 0.7f) && !iFramehit)
				{
					sounddamage.play();
					player.playerbody.setPosition(goX, goY);
					iFramehit = true;
					canshoot = false;
					playerlife--;
					Enemy.shape.setPosition(1500, 500);
				}
				if (Enemy.GetCollider().CheckCollision(playernearcol, direction, 0.7f))
				{
					Score++;
				}
			}

			for (size_t j = 0; j < enemy01BulletMove.size(); j++)
			{
				for (size_t i = 0; i < boommove.size(); i++)
				{
					Collider bulletcol = boommove[i].GetCollider();
					if (enemy01BulletMove[j].GetCollider().CheckCollision(bulletcol, direction, 0.7f))
					{
						enemy01BulletMove[j].shape.setPosition(-100, -100);
						Score++;
					}
				}
			}
			for (size_t j = 0; j < enemy02BulletMove.size(); j++)
			{
				for (size_t i = 0; i < boommove.size(); i++)
				{
					Collider bulletcol = boommove[i].GetCollider();
					if (enemy02BulletMove[j].GetCollider().CheckCollision(bulletcol, direction, 0.7f))
					{
						enemy02BulletMove[j].shape.setPosition(-100, -100);
						Score++;
					}
				}
			}

			for (size_t i = 0; i < enemys.size(); i++)
			{
				if (enemys[i].GetPosition().x < 0.0f || enemys[i].GetPosition().x >  window.getSize().x
					|| enemys[i].GetPosition().y > window.getSize().y)
				{
					enemys.erase(enemys.begin() + i);
				}
			}
			for (size_t i = 0; i < enemyr.size(); i++)
			{
				if (enemyr[i].GetPosition().x < 0.0f || enemyr[i].GetPosition().x >  window.getSize().x
					|| enemyr[i].GetPosition().y > window.getSize().y)
				{
					enemyr.erase(enemyr.begin() + i);
				}
			}
			for (size_t i = 0; i < bulletmove.size(); i++)
			{
				for (size_t j = 0; j < enemys.size(); j++)
				{
					Collider bulletcol = bulletmove[i].GetCollider();
					if (enemys[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f))
					{
						soundhit.play();
						enemyshp[j] -= playerdamage;
						bulletmove[i].shape.setPosition(VIEW_WIDTH / 2 + 500, 1);
						Score++;
					}

				}
			}
			for (size_t i = 0; i < bulletmove.size(); i++)
			{
				for (size_t j = 0; j < enemyr.size(); j++)
				{
					Collider bulletcol = bulletmove[i].GetCollider();
					if (enemyr[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f))
					{
						soundhit.play();
						enemyrhp[j] -= playerdamage;
						bulletmove[i].shape.setPosition(VIEW_WIDTH / 2 + 500, 1);
						Score++;
					}
				}
			}
			for (size_t i = 0; i < bulletmove.size(); i++)
			{
				for (size_t j = 0; j < Boss1.size(); j++)
				{
					Collider bulletcol = bulletmove[i].GetCollider();
					if (Boss1[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f))
					{
						soundhit.play();
						bosshp -= playerdamage;
						bulletmove[i].shape.setPosition(VIEW_WIDTH / 2 + 500, 1);
						Score++;
					}
				}
			}
			for (size_t i = 0; i < bulletmove.size(); i++)
			{
				for (size_t j = 0; j < Boss2.size(); j++)
				{
					Collider bulletcol = bulletmove[i].GetCollider();
					if (Boss2[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f))
					{
						soundhit.play();
						Jirenhp -= playerdamage;
						bulletmove[i].shape.setPosition(VIEW_WIDTH / 2 + 500, 1);
						Score++;
					}
				}
			}

			for (size_t i = 0; i < boommove.size(); i++)
			{
				for (size_t j = 0; j < enemys.size(); j++)
				{
					Collider bulletcol = boommove[i].GetCollider();
					if (enemys[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f))
					{

						enemyshp[j] -= playerboomdamage;
						Score += 10;
					}
				}
			}
			for (size_t i = 0; i < boommove.size(); i++)
			{
				for (size_t j = 0; j < enemyr.size(); j++)
				{
					Collider bulletcol = boommove[i].GetCollider();
					if (enemyr[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f))
					{

						enemyrhp[j] -= playerboomdamage;
						Score += 10;
					}
				}
			}
			for (size_t i = 0; i < boommove.size(); i++)
			{
				for (size_t j = 0; j < Boss1.size(); j++)
				{
					Collider bulletcol = boommove[i].GetCollider();
					if (Boss1[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f) && canBoomDamage)
					{
						canBoomDamage = false;
						bosshp -= playerboomdamage;
						Score += 10;
					}
				}
			}
			for (size_t i = 0; i < boommove.size(); i++)
			{
				for (size_t j = 0; j < Boss2.size(); j++)
				{
					Collider bulletcol = boommove[i].GetCollider();
					if (Boss2[j].GetCollider().CheckCollision(bulletcol, direction, 1.0f) && canBoomDamage)
					{
						canBoomDamage = false;
						Jirenhp -= playerboomdamage;
						Score += 10;
					}
				}
			}
			int framecountss =  0 ;
			if (iFramehit)
			{
				player.playerbody.setTexture(&playerTexturetran);
				iFrame = true;
				clockFrame.restart();
			}
			if (framecountss = 0  && iFrame )
			{
				player.playerbody.setTexture(&playerTexture);
				framecount++;
				framecountss++;
				clockFrame.restart();
			}
			if (framecountss = 1 && iFrame)
			{
				player.playerbody.setTexture(&playerTexturetran);
				framecount++;
				framecountss--;
				clockFrame.restart();
			}
			if (framecount > 100)
			{
				player.playerbody.setTexture(&playerTexture);
				canshoot = true;
			}
			if (framecount > 200)
			{
				iFrame = false;
				iFramehit = false;
				framecount = 0;
			}
			if (playerlife < 0)
			{
				Score = 0;
				playerlife = 3;
				playerboom = 2;
			}
			
			
			Power.ItemsVelocity.y = 3;
			if (Over9000)
			{
				Scorecount = rand() % 1000;
				if (Scorecount >= 900)
				{
					playerboom++;
					playerdamage++;
					playerlife++;
					Scorecount = 0;
				}
			}
			for (size_t j = 0; j < enemys.size(); j++)
			{
				if (enemyshp[j] <= 0)
				{
					soundenemyshoot.stop();
					Power.Itemshape.setPosition(enemys[j].GetPosition());
					ItemMove.push_back(Item(Power));
					enemys[j].bodyenemy.setPosition(-100, -100);
					enemyshp[j] = maxhp;
				}
			}
			for (size_t j = 0; j < enemyr.size(); j++)
			{

				if (enemyrhp[j] <= 0)
				{
					soundenemyshoot.stop();
					Power.Itemshape.setPosition(enemyr[j].GetPosition());
					ItemMove.push_back(Item(Power));
					enemyr[j].bodyenemy.setPosition(-100, -100);
					enemyrhp[j] = maxhp;

				}

			}
			for (size_t j = 0; j < Boss1.size(); j++)
			{
				if (bosshp <= 0)
				{
					soundenemyshoot.stop();
					Boss1[j].bodyenemy.setPosition(-100, -100);
					bosshp = maxbosshp;
					Score += (playerlife * 500);
					Score += (playerboom * 500);
					if (Score >= 9000)
					{
						Over9000 = true;
					}
					Jiren = true;
					GameplayMusic.stop();
					JirenMusic.play();
					randomBoss = 3;
				}
			}
			for (size_t j = 0; j < Boss2.size(); j++)
			{
				if (Jirenhp <= 0)
				{
					Boss2[j].bodyenemy.setPosition(1500, 1500);
					Score += (playerlife * 500);
					Score += (playerboom * 500);
					if (GO == 3)
					{
						CutScene = true;
					}
				}
			}

			for (size_t i = 0; i < ItemMove.size(); i++)
			{
				ItemMove[i].Itemshape.move(ItemMove[i].ItemsVelocity);
				if (ItemMove[i].Itemshape.getPosition().x < 0 || ItemMove[i].Itemshape.getPosition().x > window.getSize().x
					|| ItemMove[i].Itemshape.getPosition().y > window.getSize().y)
				{
					ItemMove.erase(ItemMove.begin() + i);
				}
			}

			for (Item& Enemy : ItemMove)
			{
				if (Enemy.GetCollider().CheckCollision(playernearcol, direction, 0.7f))
				{
					Enemy.Itemshape.setPosition(1500, 900);
					sounditem.play();
					Score += 100;
					playerdamage ++;
				}
			}
			if (End)
			{
				yourname.clear();
				mode = 4;
				End = false;
			}
			

			velocity.y = 5;

			if (bg1.getPosition().y >= window.getSize().y)bg1.setPosition(bg2.getPosition().x, bg2.getPosition().y - 2000.0f);
			if (bg2.getPosition().y >= window.getSize().y)bg2.setPosition(bg1.getPosition().x, bg1.getPosition().y - 2000.0f);
			bg1.move(velocity);
			bg2.move(velocity);


			window.draw(bg1);
			window.draw(bg2);
			player.Draw(window);
			window.draw(playerhit);
			for (size_t i = 0; i < bulletmove.size(); i++)
			{
				window.draw(bulletmove[i].shape);
			}
			for (size_t i = 0; i < enemy01BulletMove.size(); i++)
			{
				window.draw(enemy01BulletMove[i].shape);
			}
			for (size_t i = 0; i < enemy02BulletMove.size(); i++)
			{
				window.draw(enemy02BulletMove[i].shape);
			}
			for (enemy& Enemy : Boss1)
			{
				Enemy.Draw(window);
			}
			for (enemy& Enemy : Boss2)
			{
				Enemy.Draw(window);
			}
			for (enemy& Enemy : enemys)
			{
				Enemy.Draw(window);
			}
			for (enemy& Enemy : enemys2)
			{
				Enemy.Draw(window);
			}
			for (enemy& Enemy : enemyr2)
			{
				Enemy.Draw(window);
			}
			for (enemy& Enemy : enemyr)
			{
				Enemy.Draw(window);
			}
			for (size_t i = 0; i < boommove.size(); i++)
			{
				window.draw(boommove[i].shape);
			}
			for (size_t i = 0; i < ItemMove.size(); i++)
			{
				window.draw(ItemMove[i].Itemshape);
			}
			platform1.Draw(window);
			platform2.Draw(window);

			showLife = playerlife;
			showBoom = playerboom;
			showplayerdmage = playerdamage;

			score.setString(std::to_string(Score));
			ScoreText.setString("Score");
			life.setString(std::to_string(showLife));
			lifeText.setString("Life");
			BOOM.setString(std::to_string(showBoom));
			BoomText.setString("Boom");
			POW.setString(std::to_string(showplayerdmage));
			POWText.setString("POW");
			Howto.setString(L"ควบคุมตัวละคร\n -ขึ้น/W	-LShift:ความเร็วปกติ\n -ลง/S	-LCrtl:ลดความเร็ว\n -ซ้าย/A		-Z/คลิกซ้าย:ยิง\n -ขวา/D		-X/คลิกขวา:ใช้ระเบิด");

			window.draw(score);
			window.draw(ScoreText);
			window.draw(life);
			window.draw(lifeText);
			window.draw(BOOM);
			window.draw(BoomText);
			window.draw(POW);
			window.draw(POWText);
			window.draw(Beam);
			window.draw(TextBox);
			window.draw(CutsceneText);
			window.draw(Playerface);
			window.draw(Bossface);
			window.draw(LifeText);
			window.draw(SpellCardText);
			window.draw(Howto);
			

		}
		else if (mode == 2)
		{
			window.clear();
			mouse = sf::Mouse::getPosition(window);
			Event evnt;
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::Resized:
					ReSizeView(window, view);
					break;
				}
				if (evnt.type == sf::Event::KeyPressed)
					modecheck2 = 1;
					if (evnt.key.code == sf::Keyboard::Escape)
						mode = 0;
					if (evnt.key.code == sf::Keyboard::Return && modecheck2 == 1)
					{
						mode = 0;
						modecheck2 = 0;
					}
			}
			if (modecheck2 == 1)
			{
				Back.setFillColor(Color::Red);
			}
			else
			{
				Back.setFillColor(Color::White);
			}
			Back.setString("Back");
			window.draw(ScoreShape);
			window.draw(Back);
			HighScore(window, font, hs, a, b);
		}
		else if (mode == 3)
		{
			window.close();
		}
		else if (mode == 4)
		{
			window.clear();
			Event evnt;
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::Resized:
					ReSizeView(window, view);
					break;
				}
				if (evnt.type == Event::TextEntered)
				{
					if (evnt.text.unicode == '\b' && yourname.getSize() > 0) //ลบ
					{
						yourname.erase(yourname.getSize() - 1, 1);
						playername.setFont(font);
						playername.setString(yourname);
					}
					else if (evnt.text.unicode == '\b' != yourname.getSize() >= 0)
					{
						std::string name;
						yourname += static_cast<char>(evnt.text.unicode);
						name += static_cast<char>(evnt.text.unicode);
						if ((evnt.text.unicode < 128) && (yourname.getSize() < 10))
						{
							playername.setFont(font);
							playername.setString(yourname);
						}
					}
				}
				else if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::Return)
					{
						mode = 0;
						GameplayMusic.stop();
						Over9000music.stop();
						MenuMusic.play();
						playername.setString(yourname);
						SetHighScore(yourname, Score);
						yourname.clear();
						

					}
			}
			playername.setCharacterSize(40);
			playername.setPosition(window.getSize().x / 2-300, window.getSize().y / 2);
			window.draw(playername);
			window.draw(entername);

		}
		else if (mode == 5)
		{
			Event evnt;
			while (window.pollEvent(evnt))
			{
				switch (evnt.type)
				{
				case Event::Closed:
					window.close();
					break;
				case Event::Resized:
					ReSizeView(window, view);
					break;
				}
				if (evnt.type == sf::Event::KeyPressed)
					if (evnt.key.code == sf::Keyboard::Escape)
					{
						mode = 1;
						if (Over9000)
						{
							Over9000music.play();
						}
						GameplayMusic.play();
					}
			}
			window.draw(PAUSE);
		}
		window.display();
	}

	return 0;
}