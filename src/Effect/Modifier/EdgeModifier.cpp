// src/Effect/Modifier/EdgeModifier.cpp - 優化版本
#include "Effect/Modifier/EdgeModifier.hpp"
#include "Util/Logger.hpp"

namespace Effect {
    namespace Modifier {

        EdgeModifier::EdgeModifier(EdgeType type, float width, const Util::Color& edgeColor)
            : m_EdgeType(type), m_Width(width), m_EdgeColor(edgeColor) {
        }

        void EdgeModifier::Apply(Core::Program& program) {
            // 獲取 uniform 位置（只在第一次獲取）
            if (m_EdgeTypeLocation == -1) {
                m_EdgeTypeLocation = glGetUniformLocation(program.GetId(), "u_EdgeType");
                m_EdgeWidthLocation = glGetUniformLocation(program.GetId(), "u_EdgeWidth");
                m_EdgeColorLocation = glGetUniformLocation(program.GetId(), "u_EdgeColor");

                // 檢查是否成功獲取 uniform 位置
                if (m_EdgeTypeLocation == -1) {
                    LOG_WARN("u_EdgeType uniform not found in program {}", program.GetId());
                }
                if (m_EdgeWidthLocation == -1) {
                    LOG_WARN("u_EdgeWidth uniform not found in program {}", program.GetId());
                }
                if (m_EdgeColorLocation == -1) {
                    LOG_WARN("u_EdgeColor uniform not found in program {}", program.GetId());
                }
            }

            // 設置 uniform 值
            if (m_EdgeTypeLocation != -1) {
                glUniform1i(m_EdgeTypeLocation, static_cast<int>(m_EdgeType));
            }
            
            if (m_EdgeWidthLocation != -1) {
                glUniform1f(m_EdgeWidthLocation, m_Width);
            }
            
            if (m_EdgeColorLocation != -1) {
                glUniform4f(m_EdgeColorLocation, m_EdgeColor.r, m_EdgeColor.g, m_EdgeColor.b, m_EdgeColor.a);
            }
        }

    }
}