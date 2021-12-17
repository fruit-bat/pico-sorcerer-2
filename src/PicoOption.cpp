#include "PicoOption.h"

bool PicoOption::toggleSelection() {
  selected(!_selected);
  return _selected;
}
