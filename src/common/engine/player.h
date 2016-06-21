/*
 * This file is part of Spacel game.
 *
 * Copyright 2016, Vincent Glize <vincent.glize@live.fr>
 *
 * Spacel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spacel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spacel.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include "inventory.h"
#include "unit.h"

namespace spacel {

namespace engine {

class Player : public Unit {
public:
	Player(const std::string &username);
	~Player() {};

private:
	std::string m_username = "";
	uint32_t m_hp = 100;
	uint32_t m_stamina = 100;
	std::unordered_map<uint32_t, InventoryPtr> m_inventories;
};
}
}
