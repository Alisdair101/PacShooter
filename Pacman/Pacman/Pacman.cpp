#include "Pacman.h"
#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPowerFrameTime(1000), _cMusicFrameTime(75000), _cPacmanSpeed(0.1f),  _cPacmanFrameTime(250), _cGhostSpeed(0.15f), _cEnemyFrameTime(500), _cGameOverTimer(250), _cLaserFrameTime(10)
{
	srand(time(NULL));

	// Initialising Variables
	VariableInitialisation();

	// Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1600, 720, true, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}
void Pacman::VariableInitialisation()
{
	_frameCount = 0;

	// Score board variable initialisation
	_score = 0;
	_kills = 0;
	_totalShotsFired = 0;

	// Pacman Structure
	_pacman = new Player();

	// Pacman variable initialisation
	_pacman->_currentFrameTime = 0;
	_pacman->_frame = 0;
	_pacman->_sourceRow = 0;
	_pacman->_speedMultiplier = 1;
	_pacman->_dead = false;
	_pacman->_movement = false;

	// Invincibility Power Structure
	_power = new Power();

	// Invincibility Power Variable initialisation
	_power->_currentFrameRate = 0;
	_power->_invincibilityLimit = 0;
	_power->_invincibilityFull = false;
	_power->_invincibilityActive = false;
	_power->_fullReloadActive = false;
	_power->_barAdvice = "0%";
	_power->_powerUpOn = 3;

	// Ghost Structure and variable initialisation
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghost[i] = new Enemy();
		_ghost[i]->_currentFrameTime = 0;
		_ghost[i]->_directionChoice = 3;
		_ghost[i]->_ghostHealth = 3;
		_ghost[i]->_healthBarSourceRow = 1;
		_ghost[i]->_sourceRow = 0;
		_ghost[i]->_speedMultiplier = 1;
		_ghost[i]->_turnTime = rand() % (5000 - 250) + 250;
		_ghost[i]->_dead = false;
		_ghost[i]->_chaseOn = false;
	}

	// Laser Stucture and variable initialisation
	for (int i = 0; i < AMMO; i++)
	{
		_laser[i] = new Weapon();
		_laser[i]->_currentFrameTime = 0;
		_laser[i]->_fire = 1;
	}

	_shotsFired = 0;
	_mouseButtonDown = false;
	_fireGun = false;

	// Ammo Pack Structure and variable initialisation
	for (int i = 0; i < AMMOPACK; i++)
	{
		_ammoPack[i] = new Ammo();
		_ammoPack[i]->_eaten = false;
	}

	// Cherry Structure
	_cherry = new Cherry();

	// Cherry variable initialisation
	_cherry->_frame = 0;
	_cherry->_currentFrameTime = 0;
	_rKeyDown = false;
	_cherry->_moveCherry = false;
	_cherry->_eaten = false;

	// Munchie Structure and variable initialisation
    for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i] = new Munchie();
		_munchie[i]->_currentFrameTime = 0;
		_munchie[i]->_frame = rand() % 1;
		_munchie[i]->_frameTime = rand() % 500 + 50;
		_munchie[i]->_eaten = false;
	}

	// Start Menu Structure and variable initialisation
	for (int i = 0; i < STARTMENU; i++)
	{
		_startMenu[i] = new Start();
		_startMenu[i]->_sourceRow = 1;
		_startMenu[i]->_playBeep = 3;
	}

	_startMenuOn = true;

	// Option Menu Structure and variable initialisation
	for (int i = 0; i < OPTIONMENU; i++)
	{
		_optionMenu[i] = new Option();
		_optionMenu[i]->_sourceRow = 1;
	}

	_optionMenu[3]->_playBeep = 3;
	_optionMenuOn = false;
	_easy = true;
	_medium = false;
	_hard = false;

	_difficulty = 4;

	// Controls Menu Structure and variable initialisation
	_controlMenu = new Control();
	_controlMenu->_sourceRow = 1;

	_controlMenuOn = false;

	// Pause Menu
	_paused = false;
	_pKeyDown = false;

	// Game Over Menu
	_gameOver = false;
	_gameOverFrameTime = 0;

	// Music variable initialisation
	_backMusicTimer = 0;
	_playBackMusic = true;
}

Pacman::~Pacman()
{
	// Pacman delete's
	delete _pacman->_texture;
	delete _pacman->_sourceRect;
	delete _pacman->_position;
	delete _pacman;

	delete _pacman->_pacmanOrigin;
	delete _pacman->_mousePosition;

	// Cherry delete's
	delete _cherry->_texture;
	delete _cherry->_position;
	delete _cherry->_sourceRect;

	// Munchie delete's
	for (int i = 0; i < MUNCHIECOUNT; i++) 
	{ 
		delete _munchie[i]->_texture;
		delete _munchie[i]->_sourceRect;
		delete _munchie[i]->_position;
	}
 
	delete [] _munchie; 

	// Ghost delete's
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghost[i]->_texture;
		delete _ghost[i]->_sourceRect;
		delete _ghost[i]->_position;
		delete _ghost[i]->_collisionArea;
		delete _ghost[i]->_healthBarPosition;
		delete _ghost[i]->_healthBarSourceRect;
		delete _ghost[i]->_chase;
		delete _ghost[i]->_healthBarTexture;
	}

	delete [] _ghost;

	// Laser delete's
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _laser[i]->_texture;
		delete _laser[i]->_sourceRect;
		delete _laser[i]->_position;
		delete _laser[i]->_collisionArea;
		delete _laser[i]->_playerPosition;
	}

	// Ammo delete's
	for (int i = 0; i < AMMOPACK; i++)
	{
		_ammoPack[i]->_position;
		_ammoPack[i]->_sourceRect;
		_ammoPack[i]->_texture;
	}

	// Power delete's
	delete _power->_barPosition;
	delete _power->_barSourceRect;
	delete _power->_barAdvicePosition;
	delete _power->_barTexture;

	// Start Menu delete's
	for (int i = 0; i < STARTMENU; i++)
	{
		delete _startMenu[i]->_position;
		delete _startMenu[i]->_sourceRect;
		delete _startMenu[i]->_texture;
	}

	// Options Menu delete's
	for (int i = 0; i < OPTIONMENU; i++)
	{
		_optionMenu[i]->_position;
		_optionMenu[i]->_sourceRect;
		_optionMenu[i]->_texture;
	}

	// Controls Menu delete's
	delete _controlMenu->_position;
	delete _controlMenu->_sourceRect;
	delete _controlMenu->_texture;

	// Background delete's
	delete _backgroundSourceRect;
	delete _backgroundPos;
	delete _backgroundTex;

	// SoundEffect delete's
	delete _pop;
	delete _pew;
	delete _backMusic;
	delete _reload;
	delete _explode;
	delete _buttonBeep;
	delete _buttonClick;
	delete _bite;
	delete _powerUp;

	// Pause Menu delete's
	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;

	// String delete's
	delete _stringPosition;
}

void Pacman::LoadContent()
{
	// Load Background
	_backgroundTex = new Texture2D();
	_backgroundTex->Load("Textures/Background.png", true);
	// Backgroung image from: http://www.stockfreeimages.com/131776/Space-background-3.html
	_backgroundSourceRect = new Rect(0.0f, 0.0f, 1920, 1080);
	_backgroundPos = new Rect(0, 0, 1920, 1080);

	// Load Pacman
	_pacman->_texture = new Texture2D();
	_pacman->_texture->Load("Textures/Pacman.tga", false);
	_pacman->_pacmanOrigin = new Vector2(16, 16);
	_pacman->_position = new Rect(350.0f, 350.0f, 32, 32);
	_pacman->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Invincibility Bar
	_power->_barTexture = new Texture2D();
	_power->_barTexture->Load("Textures/InvincibilityBar.png", true);
	_power->_barSourceRect = new Rect(0.0f, 0.0f, 204, 25);
	_power->_barPosition = new Rect(50, 950, 204, 25);

	// Load Invincibility Bar Advice
	_power->_barAdvicePosition = new Vector2(145, 946);

	// Load Ghost
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghost[i]->_texture = new Texture2D();
		_ghost[i]->_position = new Vector2((1920 / 2.0) + ((i + 1) * 25), 1080 / 2.0);
		_ghost[i]->_sourceRect = new Rect(0.0f, 0.0f, 20, 20);
		_ghost[i]->_chase = new Rect(_ghost[i]->_position->X, _ghost[i]->_position->Y, 360, 360);
		_ghost[i]->_collisionArea = new Rect(_ghost[i]->_position->X, _ghost[i]->_position->Y, 20, 20);
	}

	// Load Ghost Health Bar
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghost[i]->_healthBarTexture = new Texture2D();
		_ghost[i]->_healthBarTexture->Load("Textures/GhostHealthBar.png", true);
		_ghost[i]->_healthBarSourceRect = new Rect(0.0f, 0.0f, 20, 10);
		_ghost[i]->_healthBarPosition = new Rect(_ghost[i]->_position->X, _ghost[i]->_position->Y - 15, 20, 10);
	}

	_ghost[0]->_texture->Load("Textures/GhostBlue.png", true);
	_ghost[1]->_texture->Load("Textures/GhostGreen.png", true);
	_ghost[2]->_texture->Load("Textures/GhostOrange.png", true);
	_ghost[3]->_texture->Load("Textures/GhostRed.png", true);
	_ghost[4]->_texture->Load("Textures/GhostBlue.png", true);
	_ghost[5]->_texture->Load("Textures/GhostGreen.png", true);
	_ghost[6]->_texture->Load("Textures/GhostOrange.png", true);
	_ghost[7]->_texture->Load("Textures/GhostRed.png", true);
	_ghost[8]->_texture->Load("Textures/GhostBlue.png", true);
	_ghost[9]->_texture->Load("Textures/GhostGreen.png", true);
	_ghost[10]->_texture->Load("Textures/GhostOrange.png", true);
	_ghost[11]->_texture->Load("Textures/GhostRed.png", true);
	
	// Load Cherry
	_cherry->_texture = new Texture2D();
	_cherry->_texture->Load("Textures/Cherries.png", true);
	_cherry->_position = new Rect(rand() % (1720 - 200) + 200, rand() % (880 - 100) + 100, 32, 32);
	_cherry->_sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	Texture2D* _munchieTex = new Texture2D();
	_munchieTex->Load("Textures/MunchiesV1.png", true);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]->_texture = _munchieTex;
		_munchie[i]->_position = new Rect(rand() % (1880 - 40) + 40, rand() % (1040 - 40) + 40, 12, 12);
		_munchie[i]->_sourceRect = new Rect (0.0f, 0.0f, 12, 12);
	}

	// Load Start Menu
	for (int i = 0; i < STARTMENU; i++)
	{
		_startMenu[i]->_texture = new Texture2D();
		_startMenu[i]->_position = new Rect(800, 300 + ((i + 1) * 75), 300, 65);
		_startMenu[i]->_sourceRect = new Rect(0.0f, 0.0f, 300, 65);
	}

	_startMenu[0]->_texture->Load("Textures/StartButton.png", true);
	_startMenu[1]->_texture->Load("Textures/ControlsButton.png", true);
	_startMenu[2]->_texture->Load("Textures/OptionsButton.png", true);
	_startMenu[3]->_texture->Load("Textures/RestartButton.png", true);
	_startMenu[4]->_texture->Load("Textures/QuitButton.png", true);

	// Load Option Menu
	for (int i = 0; i < OPTIONMENU - 1; i++)
	{
		_optionMenu[i]->_texture = new Texture2D();
		_optionMenu[i]->_position = new Rect(740, 360 + ((i + 1) * 75), 300, 65);
		_optionMenu[i]->_sourceRect = new Rect(0.0f, 0.0f, 300, 65);
	}

	_optionMenu[3]->_texture = new Texture2D();
	_optionMenu[3]->_position = new Rect(40, 920, 300, 65);
	_optionMenu[3]->_sourceRect = new Rect(0.0f, 0.0f, 300, 65);

	_optionMenu[0]->_texture->Load("Textures/EasyButton.png", true);
	_optionMenu[1]->_texture->Load("Textures/MediumButton.png", true);
	_optionMenu[2]->_texture->Load("Textures/HardButton.png", true);
	_optionMenu[3]->_texture->Load("Textures/BackButton.png", true);

	// Load Control Menu
	_controlMenu->_texture = new Texture2D();

	_controlMenu->_texture->Load("Textures/Controls.png", true);

	_controlMenu->_position = new Rect(800, 50, 400, 800);
	_controlMenu->_sourceRect = new Rect(0.0f, 0.0f, 400, 800);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, 1920, 1080); 
	_menuStringPosition = new Vector2((1920 / 2.0f), (1080 / 2.0f));
	
	// Load Lasers
	Texture2D* _laserTex = new Texture2D();
	_laserTex->Load("Textures/Laser.png", true);
	
	Vector2* _laserPosition = new Vector2(_pacman->_position->X, _pacman->_position->Y);

	for (int i = 0; i < AMMO; i++)
	{
		_laser[i]->_texture = _laserTex;
		_laser[i]->_sourceRect = new Rect(0.0f, 0.0f, 16, 16);
		_laser[i]->_fire = 1;
		_laser[i]->_collisionArea = new Rect(_laserPosition->X, _laserPosition->Y, 16, 16);
	}

	// Load Ammo Packs
	Texture2D* _ammoTex = new Texture2D();
	_ammoTex->Load("Textures/AmmoPack.png", true);
	
	for (int i = 0; i < AMMOPACK; i++)
	{
		_ammoPack[i]->_texture = _ammoTex;
		_ammoPack[i]->_sourceRect = new Rect(0.0f, 0.0f, 49, 22);
		_ammoPack[i]->_position = new Rect(rand() % (1880 - 40) + 40, rand() % (1040 - 40) + 40, 49, 22);
	}

	// Sound Effects
	// Munchie Pop
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");

	// Laser Pew
	_pew = new SoundEffect();
	_pew->Load("Sounds/laser.wav");
	// Taken from: http://soundbible.com/472-Laser-Blasts.html

	// Main Menu Music
	_backMusic = new SoundEffect();
	_backMusic->Load("Sounds/menuMusic.wav");
	// Taken from: http://www.nosoapradio.us/

	// Reload
	_reload = new SoundEffect();
	_reload->Load("Sounds/reload.wav");
	// Taken from: http://soundbible.com/1996-Shotgun-Reload-Old.html

	// Explode
	_explode = new SoundEffect();
	_explode->Load("Sounds/explode.wav");
	// Taken from: http://soundbible.com/1986-Bomb-Exploding.html

	// Button Beep
	_buttonBeep = new SoundEffect();
	_buttonBeep->Load("Sounds/buttonBeep.wav");
	// Taken from: http://soundbible.com/1540-Computer-Error-Alert.html

	// Button Click
	_buttonClick = new SoundEffect();
	_buttonClick->Load("Sounds/buttonClick.wav");
	// Taken from: http://soundbible.com/1705-Click2.html

	// Eat Cherry
	_bite = new SoundEffect();
	_bite->Load("Sounds/bite.wav");
	// Taken from: http://soundbible.com/950-Bite.html

	// Power Bar Full
	_powerUp = new SoundEffect();
	_powerUp->Load("Sounds/powerUp.wav");
	// Taken from: http://soundbible.com/1636-Power-Up-Ray.html
}
void Pacman::Update(int elapsedTime)
{
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if(_playBackMusic)
	{
		BackMusic();
	}

	LoopMusic(elapsedTime);

	if(_startMenuOn)
	{
		StartMenu(mouseState);
	}

	if(_optionMenuOn || _controlMenuOn || _gameOver)
	{
		OptionMenu(mouseState);
	}

		HandleInput(elapsedTime, mouseState);

	if (!_gameOver && !_startMenuOn && !_paused && !_optionMenuOn && !_controlMenuOn)
	{
		UpdateMouse(mouseState);
		UpdatePacman(elapsedTime);
		UpdateGhosts(elapsedTime);
		UpdateLaser(elapsedTime);
		UpdateCherry(elapsedTime);
		UpdateMunchie(elapsedTime);
		UpdateConsumables(elapsedTime);
		CheckGameOver();
	}
}
void Pacman::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing

	// Draws Background
	SpriteBatch::Draw(_backgroundTex, _backgroundPos, _backgroundSourceRect);

	// Allows us to easily create a string
	std::stringstream _stream;
	_stream << "Pacman X: " << _pacman->_position->X << " Y: " << _pacman->_position->Y;

	// Draws String
	SpriteBatch::DrawString(_stream.str().c_str(), _stringPosition, Color::Green);

	// Draws Invincibility Bar
	SpriteBatch::Draw(_power->_barTexture, _power->_barPosition, _power->_barSourceRect);

	// Draws Invincibility Bar Advice
	std::stringstream _barHelp;
	_barHelp << _power->_barAdvice;
	SpriteBatch::DrawString(_barHelp.str().c_str(), _power->_barAdvicePosition, Color::Red);

	// Draws Player
	if (!_pacman->_dead)
	{
		SpriteBatch::Draw(_pacman->_texture, _pacman->_position, _pacman->_sourceRect, _pacman->_pacmanOrigin, 1.0f, _pacman->_pacmanAngle - 90, Color::White, SpriteEffect::NONE);
	}

	// Draws Enemy
	for (int i = 0; i < _difficulty; i++)
	{
		if (!_ghost[i]->_dead)
		{
			SpriteBatch::Draw(_ghost[i]->_texture, _ghost[i]->_position, _ghost[i]->_sourceRect);
			SpriteBatch::Draw(_ghost[i]->_healthBarTexture, _ghost[i]->_healthBarPosition, _ghost[i]->_healthBarSourceRect);
		}
	}

	// Draws Laser
	for (int i = 0; i < AMMO; i++)
	{
		if (_laser[i]->_fire == 2)
		{
			SpriteBatch::Draw(_laser[i]->_texture, _laser[i]->_position, _laser[i]->_sourceRect);
		}
	}

	// Draws Ammo Packs
	for (int i = 0; i < AMMOPACK; i++)
	{
		if (!_ammoPack[i]->_eaten)
		{
			SpriteBatch::Draw(_ammoPack[i]->_texture, _ammoPack[i]->_position, _ammoPack[i]->_sourceRect);
		}
	}

	// Draws Cherry
	if (!_cherry->_eaten)
	{
		SpriteBatch::Draw(_cherry->_texture, _cherry->_position, _cherry->_sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	// Draws Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (!_munchie[i]->_eaten)
		{
			SpriteBatch::Draw(_munchie[i]->_texture, _munchie[i]->_position, _munchie[i]->_sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		}
	}

	// Start Menu
	if (_startMenuOn)
	{
		for (int i = 0; i < STARTMENU; i++)
		{
		SpriteBatch::Draw(_startMenu[i]->_texture, _startMenu[i]->_position, _startMenu[i]->_sourceRect);
		}
	}

	// Option Menu
	if (_optionMenuOn)
	{
		for (int i = 0; i < OPTIONMENU - 1; i++)
		{
			SpriteBatch::Draw(_optionMenu[i]->_texture, _optionMenu[i]->_position, _optionMenu[i]->_sourceRect);
		}
	}

	if (_optionMenuOn || _controlMenuOn || _gameOver)
	{
		SpriteBatch::Draw(_optionMenu[3]->_texture, _optionMenu[3]->_position, _optionMenu[3]->_sourceRect);
	}

	// Controls Menu
	if (_controlMenuOn)
	{
		SpriteBatch::Draw(_controlMenu->_texture, _controlMenu->_position, _controlMenu->_sourceRect);
	}

	// Pause Menu
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	// Game Over Menu
	if (_gameOver)
	{
		std::stringstream menuStream;
		menuStream << _gameOverNotice << "\n\nScore: " << _score << "\nKills: " << _kills;
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::HandleInput(int elapsedTime, Input::MouseState* mouseState)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Pacman Speed float variable
	float _pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->_speedMultiplier;
		
	if (!_startMenuOn && !_gameOver && !_optionMenuOn && !_controlMenuOn)
	{
			//Pause Menu
			if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
			{
				_pKeyDown = true;
				_paused = !_paused;
			}

			if (keyboardState->IsKeyUp(Input::Keys::P))
			{
				_pKeyDown = false;
			}
		
			if (!_paused)
			{
				// Start Menu On
				if (keyboardState->IsKeyDown(Input::Keys::ESCAPE))
				{
					_startMenuOn = true;
				}
				// Checks if D key is pressed
				if (keyboardState->IsKeyDown(Input::Keys::D))
				{
					_pacman->_position->X += _pacmanSpeed; //Moves Pacman across X axis
					_pacman->_movement = true;
				}
				if (keyboardState->IsKeyDown(Input::Keys::A))
				{
					_pacman->_position->X -= _pacmanSpeed; //Moves Pacman across X axis
					_pacman->_movement = true;
				}
				if (keyboardState->IsKeyDown(Input::Keys::W))
				{
					_pacman->_position->Y -= _pacmanSpeed; //Moves Pacman across X axis
					_pacman->_movement = true;
				}
				if (keyboardState->IsKeyDown(Input::Keys::S))
				{ 
					_pacman->_position->Y += _pacmanSpeed; //Moves Pacman across X axis
					_pacman->_movement = true;
				}

				if (_power->_invincibilityFull = true && keyboardState->IsKeyDown(Input::Keys::F))
				{
					_power->_invincibilityActive = true;
					_power->_invincibilityFull = false;
				}
				else if (_power->_invincibilityFull = true && keyboardState->IsKeyDown(Input::Keys::R))
				{
					_power->_fullReloadActive = true;
				}

				if (mouseState->LeftButton == Input::ButtonState::PRESSED && !_mouseButtonDown)
				{
					_mouseButtonDown = true;
					_fireGun = !_fireGun;
				}

				if (mouseState->LeftButton == Input::ButtonState::RELEASED)
				{
					_mouseButtonDown = false;
				}

				// Fire Laser
				if(_fireGun == true && _shotsFired < 30)
				{
					if (_laser[_shotsFired]->_fire = 1)
					{
						CreateLaser(_laser[_shotsFired]);
						FireLaser(_laser[_shotsFired], mouseState);
						_fireGun = !_fireGun;
					}
				}


				// Cherry Random Position
				if(keyboardState->IsKeyUp(Input::Keys::R))
				{
					_rKeyDown = false;
				}

				if (keyboardState->IsKeyDown(Input::Keys::R) && !_rKeyDown)
				{

					if(!_cherry->_moveCherry)
					{
						_cherry->_moveCherry = true;			
					}

					if (_cherry->_moveCherry)
					{
						int randompositionX = rand() % (1720 - 200) + 200;
						int randompositionY = rand() % (880 - 100) + 100;

						_cherry->_position->X = randompositionX;
						_cherry->_position->Y = randompositionY;
					}

					_rKeyDown = true;
					_cherry->_moveCherry = !_cherry->_moveCherry;

				}

				if (keyboardState->IsKeyUp(Input::Keys::R))
				{
					_rKeyDown = false;
				}

				//Pacman Speed Multiplier
				if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
				{
					_pacman->_speedMultiplier = 2;
				}
				else
				{
					_pacman->_speedMultiplier = 1;
				}

				if (!_startMenuOn)
				{
					if (!_paused)
					{
							_frameCount++;
					}
				}
		}
	}
}
void Pacman::PacmanAnimation(int elapsedTime)
{
		
	//Changes the texture of Pacman to fit with the direction he is moving in

		_pacman->_currentFrameTime += elapsedTime;
		_pacman->_sourceRect->Y = _pacman->_sourceRect->Height * _pacman->_sourceRow;

		if (_pacman->_currentFrameTime > _cPacmanFrameTime)
		{
			_pacman->_frame++;

			if (_pacman->_frame >= 2)
			{
				_pacman->_frame = 0;
			}
		
			_pacman->_currentFrameTime = 0;

		}

		_pacman->_sourceRect->X = _pacman->_sourceRect->Width * _pacman->_frame;
		_pacman->_movement = false;
}
void Pacman::PacmanWall()
{
		if (_pacman->_position->X > 1920)
		{
			_pacman->_position->X = 0 - _pacman->_sourceRect->Width;
		}

		if (_pacman->_position->X + _pacman->_sourceRect->Width < 0)
		{
			_pacman->_position->X = 1920;
		}

		if (_pacman->_position->Y + _pacman->_sourceRect->Width < 0)
		{
			_pacman->_position->Y = 1080;
		}

		if (_pacman->_position->Y > 1080)
		{
			_pacman->_position->Y = 0 - _pacman->_sourceRect->Height;
		}	
}
void Pacman::UpdateMouse(Input::MouseState* mouseState)
{
	_pacman->_pacmanAngle = atan2(mouseState->X - _pacman->_position->X, mouseState->Y - _pacman->_position->Y);
	_pacman->_pacmanAngle = _pacman->_pacmanAngle * (180/M_PI);
}
void Pacman::UpdatePacman(int elapsedTime)
{
	if (_pacman->_movement == true)
	{
		PacmanAnimation(elapsedTime);
	}

	PacmanWall();
}

void Pacman::UpdateAmmoPack()
{
	for (int i = 0; i < AMMOPACK; i++)
	{
		if (!_ammoPack[i]->_eaten)
		{
			if (_ammoPack[i]->_position->Intersects(*_pacman->_position))
			{
				Audio::Play(_reload);
				_shotsFired = 0;
				_ammoPack[i]->_eaten = true;
			}
		}
	}
}
void Pacman::UpdateInvincibility(int elapsedTime)
{
	if (_power->_invincibilityActive || _power->_fullReloadActive)
	{
		InvincibilityPower(elapsedTime);
	}

	InvincibilityAdvice();
	InvincibilityBar();
}
void Pacman::InvincibilityBar()
{
	if (_power->_invincibilityLimit > 10)
	{
		_power->_invincibilityLimit = 10;
	}

	_power->_barSourceRect->X = _power->_barSourceRect->Width * _power->_invincibilityLimit;

	if (_power->_invincibilityLimit >= 10)
	{
		_power->_invincibilityFull = true;
		
		if (_power->_powerUpOn == 3)
		{
			_power->_powerUpOn = 1;
		}
	}

	if (_power->_powerUpOn == 1)
	{
		Audio::Play(_powerUp);
		_power->_powerUpOn = 2;
	}

	if (_power->_invincibilityLimit == 0 && _power->_powerUpOn == 2)
	{
		_power->_powerUpOn = 1;
	}
}
void Pacman::InvincibilityAdvice()
{
	switch (_power->_invincibilityLimit)
	{
	default: _power->_barAdvice = "0%";
	case 0: _power->_barAdvice = "0%";
		break;
	case 1: _power->_barAdvice = "10%";
		break;
	case 2: _power->_barAdvice = "20%";
		break;
	case 3: _power->_barAdvice = "30%";
		break;
	case 4: _power->_barAdvice = "40%";
		break;
	case 5: _power->_barAdvice = "50%";
		break;
	case 6: _power->_barAdvice = "60%";
		break;
	case 7: _power->_barAdvice = "70%";
		break;
	case 8: _power->_barAdvice = "80%";
		break;
	case 9: _power->_barAdvice = "90%";
		break;
	case 10: _power->_barAdvice = "100% Press F or R!";
		break;
	}

	if(_power->_invincibilityActive)
	{
		_power->_barAdvice = "INVINCIBLE!";
	}
	else if (_power->_fullReloadActive)
	{
		_power->_barAdvice = "RELOAD!";
	}
}
void Pacman::InvincibilityPower(int elapsedTime)
{
	_power->_currentFrameRate += elapsedTime;

	if (_power->_currentFrameRate > _cPowerFrameTime)
	{
		_power->_invincibilityLimit--;

		if (_power->_invincibilityLimit == 0 && _power->_invincibilityActive)
		{
			_power->_invincibilityActive = false;
		}
		if (_power->_invincibilityLimit == 0 && _power->_fullReloadActive)
		{
			_shotsFired = 0;
			_power->_fullReloadActive = false;
		}

		_power->_currentFrameRate = 0;
	}
}
void Pacman::UpdateConsumables(int elapsedTime)
{
	UpdateAmmoPack();
	UpdateInvincibility(elapsedTime);
}

void Pacman::CreateLaser(Weapon* _laser)
{
	Vector2* _laserPosition = new Vector2(_pacman->_position->X, _pacman->_position->Y);
	_laser->_position = _laserPosition;
}
void Pacman::FireLaser(Weapon* _laser, Input::MouseState* mouseState)
{
	Vector2 _direction(Vector2(_pacman->_position->X, _pacman->_position->Y) - Vector2(mouseState->X, mouseState->Y));
	_direction.Normalize();
	_laser->_direction = _direction;
	_laser->_fire = 2;
	_shotsFired++;
	_totalShotsFired += 1;
	_score--;
	Audio::Play(_pew);
}
void Pacman::UpdateLaserPosition(int elapsedTime)
{
	for (int i = 0; i < AMMO; i++)
	{
		if(_laser[i]->_fire == 2)
		{
				_laser[i]->_currentFrameTime += elapsedTime;
				_laser[i]->_collisionArea->X = _laser[i]->_position->X;
				_laser[i]->_collisionArea->Y = _laser[i]->_position->Y;

				if (_laser[i]->_currentFrameTime > _cLaserFrameTime)
				{
					*_laser[i]->_position -= _laser[i]->_direction * 1.0f  * elapsedTime;
					_laser[i]->_currentFrameTime = 0;
				}
		}
		
	}
}
void Pacman::CheckLaserCollisions(Weapon* _laser)
{
	for (int i = 0; i < _difficulty; i++)
	{
		if (_laser->_fire == 2)
		{
			_ghost[i]->_collisionArea->X = _ghost[i]->_position->X;
			_ghost[i]->_collisionArea->Y = _ghost[i]->_position->Y;

			if (!_ghost[i]->_dead)
			{
				if (_laser->_collisionArea->Intersects(*_ghost[i]->_collisionArea))
				{
					_ghost[i]->_ghostHealth--;
					_laser->_fire = 3;
				}
			}
		}
	}
}
void Pacman::LaserWall(Weapon* _laser)
{
	if (_laser->_collisionArea->X > 1920
		|| _laser->_collisionArea->X < -16
		|| _laser->_collisionArea->Y < 0
		|| _laser->_collisionArea->Y > 1080)
	{
		_laser->_fire = 3;
	}
}
void Pacman::ResetLaser(Weapon* _laser)
{
	Rect* _laserRect = new Rect(0.0f, 0.0f, 16, 16);
	Vector2* _laserPosition = new Vector2(_pacman->_position->X, _pacman->_position->Y);

	_laser->_currentFrameTime = 0;
	_laser->_sourceRect = _laserRect;
	_laser->_fire = 1;
	_laser->_collisionArea = new Rect(_laserPosition->X, _laserPosition->Y, 16, 16);
}
void Pacman::UpdateLaser(int elapsedTime)
{
	UpdateLaserPosition(elapsedTime);

	for (int i = 0; i < AMMO; i++)
	{
		CheckLaserCollisions(_laser[i]);
		LaserWall(_laser[i]);

		if (_laser[i]->_fire == 3)
		{
			ResetLaser(_laser[i]);
		}
	}
}

void Pacman::GhostMovement(Enemy* _ghost, int elapsedTime)
{
	float _ghostSpeed = _cGhostSpeed * elapsedTime * _ghost->_speedMultiplier;
	_ghost->_sourceRect->X = _ghost-> _sourceRect->Width * _ghost->_sourceRow;
	_ghost->_chase->X = _ghost->_position->X;
	_ghost->_chase->Y = _ghost->_position->Y;

		if (_ghost->_directionChoice == 1)
		{
			_ghost->_position->X -= _ghostSpeed;
			_ghost->_sourceRow = 1;
		}
		else if (_ghost->_directionChoice == 2)
		{
			_ghost->_position->X += _ghostSpeed;
			_ghost->_sourceRow = 4;
		}
		else if (_ghost->_directionChoice == 3)
		{
			_ghost->_position->Y -= _ghostSpeed;
			_ghost->_sourceRow = 2;
		}
		else if (_ghost->_directionChoice == 4)
		{
			_ghost->_position->Y += _ghostSpeed;
			_ghost->_sourceRow = 3;
		}
	}
void Pacman::CheckChase(Enemy* _ghost)
{
	_ghost->_chase->X = _ghost->_position->X;
	_ghost->_chase->Y = _ghost->_position->Y;

	if(_ghost->_chase->Intersects(*_pacman->_position))
	{
		_ghost->_chaseOn = true;
	}
	else
	{
		_ghost->_chaseOn = false;
	}
}
void Pacman::GhostChase(Enemy* _ghost)
{
	Vector2 _direction(Vector2(_pacman->_position->X, _pacman->_position->Y) - Vector2(_ghost->_position->X, _ghost->_position->Y));
	_direction.Normalize();
	_ghost->_direction = _direction;
}
void Pacman::UpdateGhostChase(Enemy* _ghost, int elapsedTime)
{
	_ghost->_currentFrameTime += elapsedTime;
	float _ghostSpeed = _cGhostSpeed * elapsedTime * _ghost->_speedMultiplier;
	_ghost->_chase->X = _ghost->_position->X;
	_ghost->_chase->Y = _ghost->_position->Y;

	if (_ghost->_currentFrameTime > _cLaserFrameTime)
	{
		*_ghost->_position += _ghost->_direction * _ghostSpeed;
		_ghost->_currentFrameTime = 0;
	}
}
void Pacman::GhostPatrol(Enemy* _ghost, int elapsedTime)
{
	_ghost->_currentFrameTime += elapsedTime;

	if (_ghost->_currentFrameTime > _ghost->_turnTime)
	{
		_ghost->_directionChoice = rand() % 4 + 1;
		_ghost->_currentFrameTime = 0;
		_ghost->_turnTime = rand() % (5000 - 250) + 250;
	}
}
void Pacman::CheckGhostCollisions(Enemy* _ghost)
{
	if (!_ghost->_dead)
	{
		_ghost->_collisionArea->X = _ghost->_position->X;
		_ghost->_collisionArea->Y = _ghost->_position->Y;

		if (_ghost->_collisionArea->Intersects(*_pacman->_position))
		{
			_gameOverNotice = "PACMAN IS DEAD! GAME OVER!";
			_pacman->_dead = true;
			_gameOver = true;
		}
	}
}
void Pacman::GhostWall(Enemy* _ghost)
{
	if (_ghost->_position->X > 1920)
		{
			_ghost->_position->X = 0 - _ghost->_sourceRect->Width;
		}

		if (_ghost->_position->X + _ghost->_sourceRect->Width < 0)
		{
			_ghost->_position->X = 1920;
		}

		if (_ghost->_position->Y + _ghost->_sourceRect->Width < 0)
		{
			_ghost->_position->Y = 1080;
		}

		if (_ghost->_position->Y > 1080)
		{
			_ghost->_position->Y = 0 - _ghost->_sourceRect->Height;
		}	
}
void Pacman::GhostBuff()
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghost[i]->_speedMultiplier += 0.1;
	}
}
void Pacman::GhostHealthBar(Enemy* _ghost)
{
	_ghost->_healthBarSourceRect->X = _ghost-> _healthBarSourceRect->Width * _ghost->_healthBarSourceRow;
	_ghost->_healthBarPosition->X = _ghost->_position->X;
	_ghost->_healthBarPosition->Y = _ghost->_position->Y - 15;

	if (_ghost->_ghostHealth == 3)
	{
		_ghost->_healthBarSourceRow = 0;
	}

	if (_ghost->_ghostHealth == 2)
	{
		_ghost->_healthBarSourceRow = 1;
	}

	if (_ghost->_ghostHealth == 1)
	{
		_ghost->_healthBarSourceRow = 2;
	}

	if (_ghost->_ghostHealth == 0)
	{
		Audio::Play(_explode);
		_ghost->_dead = true;
		GhostBuff();
		_ghost->_ghostHealth = -1;
		_kills += 1;
		_score += 25;
	}
}
void Pacman::UpdateGhosts(int elapsedTime)
{

	for (int i = 0; i < _difficulty; i++)
	{
		CheckChase(_ghost[i]);

		if (!_ghost[i]->_chaseOn)
		{
			GhostMovement(_ghost[i], elapsedTime);
			GhostPatrol(_ghost[i], elapsedTime);
		}
		else if (_ghost[i]->_chaseOn)
		{
			GhostChase(_ghost[i]);
			UpdateGhostChase(_ghost[i], elapsedTime);
		}

		if (!_power->_invincibilityActive)
		{
			CheckGhostCollisions(_ghost[i]);
		}

		GhostWall(_ghost[i]);
		GhostHealthBar(_ghost[i]);
	}
}

void Pacman::CherryAnimation(int elapsedTime)
{
	_cherry->_currentFrameTime += elapsedTime;

	if (_cherry->_currentFrameTime > _cEnemyFrameTime)
	{
		_cherry->_frame++;

		if (_cherry->_frame >= 2) 
		{
			_cherry->_frame = 0;
		}
		
		_cherry->_currentFrameTime = 0;

	}

	_cherry->_sourceRect->X = _cherry->_sourceRect->Width * _cherry->_frame;
}
void Pacman::CheckCherryCollisions()
{
		if (!_cherry->_eaten)
		{
			if (_cherry->_position->Intersects(*_pacman->_position))
			{
				Audio::Play(_bite);
				_cherry->_eaten = true;
				_power->_invincibilityLimit = 10;
			}
		}
}
void Pacman::UpdateCherry(int elapsedTime)
{
	CherryAnimation(elapsedTime);
	CheckCherryCollisions();
}

void Pacman::CheckMunchieCollisions(Munchie* _munchie, int _shotsFired)
{
		if (!_munchie->_eaten)
		{
			if (_munchie->_position->Intersects(*_pacman->_position))
			{
				_munchie->_eaten = true;
				_score += 5;
				_power->_invincibilityLimit++;
				Audio::Play(_pop);
			}
		}
}
void Pacman::MunchieAnimation(Munchie* _munchie, int elapsedTime)
{
	_munchie->_currentFrameTime += elapsedTime;

		if (_munchie->_currentFrameTime > _munchie->_frameTime)
		{
			_munchie->_frame++;

			if (_munchie->_frame >= 2)
			{
				_munchie->_frame = 0;
			}
		
			_munchie->_currentFrameTime = 0;

		}

	_munchie->_sourceRect->X = _munchie->_sourceRect->Width * _munchie->_frame;
}
void Pacman::UpdateMunchie(int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		MunchieAnimation(_munchie[i], elapsedTime);
		CheckMunchieCollisions(_munchie[i], _shotsFired);
	}
}

void Pacman::StartMenu(Input::MouseState* mouseState)
{
	_pacman->_mouseRect = new Rect(mouseState->X, mouseState->Y, 5, 5);

	for (int i = 0; i < STARTMENU; i++)
	{
		_startMenu[i]->_sourceRect->Y = _startMenu[i]->_sourceRect->Height * _startMenu[i]->_sourceRow;

		if (_pacman->_mouseRect->Intersects(*_startMenu[i]->_position))
		{
			_startMenu[i]->_sourceRow = 2;
			
			if (_startMenu[i]->_playBeep == 3)
			{
				_startMenu[i]->_playBeep = 1;
			}

			if (_startMenu[i]->_playBeep == 1)
			{
				Audio::Play(_buttonBeep);
				_startMenu[i]->_playBeep = 2;
			}
		}
		else
		{
			_startMenu[i]->_playBeep = 3;
			_startMenu[i]->_sourceRow = 1;
			_startMenu[i]->_sourceRow = 1;
		}
	}

	if (_pacman->_mouseRect->Intersects(*_startMenu[0]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _startMenuOn)
	{
		Audio::Play(_buttonClick);
		_startMenuOn = false;
	}

	if (_pacman->_mouseRect->Intersects(*_startMenu[1]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _startMenuOn)
	{
		Audio::Play(_buttonClick);
		_startMenuOn = false;
		_controlMenuOn = true;
	}

	if (_pacman->_mouseRect->Intersects(*_startMenu[2]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _startMenuOn)
	{
		Audio::Play(_buttonClick);
		_startMenuOn = false;
		_optionMenuOn = true;
	}

	if (_pacman->_mouseRect->Intersects(*_startMenu[3]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _startMenuOn)
	{
		Audio::Play(_buttonClick);
		Restart();
		LoadContent();
	}

	if (_pacman->_mouseRect->Intersects(*_startMenu[4]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _startMenuOn)
	{
		Audio::Play(_buttonClick);
		exit(0);
	}
}
void Pacman::OptionMenu(Input::MouseState* mouseState)
{
	_pacman->_mouseRect = new Rect(mouseState->X, mouseState->Y, 5, 5);

	if (_optionMenuOn)
	{
		for (int i = 0; i < OPTIONMENU - 1; i++)
		{
			_optionMenu[i]->_sourceRect->Y = _optionMenu[i]->_sourceRect->Height * _optionMenu[i]->_sourceRow;
		}

		if (_pacman->_mouseRect->Intersects(*_optionMenu[0]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _optionMenuOn)
		{

			Audio::Play(_buttonClick);

			_easy = true;
			_medium = false;
			_hard = false;

			_optionMenu[0]->_sourceRow = 2;
			_optionMenu[1]->_sourceRow = 1;
			_optionMenu[2]->_sourceRow = 1;

			_difficulty = 4;
			_scoreMultiplier = 1;
		}

		if (_pacman->_mouseRect->Intersects(*_optionMenu[1]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _optionMenuOn)
		{
			Audio::Play(_buttonClick);

			_easy = false;
			_medium = true;
			_hard = false;

			_optionMenu[0]->_sourceRow = 1;
			_optionMenu[1]->_sourceRow = 2;
			_optionMenu[2]->_sourceRow = 1;

			_difficulty = 8;
			_scoreMultiplier = 2;
		}

		if (_pacman->_mouseRect->Intersects(*_optionMenu[2]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED && _optionMenuOn)
		{
			Audio::Play(_buttonClick);

			_easy = false;
			_medium = false;
			_hard = true;

			_optionMenu[0]->_sourceRow = 1;
			_optionMenu[1]->_sourceRow = 1;
			_optionMenu[2]->_sourceRow = 2;

			_difficulty = 12;
			_scoreMultiplier = 3;
		}
	}

	_optionMenu[3]->_sourceRect->Y = _optionMenu[3]->_sourceRect->Height * _optionMenu[3]->_sourceRow;

	if (_pacman->_mouseRect->Intersects(*_optionMenu[3]->_position))
	{
		_optionMenu[3]->_sourceRow = 2;

		if (_optionMenu[3]->_playBeep == 3)
		{
			_optionMenu[3]->_playBeep = 1;
		}

		if (_optionMenu[3]->_playBeep == 1)
		{
			Audio::Play(_buttonBeep);
			_optionMenu[3]->_playBeep = 2;
		}
	}
	else
	{
		_optionMenu[3]->_playBeep = 3;
		_optionMenu[3]->_sourceRow = 1;
	}

	if (_pacman->_mouseRect->Intersects(*_optionMenu[3]->_position) && mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		if (_optionMenuOn || _gameOver || _controlMenuOn)
		{
			Audio::Play(_buttonClick);

			_controlMenuOn = false;
			_gameOver = false;
			_optionMenuOn = false;
			_startMenuOn = true;
		}
	}
}
void Pacman::CheckGameOver()
{
	if (_difficulty == 4)
	{
		if (_ghost[0]->_dead && _ghost[1]->_dead && _ghost[2]->_dead && _ghost[3]->_dead)
		{
			_gameOverNotice = "VICTORY!!!";
			_gameOver = true;
		}
	}

	else if (_difficulty == 8)
	{
		if (_ghost[0]->_dead && _ghost[1]->_dead && _ghost[2]->_dead && _ghost[3]->_dead && _ghost[4]->_dead && _ghost[5]->_dead && _ghost[6]->_dead && _ghost[7]->_dead)
		{
			_gameOverNotice = "VICTORY!!!";
			_gameOver = true;
		}
	}

	else if (_difficulty == 12)
	{
		if (_ghost[0]->_dead && _ghost[1]->_dead && _ghost[2]->_dead && _ghost[3]->_dead && _ghost[4]->_dead && _ghost[5]->_dead && _ghost[6]->_dead && _ghost[7]->_dead && _ghost[8]->_dead && _ghost[9]->_dead && _ghost[10]->_dead && _ghost[11]->_dead)
		{
			_gameOverNotice = "VICTORY!!!";
			_gameOver = true;
		}
	}
}
void Pacman::BackMusic()
{
	Audio::Play(_backMusic);
	_playBackMusic = false;
}
void Pacman::LoopMusic(int elapsedTime)
{
	_backMusicTimer += elapsedTime;

	if (_backMusicTimer > _cMusicFrameTime)
		{
			_playBackMusic = true;
			_backMusicTimer = 0;
		}
}
void Pacman::Restart()
{
	_frameCount = 0;

	// Score board variable initialisation
	_score = 0;
	_kills = 0;
	_totalShotsFired = 0;

	// Pacman variable initialisation
	_pacman->_currentFrameTime = 0;
	_pacman->_frame = 0;
	_pacman->_sourceRow = 0;
	_pacman->_speedMultiplier = 1;
	_pacman->_dead = false;
	_pacman->_movement = false;

	// Invincibility Power Variable initialisation
	_power->_currentFrameRate = 0;
	_power->_invincibilityLimit = 0;
	_power->_invincibilityFull = false;
	_power->_invincibilityActive = false;
	_power->_fullReloadActive = false;
	_power->_barAdvice = "0%";

	// Ghost Structure and variable initialisation
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghost[i] = new Enemy();
		_ghost[i]->_currentFrameTime = 0;
		_ghost[i]->_directionChoice = 3;
		_ghost[i]->_ghostHealth = 3;
		_ghost[i]->_healthBarSourceRow = 1;
		_ghost[i]->_sourceRow = 0;
		_ghost[i]->_speedMultiplier = 1;
		_ghost[i]->_turnTime = rand() % (5000 - 250) + 250;
		_ghost[i]->_dead = false;
		_ghost[i]->_chaseOn = false;
	}

	// Laser Stucture and variable initialisation
	for (int i = 0; i < AMMO; i++)
	{
		_laser[i]->_currentFrameTime = 0;
		_laser[i]->_fire = 1;
	}

	_shotsFired = 0;
	_mouseButtonDown = false;
	_fireGun = false;

	// Ammo Pack Structure and variable initialisation
	for (int i = 0; i < AMMOPACK; i++)
	{
		_ammoPack[i]->_eaten = false;
	}

	// Cherry variable initialisation
	_cherry->_frame = 0;
	_cherry->_currentFrameTime = 0;
	_rKeyDown = false;
	_cherry->_moveCherry = false;
	_cherry->_eaten = false;

	// Munchie Structure and variable initialisation
    for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchie[i]->_currentFrameTime = 0;
		_munchie[i]->_frame = rand() % 1;
		_munchie[i]->_frameTime = rand() % 500 + 50;
		_munchie[i]->_eaten = false;
	}

	// Start Menu Structure and variable initialisation
	for (int i = 0; i < STARTMENU; i++)
	{
		_startMenu[i]->_sourceRow = 1;
	}

	_startMenuOn = true;

	// Option Menu Structure and variable initialisation
	for (int i = 0; i < OPTIONMENU; i++)
	{
		_optionMenu[i]->_sourceRow = 1;
	}

	_optionMenuOn = false;
	_easy = true;
	_medium = false;
	_hard = false;

	_difficulty = 4;

	// Controls Menu Structure and variable initialisation
	_controlMenu->_sourceRow = 1;

	_controlMenuOn = false;

	// Pause Menu
	_paused = false;
	_pKeyDown = false;

	// Game Over Menu
	_gameOver = false;
	_gameOverFrameTime = 0;
}