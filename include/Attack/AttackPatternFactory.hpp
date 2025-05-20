// include/Attack/AttackPatternFactory.hpp
#ifndef ATTACKPATTERNFACTORY_HPP
#define ATTACKPATTERNFACTORY_HPP

#include "Attack/AttackPattern.hpp"
#include "Attack/CircleAttack.hpp"
#include "Attack/CornerBulletAttack.hpp"
#include "Attack/RectangleAttack.hpp"  // 整合後不再需要單獨的 LaserAttack.hpp

/**
 * @class AttackPatternFactory
 * @brief 負責創建各種預定義的攻擊模式
 *
 * 此工廠類別提供各種常用攻擊模式的創建方法，便於敵人重用這些模式。
 * 每個方法返回一個完全配置好的AttackPattern對象，可直接由敵人使用。
 */
class AttackPatternFactory {
public:
    /**
     * @brief 獲取工廠的單例實例
     * @return 工廠實例的引用
     */
    static AttackPatternFactory& GetInstance();

    // 禁止複製和移動
    AttackPatternFactory(const AttackPatternFactory&) = delete;
    AttackPatternFactory(AttackPatternFactory&&) = delete;
    AttackPatternFactory& operator=(const AttackPatternFactory&) = delete;
    AttackPatternFactory& operator=(AttackPatternFactory&&) = delete;

    void AddCircleAttackRow(
        std::shared_ptr<AttackPattern> pattern,
        const glm::vec2& startPos,
        bool isHorizontal,
        int count,
        float zindex,
        float spacing,
        float radius,
        const Util::Color& color,
        const glm::vec2& moveDirection,
        float moveSpeed,
        float moveDistance,
        float delay,
        float startTime,
        float timeInterval);

    void AddCrossLaserAttack(
        std::shared_ptr<AttackPattern> pattern,
        const glm::vec2& centerPosition,
        float width,
        float length,
        const Util::Color& color,
        float delay,
        float startTime,
        float duration = 0.5f,
        int sequenceNumber = 1);

    std::shared_ptr<AttackPattern> CreateBattle1Pattern();
    std::shared_ptr<AttackPattern> CreateBattle2Pattern();
    std::shared_ptr<AttackPattern> CreateBattle3Pattern();
    std::shared_ptr<AttackPattern> CreateBattle4Pattern();
    std::shared_ptr<AttackPattern> CreateBattle5Pattern();
    std::shared_ptr<AttackPattern> CreateBattle6Pattern();
    std::shared_ptr<AttackPattern> CreateBattle7Pattern();
    std::shared_ptr<AttackPattern> CreateBattle8Pattern();
    std::shared_ptr<AttackPattern> BossPattern1();
    std::shared_ptr<AttackPattern> BossPattern2();
    std::shared_ptr<AttackPattern> BossPattern3();
    std::shared_ptr<AttackPattern> BossPattern4();
    std::shared_ptr<AttackPattern> BossPattern5();

private:
    // 私有構造函數防止外部創建實例
    AttackPatternFactory() = default;

    // 計算環形位置的輔助方法
    std::vector<glm::vec2> CalculateCircularPositions(
        const glm::vec2& center,
        float radius,
        int count,
        float startAngle = 0.0f);
};

#endif // ATTACKPATTERNFACTORY_HPP