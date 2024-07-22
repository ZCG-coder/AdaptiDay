#pragma once

#include "store/mainStore.hpp"

#include <SDL.h>
#include <SDL_opengl.h>
#include <memory>

void runEditingModal(const std::shared_ptr<MainStore>& store);
