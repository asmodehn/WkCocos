#include "WkCocosApp/GameLogic.h"


GameLogic::GameLogic()
: m_localdatamngr(new WkCocos::LocalData::LocalDataManager())
, m_options(new MyOptions(m_localdatamngr))
, m_player(new MyPlayer(m_localdatamngr))
{}

GameLogic::~GameLogic()
{}

