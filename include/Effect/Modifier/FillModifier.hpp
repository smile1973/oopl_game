#ifndef FILLMODIFIER_HPP
#define FILLMODIFIER_HPP

#include "Effect/Shape/BaseShape.hpp"

namespace Effect {
    namespace Modifier {

        enum class FillType {
            SOLID,
            HOLLOW
        };

        class FillModifier {
        public:
            FillModifier(FillType type = FillType::SOLID, float thickness = 0.02f);
            void Apply(Core::Program& program);
        private:
            FillType m_FillType;
            float m_Thickness;

            GLint m_FillTypeLocation = -1;
            GLint m_ThicknessLocation = -1;
        };

    }
}

#endif
