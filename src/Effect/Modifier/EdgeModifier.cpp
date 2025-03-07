#include "Effect/Modifier/EdgeModifier.hpp"
#include "Util/Logger.hpp"

namespace Effect {
    namespace Modifier {

        EdgeModifier::EdgeModifier(EdgeType type, float width, const Util::Color& edgeColor)
            : m_EdgeType(type), m_Width(width), m_EdgeColor(edgeColor) {
        }

        void EdgeModifier::Apply(Core::Program& program) {
            // 如果是第一次應用，獲取uniform位置
            if (m_EdgeTypeLocation == -1) {
                m_EdgeTypeLocation = glGetUniformLocation(program.GetId(), "u_EdgeType");
                m_EdgeWidthLocation = glGetUniformLocation(program.GetId(), "u_EdgeWidth");
                m_EdgeColorLocation = glGetUniformLocation(program.GetId(), "u_EdgeColor");

                if (m_EdgeTypeLocation == -1 || m_EdgeWidthLocation == -1 || m_EdgeColorLocation == -1) {
                    LOG_ERROR("Failed to get uniform locations for EdgeModifier");
                }
            }

            // 設置uniform值
            glUniform1i(m_EdgeTypeLocation, static_cast<int>(m_EdgeType));
            glUniform1f(m_EdgeWidthLocation, m_Width);
            glUniform4f(m_EdgeColorLocation, m_EdgeColor.r, m_EdgeColor.g, m_EdgeColor.b, m_EdgeColor.a);
        }

    } // namespace Modifier
} // namespace Effect
