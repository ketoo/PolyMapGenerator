#include "Math/LineEquation.h"
#include "Structure.h"

Edge::Edge(unsigned int index, Center* center1, Center* center2, Corner* corner1, Corner* corner2) :
	m_index(index), m_d0(center1), m_d1(center2), m_v0(corner1), m_v1(corner2), m_riverVolume(0.0)
{

}

bool Center::RemoveEdge(Edge* e)
{
	for (auto edgeIter = m_edges.begin(); edgeIter != m_edges.end(); ++edgeIter)
	{
		if (*edgeIter == e)
		{
			m_edges.erase(edgeIter);
			return true;
		}
	}

	return false;
}

bool Center::RemoveCorner(Corner* c)
{
	for (auto iter = m_corners.begin(); iter != m_corners.end(); ++iter)
	{
		if (*iter == c)
		{
			m_corners.erase(iter);
			return true;
		}
	}

	return false;
}

Edge* Center::GetEdgeWith(Center* c)
{
	for (auto iter = m_edges.begin(); iter != m_edges.end(); ++iter)
	{
		if ((*iter)->m_d0 == c || (*iter)->m_d1 == c)
		{
			return *iter;
		}
	}

	return nullptr;
}

void Center::MakeBorder()
{
	m_border = true;
	m_water = true;
	m_ocean = true;

	for (auto iter = m_corners.begin(); iter != m_corners.end(); ++iter)
	{
		(*iter)->m_border = true;
		(*iter)->m_water = true;
		(*iter)->m_ocean = true;
	}
}

bool Center::IsInsideBoundingBox(int width, int height) const
{
	if (m_position.x < 0 || m_position.x >= width || m_position.y < 0 || m_position.y >= height)
	{
		return false;
	}

	return true;
}

bool Center::IsContain(Vector2 pos)
{
	if (m_corners.size() < 3)
	{
		return false;
	}

	Vector2 firstSec(m_corners[0]->m_position, m_corners[1]->m_position);
	Vector2 firstPos(m_corners[0]->m_position, pos);
	bool sign = firstSec.CrossProduct(firstPos) > 0;

	for (auto iter = m_corners.begin() + 1; iter != m_corners.end() - 1; ++iter)
	{
		Vector2 ab((*iter)->m_position, (*(iter + 1))->m_position);
		Vector2 ap((*iter)->m_position, pos);

		if (sign != ab.CrossProduct(ap) > 0)
		{
			return false;
		}
	}

	return true;
}

std::pair<Vector2, Vector2> Center::GetBoundingBox()
{
	double minX = m_corners[0]->m_position.x, maxX = m_corners[0]->m_position.x;
	double minY = m_corners[0]->m_position.y, maxY = m_corners[0]->m_position.y;

	for (auto iter = m_corners.begin() + 1; iter != m_corners.end(); ++iter)
	{
		if ((*iter)->m_position.x < minX)
		{
			minX = (*iter)->m_position.x;
		}
		else if ((*iter)->m_position.x > maxX)
		{
			maxX = (*iter)->m_position.x;
		}

		if ((*iter)->m_position.y < minY)
		{
			minY = (*iter)->m_position.y;
		}
		else if ((*iter)->m_position.y > maxY)
		{
			maxY = (*iter)->m_position.y;
		}
	}

	Vector2 minPos(minX, minY);
	Vector2 maxPos(maxX, maxY);
	Vector2 halfDiagonal(Vector2(minPos, maxPos) / 2);

	return std::make_pair(minPos + halfDiagonal, halfDiagonal);
}

void SortCorners();
bool IsGoesBefore(Vector2 a, Vector2 b);