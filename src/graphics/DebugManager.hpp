#pragma once

#include <common/math.hpp>
#include <common/bgfx_utils.h>
#include <common/debugdraw/debugdraw.h>
#include <graphics/GraphicsEngine.hpp>

namespace pumpkin {

class DebugManager {


public:

	template <typename T>
	static void draw(T & object) {
		ddDraw(object);
	}

	static void drawPoly(const std::vector<Vec2f> & poly) {
		Vec2f first_vert = poly[0];
		ddMoveTo(first_vert(0), first_vert(1));
		for (int i = 0; i < poly.size(); i++) {
			ddLineTo(poly[i](0), poly[i](1));
		}
		ddLineTo(first_vert(0), first_vert(1));
	}

	static void update(float delta) {

		ddBegin(RENDER_PASS_GEOMETRY);
		ddPush();

		for (auto aabb : aabb_vec) {
			draw(aabb);
		}

		for (auto cyl : cyl_vec) {
			ddDrawCylinder(cyl.m_pos, cyl.m_end, cyl.m_radius);
		}

		for (auto poly : poly_vec) {
			drawPoly(poly);
		}

		ddPop();
		ddEnd();

		clear();
	}

	static void push_cyl(Vec3f from, Vec3f to, float rad) {
		Cylinder c;
		FROM_V3(c.m_pos, from);
		FROM_V3(c.m_end, to);
		c.m_radius = rad;
		cyl_vec.push_back(c);
	}

	static void push_polygon(const std::vector<Vec2f> & poly) {
		poly_vec.push_back(poly);
	}

	static void push_aabb(Vec3f a, Vec3f b) {
		Aabb box;
		box.m_min[0] = a(0);
		box.m_min[1] = a(1);
		box.m_min[2] = a(2);
		box.m_max[0] = b(0);
		box.m_max[1] = b(1);
		box.m_max[2] = b(2);
		aabb_vec.push_back(box);
	}

	static void clear() {
		cyl_vec.clear();
		aabb_vec.clear();
		poly_vec.clear();
	}

protected:

	static std::vector<Aabb> aabb_vec;
	static std::vector<Cylinder> cyl_vec;
	static std::vector<std::vector<Vec2f>> poly_vec;


};

}
