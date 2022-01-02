#include "Sorcerer2TapeFatFsSpi.h"

Sorcerer2TapeFatFsSpi::Sorcerer2TapeFatFsSpi(SdCardFatFsSpi* sdCard, const char* name, bool writable) :
  _sdCard(sdCard),
  _name(name),
  _open(false),
  _writable(writable)
{
}
