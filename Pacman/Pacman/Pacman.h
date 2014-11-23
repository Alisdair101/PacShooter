#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

// Array Size declarations
#define AMMOPACK 5
#define AMMO 30
#define STARTMENU 5
#define OPTIONMENU 4
#define GHOSTCOUNT 12
#define MUNCHIECOUNT 50

// Math Values
#define M_PI       3.14159265358979323846

// Player Structure
struct Player
{
	Rect* _position;
	Rect* _sourceRect;

	Rect* _mouseRect;

	const Vector2* _pacmanOrigin;
	Vector2* _mousePosition;

	Texture2D* _texture;

	int _currentFrameTime;
	int _frame;
	int _sourceRow;

	float _pacmanAngle;
	float _speedMultiplier;

	bool _movement;
	bool _dead;
};

// Enemy Structure
struct Enemy 
{ 
	Rect* _sourceRect;
	Rect* _healthBarPosition;
	Rect* _healthBarSourceRect;
	Rect* _chase;
	Rect* _collisionArea;

	Vector2* _position;
	Vector2 _direction;

	Texture2D* _texture;
	Texture2D* _healthBarTexture;

	int _directionChoice;
	int _currentFrameTime;
	int _sourceRow;
	int _turnTime;

	int _ghostHealth;
	int _healthBarSourceRow;

	float _speedMultiplier;

	bool _dead;
	bool _chaseOn;
}; 

// Laser Structure
struct Weapon
{
	Rect* _sourceRect;
	Rect* _collisionArea;

	Texture2D* _texture;

	Vector2* _playerPosition;
	Vector2* _position;

	Vector2 _direction;

	int _currentFrameTime;

	int _fire;
};

// Enemy Structure
struct Cherry
{
	Rect* _position;
	Rect* _sourceRect;

	Texture2D* _texture;

	int _currentFrameTime;
	int _frame;

	bool _moveCherry;
	bool _eaten;
};

// Munchie Structure
struct Munchie
{
	Rect* _position;
	Rect* _sourceRect;

	Texture2D* _texture;

	int _currentFrameTime;
	int _frame;
	int _frameTime;
	
	bool _eaten;
};

// Ammo Pack Structure
struct Ammo
{
	Rect* _position;
	Rect* _sourceRect;

	Texture2D* _texture;

	bool _eaten;
};

// Invincibility Power Structure
struct Power
{
	Rect* _barPosition;
	Rect* _barSourceRect;

	Vector2* _barAdvicePosition;

	Texture2D* _barTexture;

	int _invincibilityLimit;

	std::string _barAdvice;

	int _currentFrameRate;
	int _powerUpOn;

	bool _invincibilityFull;
	bool _invincibilityActive;
	bool _fullReloadActive;
};

// Start Menu Structure
struct Start
{
	Rect* _position;
	Rect* _sourceRect;

	Texture2D* _texture;

	int _sourceRow;

	int _playBeep;
};

// Options Menu Structure
struct Option
{
	Rect* _position;
	Rect* _sourceRect;

	Texture2D* _texture;

	int _sourceRow;

	int _playBeep;
};

// Controls Menu Structure
struct Control
{
	Rect* _position;
	Rect* _sourceRect;

	Texture2D* _texture;

	int _sourceRow;
};

class Pacman : public Game
{
private:
	// System variables
	int _frameCount;

	// Background
	Rect* _backgroundSourceRect;
	Rect* _backgroundPos;

	Texture2D* _backgroundTex;

	// Sound Effects
	SoundEffect* _pop;
	SoundEffect* _pew;
	SoundEffect* _backMusic;
	SoundEffect* _reload;
	SoundEffect* _explode;
	SoundEffect* _buttonBeep;
	SoundEffect* _buttonClick;
	SoundEffect* _bite;
	SoundEffect* _powerUp;

	// Score board statistics
	int _score;
	int _scoreMultiplier;
	int _kills;
	int _totalShotsFired;
	std::string _gameOverNotice;

	// Player Constants
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;

	// MUsic Constant
	const int _cMusicFrameTime;

	// Ghost Constants
	const float _cGhostSpeed;

	// Laser Constants
	const int _cLaserFrameTime;

	// Laser Variables
	int _ammo;
	int _shotsFired;
	bool _fireGun;

	// Power Constant
	const int _cPowerFrameTime;

	// Data to represent Cherry
	const int _cEnemyFrameTime;

	//Structure pointers
	Player* _pacman;
	Enemy* _ghost[GHOSTCOUNT];
	Cherry* _cherry;
	Munchie* _munchie[MUNCHIECOUNT];
	Start* _startMenu[STARTMENU];
	Weapon* _laser[AMMO];
	Ammo* _ammoPack[AMMOPACK];
	Option* _optionMenu[OPTIONMENU];
	Power* _power;
	Control* _controlMenu;

	// Pause Menu Variables
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;

	// Start Menu
	bool _startMenuOn;
	

	// Option Menu
	bool _optionMenuOn;
	bool _easy;
	bool _medium;
	bool _hard;

	int _difficulty;

	// Control Menu
	bool _controlMenuOn;

	// Pause Menu
	bool _paused;

	// Game Over Menu
	bool _gameOver;

	int _gameOverFrameTime;
	const int _cGameOverTimer;

	// Input Booleans
	bool _pKeyDown;
	bool _sKeydown;
	bool _rKeyDown;
	bool _mouseButtonDown;

	// Position for String
	Vector2* _stringPosition;

	// Background Music Variables
	int _backMusicTimer;
	bool _playBackMusic;

	// Methods
	void VariableInitialisation();

	void HandleInput(int elapsedTime, Input::MouseState* mouseState);
	void PacmanAnimation(int elapsedTime);
	void PacmanWall();
	void UpdateMouse(Input::MouseState* mouseState);
	void UpdatePacman(int elapsedTime);

	void UpdateAmmoPack();
	void UpdateInvincibility(int elapsedTime);
	void InvincibilityBar();
	void InvincibilityAdvice();
	void InvincibilityPower(int elapsedTime);
	void UpdateConsumables(int elapsedTime);

	void CreateLaser(Weapon* _laser);
	void FireLaser(Weapon* _laser, Input::MouseState* mouseState);
	void CheckLaserCollisions(Weapon* _laser);
	void UpdateLaserPosition(int elapsedTime);
	void LaserWall(Weapon* _laser);
	void ResetLaser(Weapon* _laser);
	void UpdateLaser(int elapsedTime);

	void GhostMovement(Enemy* _ghost, int elapsedTime);
	void CheckChase(Enemy* _ghost);
	void GhostChase(Enemy* _ghost);
	void UpdateGhostChase(Enemy* _ghost, int elapsedTime);
	void GhostPatrol(Enemy* _ghost, int elapsedTime);
	void CheckGhostCollisions(Enemy* _ghost);
	void GhostWall(Enemy* _ghost);
	void GhostBuff();
	void GhostHealthBar(Enemy* _ghost);
	void UpdateGhosts(int elapsedTime);

	void CherryAnimation(int elapsedTime);
	void CheckCherryCollisions();
	void UpdateCherry(int elapsedTime);

	void CheckMunchieCollisions(Munchie* _munchie, int _shotsFired);
	void MunchieAnimation(Munchie* _munchie, int elapsedTime);
	void UpdateMunchie(int elapsedTime);

	void StartMenu(Input::MouseState* mouseState);	
	void OptionMenu(Input::MouseState* mouseState);
	void CheckGameOver();
	void BackMusic();
	void LoopMusic(int elapsedTime);
	void Restart();

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};