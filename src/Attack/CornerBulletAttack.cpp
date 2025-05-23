#include "Attack/CornerBulletAttack.hpp"
#include "Util/Logger.hpp"
#include "Effect/EffectManager.hpp"
#include "Attack/AttackManager.hpp" // 添加引用攻擊管理器
#include <cmath>

CornerBulletAttack::CornerBulletAttack(float delay, int bulletCount, int sequenceNumber)
    : CircleAttack({0, 0}, delay, 30.0f, sequenceNumber),  // 使用基類構造函數
      m_BulletCount(bulletCount) {

    // 初始化隨機數生成器
    std::random_device rd;
    m_RandomEngine.seed(rd());
}

void CornerBulletAttack::SetBulletSpeed(float speed) {
    m_BulletSpeed = speed;
}

std::vector<float> CornerBulletAttack::GenerateRandomAngles(float baseAngle) {
    // 與原來相同
    std::vector<float> angles;

    // 固定偏移角
    // 將角度轉換為弧度 (π/180 * 角度)
    std::vector<float> fixedOffsets1 = {
        M_PI / 180.0f * 35.0f,
        M_PI / 180.0f * 55.0f,
        M_PI / 180.0f * 75.0f
    };
    std::vector<float> fixedOffsets2 = {
        M_PI / 180.0f * 15.0f,
        M_PI / 180.0f * 35.0f,
        M_PI / 180.0f * 55.0f
    };

    // 隨機偏移 ±5度
    float maxDeviation = M_PI / 180.0f * 5.0f;  // 5度轉換為弧度
    std::uniform_real_distribution<float> distribution(-maxDeviation, maxDeviation);

    // 生成每個子彈的角度
    std::vector<float> use = (baseAngle == 0.0f || baseAngle == M_PI) ? fixedOffsets1 : fixedOffsets2;

    for (int i = 0; i < m_BulletCount; ++i) {
        float angle = baseAngle + use[i % use.size()] + distribution(m_RandomEngine);
        angles.push_back(angle);
        LOG_DEBUG("Generated bullet angle: {:.2f} ({:.4f} rad)", angle * 180.0f / M_PI, angle);
    }
    return angles;
}

void CornerBulletAttack::AddBulletPath(const glm::vec2& startPosition, float angle) {
    BulletPath path;
    path.startPosition = startPosition;
    path.currentPosition = startPosition;
    path.angle = angle;
    m_BulletPaths.push_back(path);
}

void CornerBulletAttack::CreateWarningEffect() {
    m_BulletPaths.clear();
    m_BulletAttacks.clear();  // 清除之前的子彈攻擊

    // 四個角落的位置
    glm::vec2 topLeft(-642.0f, 362.0f);
    glm::vec2 topRight(642.0f, 362.0f);
    glm::vec2 bottomLeft(-642.0f, -362.0f);
    glm::vec2 bottomRight(642.0f, -362.0f);

    // 設置各個角落的子彈
    auto tlAngles = GenerateRandomAngles(0.0f);
    for (float angle : tlAngles) {
        AddBulletPath(topLeft, angle);
    }

    auto trAngles = GenerateRandomAngles(2.0f * M_PI / 4.0f);
    for (float angle : trAngles) {
        AddBulletPath(topRight, angle);
    }

    auto brAngles = GenerateRandomAngles(4.0f * M_PI / 4.0f);
    for (float angle : brAngles) {
        AddBulletPath(bottomRight, angle);
    }

    auto blAngles = GenerateRandomAngles(6.0f * M_PI / 4.0f);
    for (float angle : blAngles) {
        AddBulletPath(bottomLeft, angle);
    }

    for (auto& path : m_BulletPaths) {
        // 計算彈道終點
        float distance = 3000.0f;
        glm::vec2 endPosition = path.startPosition +
            glm::vec2(cos(path.angle), sin(path.angle)) * distance;

        // 長度和旋轉角度
        float length = glm::distance(path.startPosition, endPosition);
        float width = GetRadius() * 2.0f;

        // 創建矩形警告效果
        auto warningEffect = Effect::EffectManager::GetInstance().GetEffect(Effect::EffectType::RECT_BEAM);
        if (auto rectangleShape = std::dynamic_pointer_cast<Effect::Shape::RectangleShape>(warningEffect->GetBaseShape())) {
            rectangleShape->SetDimensions(glm::vec2(1.0f, width / length));
            rectangleShape->SetRotation(path.angle);
            rectangleShape->SetSize({length, length});
            rectangleShape->SetColor(Util::Color(1.0f, 0.0f, 0.0f, 0.2f));
            rectangleShape->SetAutoRotation(false);
        }
        warningEffect->SetFillModifier(Effect::Modifier::FillModifier(Effect::Modifier::FillType::SOLID));
        warningEffect->SetEdgeModifier(Effect::Modifier::EdgeModifier(Effect::Modifier::EdgeType::GLOW, 0.01, Util::Color(0.9, 0.1, 0.1, 0.5)));
        warningEffect->SetDuration(m_Delay + 0.5f);

        warningEffect->Play(path.startPosition, m_ZIndex - 1.0f);
        path.warningEffect = warningEffect;
    }
}

void CornerBulletAttack::CreateAttackEffect() {
    LOG_WARN("test+++++++++++++++++++++++++++++++++++++++++++++in create");
    m_BulletAttacks.clear();

    int bulletIndex = 0;
    for (auto& path : m_BulletPaths) {
        auto bulletAttack = std::make_shared<CircleAttack>(
            path.startPosition, 0.1f,
            GetRadius(), bulletIndex++
        );

        bulletAttack->SetColor(Util::Color(1.0f, 0.0f, 0.0f, 0.7f));

        glm::vec2 direction(cos(2.0f * M_PI - path.angle), sin(2.0f * M_PI - path.angle));
        bulletAttack->SetMovementParams(direction, m_BulletSpeed, 3000.0f);

        bulletAttack->SetAttackDuration(5.0f);

        if (m_TargetCharacter) {
            bulletAttack->SetTargetCharacter(m_TargetCharacter);
        }

        AttackManager::GetInstance().RegisterAttack(bulletAttack);
        path.bulletAttack = bulletAttack;
        m_BulletAttacks.push_back(bulletAttack);
    }
    LOG_WARN("test+++++++++++++++++++++++++++++++++++++++++++++end create");
}

void CornerBulletAttack::OnAttackStart() {
    CreateAttackEffect();
    for (auto& path : m_BulletPaths) {
        if (path.warningEffect) {
            path.warningEffect->Reset();
        }
    }
}

void CornerBulletAttack::CleanupVisuals() {
    CircleAttack::CleanupVisuals();
    for (auto& path : m_BulletPaths) {
        if (path.warningEffect) {
            path.warningEffect->Reset();
        }
    }
}