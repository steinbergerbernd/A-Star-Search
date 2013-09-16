#include "Target.h"


Target::Target(World* world, const HexMap& hexMap, CL_Point position) : MapObject(world, hexMap, position, CL_Colorf::darkmagenta)
{
}


Target::~Target(void)
{
}
