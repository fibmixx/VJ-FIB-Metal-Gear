#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, int newlevel)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	map->setLevel(newlevel);
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin >> tile;
			map[j * mapSize.x + i] = tile;
		}
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

bool TileMap::isTileSolid(int tile, int level) const
{
	if (level == 1) {
		return !(tile == 45 || tile == 49 || tile == 50 || tile == 51 || tile == 52 || tile == 53);
	}
	else if (level == 2) {
		return !(tile == 39 || tile == 37 || tile == 33 || tile == 61 || tile == 56 || tile == 47 ||
			tile == 57 || tile == 58 || tile == 59 || tile == 60 || tile == 36);
	}
	else if (level == 3) {
		return !(tile == 7 || tile == 8 || tile == 9 || tile == 14 || tile == 22 || tile == 45 ||
			tile == 23 || tile == 62 || tile == 63 || tile == 51 || tile == 52 || tile == 53 ||
			tile == 54 || tile == 43);
	}
	else if (level == 4) {
		return !(tile == 4 || tile == 5 || tile == 6 || tile == 7 || tile == 18 || tile == 35 || tile == 36);
	}
	else if (level == 5) {
		return !(tile == 20 || tile == 28 || tile == 21);
	}
	else if (level == 6) {
		return !(tile == 17 || tile == 20 || tile == 24 || tile == 29 || tile == 19);
	}
	else if (level == 7) {
		return !(tile == 1 || tile == 22 || tile == 17 || tile == 31);
	}
	else if (level == 8) {
		return !(tile == 5 || tile == 6 || tile == 22 || tile == 29);
	}
	else if (level == 9) {
		return !(tile == 3 || tile == 4 || tile == 29 || tile == 36);
	}
	else if (level == 10) {
		return !(tile == 3 || tile == 4 || tile == 21);
	}
	else if (level == 11) {
		return !(tile == 21 || tile == 20 || tile == 26 || tile == 37 || tile == 5);
	}
	else if (level == 12)
		return !(tile == 17 || tile == 19 || tile == 36);
	else {
		return true;
	}
}


// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + 10) / tileSize;
	y0 = (pos.y + size.y * 0.75) / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{	
		int tile = map[y * mapSize.x + x];
		if (isTileSolid(tile, level))
			return true;
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 10) / tileSize;
	y0 = (pos.y + size.y * 0.75) / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		if (isTileSolid(tile, level))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = (pos.x + 10) / tileSize;
	x1 = (pos.x + size.x - 10) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		if (isTileSolid(tile, level))
			if (tile != 0 && tile != 3 && tile != 4 && tile != 21)
				return true;
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = (pos.x + 10) / tileSize;
	x1 = (pos.x + size.x - 10) / tileSize;
	y = (pos.y + size.y * 0.6) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		if (isTileSolid(tile, level))
			return true;
	}

	return false;
}

bool TileMap::hasObstacleBetween(const glm::ivec2& start, const glm::ivec2& end) const
{
	// Conversión a coordenadas de tile
	glm::ivec2 tileStart = start / tileSize;
	glm::ivec2 tileEnd = end / tileSize;

	int x0 = tileStart.x;
	int y0 = tileStart.y;
	int x1 = tileEnd.x;
	int y1 = tileEnd.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		// Calcula índice y verifica colisión
		if (x0 >= 0 && x0 < mapSize.x && y0 >= 0 && y0 < mapSize.y) {
			int tile = map[y0 * mapSize.x + x0];
			if (isTileSolid(tile, level))
				return true; // hay obstáculo
		}

		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}

	return false; // no hay obstáculos
}

void TileMap::changeLevel(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

void TileMap::setLevel(int newlevel)
{
	level = newlevel;
}
