#ifndef EDGEMODIFIER_HPP
#define EDGEMODIFIER_HPP

#include "Effect/Shape/BaseShape.hpp"

namespace Effect {
    namespace Modifier {
        enum class EdgeType {
            NONE,
            DARK,
            GLOW
        };

        class EdgeModifier {
        public:
            EdgeModifier(EdgeType type = EdgeType::NONE, float width = 0.05f, const Util::Color& edgeColor = Util::Color::FromName(Util::Colors::PINK));
            void Apply(Core::Program& program);
        private:
            EdgeType m_EdgeType;
            float m_Width;
            Util::Color m_EdgeColor;

            GLint m_EdgeTypeLocation = -1;
            GLint m_EdgeWidthLocation = -1;
            GLint m_EdgeColorLocation = -1;
        };

    }
}

#endif
