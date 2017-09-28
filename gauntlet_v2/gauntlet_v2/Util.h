#pragma once

enum class ANIMATION_STATE 
{
	WALK_UP,
	WALK_DOWN,
	WALK_RIGHT,
	WALK_LEFT,
	IDLE_UP,
	IDLE_DOWN,
	IDLE_RIGHT,
	IDLE_LEFT,
	COUNT
};

const std::string PLAYER_TAG = "Player";
const std::string FOLLOWER_TAG = "Follower";