/*
 * This file is part of Spacel game.
 *
 * Copyright 2016, Loic Blot <loic.blot@unix-experience.fr>
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

#include <cassert>
#include "space.h"
#include "generators.h"

namespace spacel {
namespace engine {

Planet::~Planet()
{
	for (auto &moon: moons) {
		delete moon;
	}
}
SolarSystem::~SolarSystem()
{
	for (auto &planet: planets) {
		delete planet;
	}
}

Galaxy::~Galaxy()
{
	for (auto &ss: solar_systems) {
		delete ss.second;
	}
}

Universe::~Universe()
{
	// Don't delete solar systems, it's done by deleting galaxies
	for (auto &galaxy: m_galaxies) {
		delete galaxy.second;
	}

	// @TODO save current new object ids
}

/*
 * This function init solar system without its planets
 */
void Universe::CreateSolarSystem(Galaxy *galaxy)
{
	while (m_solar_systems.find(m_next_solarsystem_id) != m_solar_systems.end()) {
		m_next_solarsystem_id++;
	}

	SolarSystem *ss = new SolarSystem();
	ss->id = m_next_solarsystem_id;
	ss->name = UniverseGenerator::instance()->generate_world_name();
	ss->radius = UniverseGenerator::instance()->
		generate_solarsystem_radius(m_next_solarsystem_id);
	ss->type = (SolarType) UniverseGenerator::instance()->
			generate_solarsystem_type(m_next_solarsystem_id);

	// @TODO generate coordinates properly
	ss->pos_x = 1;
	ss->pos_y = 1;
	ss->pos_z = 1;
	ss->galaxy = galaxy;

	galaxy->solar_systems[m_next_solarsystem_id] = ss;
	m_solar_systems[m_next_solarsystem_id] = ss;
	m_next_solarsystem_id++;
}

void Universe::CreateSolarSystemPhase2(SolarSystem *ss)
{
	uint8_t planet_number = UniverseGenerator::instance()->
		generate_solarsystem_planetnumber(ss->id, ss->type);

	for (uint8_t i = 0; i < planet_number; ++i) {
		/*
		 * @TODO do this for planets
		 * while (m_galaxies.find(m_next_galaxy_id) != m_galaxies.end()) {
			m_next_galaxy_id++;
		}*/

		Planet *planet = new Planet();
		planet->id = m_next_planet_id;
		planet->name = UniverseGenerator::instance()->generate_world_name();
		planet->type = (PlanetType) UniverseGenerator::instance()->
			generate_planet_type(planet->id);
		planet->radius = UniverseGenerator::instance()->
			generate_planet_radius(planet->id, planet->type);
		planet->distance_to_parent = UniverseGenerator::instance()->
			generate_planet_distance(planet->id, planet->type, ss->radius);
		planet->moons = {};

		ss->planets.push_back(planet);

		m_next_planet_id++;
	}
}

bool Universe::RemoveSolarSystem(const uint64_t &id)
{
	SolarSystemMap::iterator ss_it = m_solar_systems.find(id);
	if (ss_it == m_solar_systems.end()) {
		return false;
	}

	// If there is galaxy drop the pointer from the galaxy
	if (Galaxy *galaxy = (*ss_it).second->galaxy) {
		SolarSystemMap::iterator ss_galaxy_it = galaxy->solar_systems.find(id);
		if (ss_galaxy_it != galaxy->solar_systems.end()) {
			galaxy->solar_systems.erase(ss_galaxy_it);
		}
	}

	// And then destroy object and reference in universe
	delete (*ss_it).second;
	m_solar_systems.erase(ss_it);
	return true;
}

bool Universe::SetGalaxy(Galaxy *galaxy)
{
	if (m_galaxies.find(galaxy->id) != m_galaxies.end()) {
		return false;
	}

	m_galaxies[galaxy->id] = galaxy;
	return true;
}

Galaxy *Universe::CreateGalaxy(const uint64_t &max_solar_systems)
{
	// Increment id if there is galaxies
	while (m_galaxies.find(m_next_galaxy_id) != m_galaxies.end()) {
		m_next_galaxy_id++;
	}

	Galaxy *galaxy = new Galaxy();
	galaxy->id = m_next_galaxy_id;
	galaxy->name = UniverseGenerator::instance()->generate_world_name();

	// Hardcoded but need some seed
	for (uint32_t i = 0; i < max_solar_systems; ++i) {
		CreateSolarSystem(galaxy);
	}

	m_galaxies[m_next_galaxy_id] = galaxy;

	m_next_galaxy_id++;
	return galaxy;
}

bool Universe::RemoveGalaxy(const uint64_t &id)
{
	GalaxyMap::iterator galaxy_it = m_galaxies.find(id);
	if (galaxy_it == m_galaxies.end()) {
		return false;
	}

	delete (*galaxy_it).second;
	return true;
}

}
}
