#include "Effect/Modifier/FillModifier.hpp"
#include "Util/Logger.hpp"

namespace Effect {
    namespace Modifier {

        FillModifier::FillModifier(FillType type, float thickness)
            : m_FillType(type), m_Thickness(thickness) {
        }

        void FillModifier::Apply(Core::Program& program) {
            if (m_FillTypeLocation == -1) {
                m_FillTypeLocation = glGetUniformLocation(program.GetId(), "u_FillType");

                m_ThicknessLocation = glGetUniformLocation(program.GetId(), "u_Thickness");
                if (m_ThicknessLocation == -1) {
                    m_ThicknessLocation = glGetUniformLocation(program.GetId(), "u_FillThickness");
                }

                // 檢查是否成功獲取必要的 uniform 位置
                if (m_FillTypeLocation == -1) {
                    LOG_WARN("u_FillType uniform not found in program {}", program.GetId());
                }
                if (m_ThicknessLocation == -1) {
                    LOG_WARN("u_Thickness/u_FillThickness uniform not found in program {}", program.GetId());
                }
            }

            // 設置 uniform 值
            if (m_FillTypeLocation != -1) {
                glUniform1i(m_FillTypeLocation, static_cast<int>(m_FillType));
            }

            if (m_ThicknessLocation != -1) {
                glUniform1f(m_ThicknessLocation, m_Thickness);
            }
        }
    }
}