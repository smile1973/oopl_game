#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"
#include "Util/Renderer.hpp"
#include "Util/Logger.hpp"

class Enemy : public Character {
public:
    Enemy(float health, const std::vector<std::string>& ImageSet);

    void TakeDamage(float damage);
    [[nodiscard]] bool IsAlive() const;

    void DrawHealthBar(const glm::vec2& position);

private:
    static void InitProgram();
    static void InitVertexArray();
    void InitUniforms();

    static std::unique_ptr<Core::Program> s_Program;
    static std::unique_ptr<Core::VertexArray> s_VertexArray;

    float m_Health;
    float m_MaxHealth;
    float m_Width;
    float m_Height;

    GLint m_ColorLocation;
    GLint m_WidthLocation;
};

#endif // ENEMY_HPP