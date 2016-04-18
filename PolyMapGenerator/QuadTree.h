#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <SFML/System.hpp>

#include "Math/Vector2.h"

// Forward Declaration
struct AABB;

template <typename T>
class QuadTree
{
public:
	QuadTree() :
		m_northWest(nullptr), m_northEast(nullptr),
		m_southEast(nullptr), m_southWest(nullptr),
		m_divided(false), m_branchDepth(0), m_elementsBranch(0) { }
	QuadTree(AABB boundary, int depth) :
		m_boundary(boundary), m_divided(false), m_branchDepth(depth), m_elementsBranch(0) { }

	~QuadTree()
	{
		if (m_divided)
		{
			delete m_northWest;
			delete m_northEast;
			delete m_southEast;
			delete m_southWest;
		}
	}

	QuadTree(const QuadTree& qt) = delete;
	QuadTree(QuadTree&& qt) = delete;

	QuadTree& operator=(const QuadTree& qt) = delete;
	QuadTree& operator=(QuadTree&& qt) = delete;

	AABB m_boundary;

private:
	static int MAX_TREE_DEPTH;

	std::vector<T> m_elements;
	std::vector<AABB> m_elementRegions;

	QuadTree* m_northWest;
	QuadTree* m_northEast;
	QuadTree* m_southEast;
	QuadTree* m_southWest;

	bool m_divided;
	int m_branchDepth;
	int m_elementsBranch;
};

template <typename T>
int QuadTree<T>::MAX_TREE_DEPTH = 6;

// Axis-Aligned Bounding Box (AABB)
struct AABB
{
	AABB() = default;
	AABB(Vector2 pos, Vector2 half) : m_pos(pos), m_half(half) { }

	~AABB() = default;

	AABB(const AABB& aabb) = default;
	AABB(AABB&& aabb) = default;

	AABB& operator=(const AABB& aabb) = default;
	AABB& operator=(AABB&& aabb) = default;

	bool IsContain(const Vector2 point) const
	{
		Vector2 minPoint = m_pos - m_half;
		if (point.x >= minPoint.x && point.y >= minPoint.y)
		{
			Vector2 maxPoint = m_pos + m_half;
			return point.x <= maxPoint.x && point.y <= maxPoint.y;
		}

		return false;
	}

	bool IsIntersect(const AABB& sec) const
	{
		double diffX = abs(m_pos.x - sec.m_pos.x);
		double diffY = abs(m_pos.y - sec.m_pos.y);

		if (diffX > m_half.x + sec.m_half.x || diffY > m_half.y + sec.m_half.y)
		{
			return false;
		}

		return true;
	}

	Vector2 m_pos;
	Vector2 m_half;
};

#endif