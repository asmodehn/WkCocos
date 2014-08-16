#include "WkCocosApp/GameLogic.h"


GameLogic::GameLogic()
: m_localdatamngr(new WkCocos::LocalData::LocalDataManager(/*std::bind(&GameLogic::localDataError,this)*/))
, m_options(new MyOptions(m_localdatamngr))
, m_player(new MyPlayer(m_localdatamngr))
{}

GameLogic::~GameLogic()
{}

