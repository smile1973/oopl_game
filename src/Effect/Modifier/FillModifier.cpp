#include "Effect/Modifier/FillModifier.hpp"
#include "Util/Logger.hpp"

namespace Effect {
    namespace Modifier {

        FillModifier::FillModifier(FillType type, float thickness)
            : m_FillType(type), m_Thickness(thickness) {
        }

        void FillModifier::Apply(Core::Program& program) {
            // 如果是第一次應用，獲取uniform位置
            if (m_FillTypeLocation == -1) {
                m_FillTypeLocation = glGetUniformLocation(program.GetId(), "u_FillType");
                m_ThicknessLocation = glGetUniformLocation(program.GetId(), "u_Thickness");

                if (m_FillTypeLocation == -1 || m_ThicknessLocation == -1) {
                    LOG_ERROR("Failed to get uniform locations for FillModifier");
                }
            }

            // 設置uniform值
            glUniform1i(m_FillTypeLocation, static_cast<int>(m_FillType));
            glUniform1f(m_ThicknessLocation, m_Thickness);
        }

    } // namespace Modifier
} // namespace Effect