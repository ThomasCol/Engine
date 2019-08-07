#ifndef _DEFINE_H_
#define _DEFINE_H_

/* Camera data */
#define NEAR 0.1
#define FAR 10000
#define FOV 70

/* GUN */
#define GUN "resources/media/Sprites/Gun"
#define HUDGUN "resources/media/HUD/gun"
#define AMMOGUN 30
#define GUN_AMMO_USED 1
#define GUN_DMG 10
#define GUN_IMPACT_SIZE 1

/* SHOTGUN */
#define SHOTGUN "resources/media/Sprites/Shotgun"
#define HUDSHOTGUN "resources/media/HUD/shotgun"
#define AMMOSHOTGUN 25
#define SHOTGUN_AMMO_USED 1
#define SHOTGUN_DMG 30
#define SHOTGUN_IMPACT_SIZE 4

/* RIFLE */
#define RIFLE "resources/media/Sprites/Rifle"
#define HUDRIFLE "resources/media/HUD/rifle"
#define AMMORIFLE 100
#define RIFLE_AMMO_USED 3
#define RIFLE_DMG 15
#define RIFLE_IMPACT_SIZE 1

/* ROCKET */
#define ROCKET "resources/media/Sprites/Rocket"
#define HUDROCKET "resources/media/HUD/rocket"
#define AMMOROCKET 50
#define ROCKET_AMMO_USED 2
#define ROCKET_DMG 50
#define ROCKET_IMPACT_SIZE 6

/* RPG */
#define RPG "resources/media/Sprites/RPG"
#define HUDRPG "resources/media/HUD/rpg"
#define AMMORPG 10
#define RPG_AMMO_USED 1
#define RPG_DMG 75
#define RPG_IMPACT_SIZE 10

/* HEALTH */
#define HEALTH "resources/media/HUD/Health"

/* END LEVEL */
#define ENDL "resources/media/Level/EndTW"
#define ENDL2 "resources/media/Level/EndW"
#define ENDP "resources/media/Sprites/EndPunch"

/* LEVEL TEXTURES */
#define LEVEL1 "resources/map/map2"
#define LEVEL2 "resources/map/map3"
#define WALLS "resources/media/Level/Wall1.png"
#define WALLS2 "resources/media/Level/Wall2.png"
#define WALLTAPS1 "resources/media/Level/Walltap1.png"
#define WALLTAPS2 "resources/media/Level/Walltap2.png"
#define FLOOR "resources/media/Level/floor.jpg"
#define ENDLEVEL "resources/media/Level/EndTW.png"
#define CRATE "resources/media/Level/Crate.jpg"

/* SOUNDS */
/* Weapon Sounds */
#define SOUNDGUN "resources/media/Sound/Weapon/Shot/pistol.wav"
#define SOUNDSHOTGUN "resources/media/Sound/Weapon/Shot/SHOTGUN7.flac"
#define CHANGESHOTGUN "resources/media/Sound/Weapon/ChangeWeapon/shotgnck.wav"
#define SOUNDRIFLE "resources/media/Sound/Weapon/Shot/chaingun.wav"
#define CHANGERIFLE "resources/media/Sound/Weapon/ChangeWeapon/wpnsel21.wav"
#define SOUNDROCKET "resources/media/Sound/Weapon/Shot/rpgfire.wav"
#define CHANGEROCKET "resources/media/Sound/Weapon/ChangeWeapon/wpnsel21.wav"
#define SOUNDRPG "resources/media/Sound/Weapon/Shot/rpgfire.wav"
#define CHANGERPG "resources/media/Sound/Weapon/ChangeWeapon/wpnsel21.wav"

/* End Level Sounds */
#define SOUNDENDLEVEL "resources/media/Sound/Level/EndLevel/bombexpl.wav"
#define POC "resources/media/Sound/Level/EndLevel/piece02.wav"

/* Enemy Sounds */
#define TRIGGERENEMY "resources/media/Sound/Enemy/predrg.wav"
#define DIEENEMY "resources/media/Sound/Enemy/preddy.wav"
#define DMGENEMY "resources/media/Sound/Enemy/predpn.wav"

/* Get Items Sound */
#define GETITEM "resources/media/Sound/Item/getitm19.wav"

/* Player */
#define COOLSOUND "resources/media/Sound/cool01.wav"
#define DMGSOUND "resources/media/Sound/dscrem17.wav"
#define DEATHSOUND "resources/media/Sound/dmdeath.wav"
#define AMMO_VALUE 15
#define HEAL_VALUE 10
#define PLAYER_LIFE_MAX 100
#define PLAYER_SPEED 300

/* Headshot */
#define HEADSHOT "resources/media/Sound/HEADSHOT.wav"

/* MUSICS */
#define DEFAULTMUSIC "resources/media/Music/MainTitleTheme.ogg"
#define MUSIC1 "resources/media/Music/202_-_Duke_Nukem_3D_-_DOS_-_Stalker.ogg"
#define MUSIC2 "resources/media/Music/204_-_Duke_Nukem_3D_-_DOS_-_The_City_Streets.ogg"

/* Main Menu Background */
#define MAINBACKGROUND "resources/media/Menu/Main/Background.png"

/* ENEMY */
#define FORWARDWALKE "resources/media/Sprites/SpriteEnemy/troopers"
#define BACKWARDWALKE "resources/media/Sprites/SpriteEnemy/trooperBack"
#define DEADE "resources/media/Sprites/SpriteEnemy/troopersDeath"
#define ENEMY_LIFE 100
#define ENEMY_DAMAGE 10
#define DAMAGE_CD 1.0
#define ENEMY_VIEW 100

/* EXTENSION */
#define PNG ".png"
#define JPG ".jpg"

/* Physics Update */
#define FIXED_FRAMETIME 0.02

/* Layers */
#define GROUND_LAYER 1
#define PLAYER_LAYER 2
#define ENEMY_LAYER 3

#endif // !_DEFINE_H_
