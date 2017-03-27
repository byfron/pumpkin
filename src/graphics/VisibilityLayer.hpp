//
// Created by rubio on 24/03/17.
//

#ifndef VOYAGE_VISIBILITYLAYER_H
#define VOYAGE_VISIBILITYLAYER_H

#include "GraphicsObject.hpp"
#include "DebugManager.hpp"

namespace pumpkin {

    class VisibilityLayer : public pumpkin::GraphicsObject {

    public:

        VisibilityLayer() {}

        void update(float delta) {

            pumpkin::DebugManager::push_polygon(m_polygon);

        }

        std::vector<Vec2f> m_polygon;
    };

}
#endif //VOYAGE_VISIBILITYLAYER_H
