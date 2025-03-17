// include/Attack/AttackPatternFactory.hpp
#ifndef ATTACKPATTERNFACTORY_HPP
#define ATTACKPATTERNFACTORY_HPP

#include "Attack/AttackPattern.hpp"
#include "Attack/CircleAttack.hpp"
#include "Attack/RectangleAttack.hpp"
#include "Attack/LaserAttack.hpp"

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

    /**
     * @brief 創建單個圓形攻擊的模式
     * @param position 攻擊位置
     * @param radius 圓形半徑
     * @param delay 倒數時間
     * @return 配置好的攻擊模式
     */
    std::shared_ptr<AttackPattern> CreateSingleCirclePattern(
        const glm::vec2& position,
        float radius = 100.0f,
        float delay = 2.0f);

    /**
     * @brief 創建多個圓形攻擊的模式
     * @param positions 多個攻擊位置
     * @param radius 圓形半徑
     * @param delay 倒數時間
     * @param interval 攻擊間隔時間
     * @return 配置好的攻擊模式
     */
    std::shared_ptr<AttackPattern> CreateMultiCirclePattern(
        const std::vector<glm::vec2>& positions,
        float radius = 100.0f,
        float delay = 2.0f,
        float interval = 0.5f);

    /**
     * @brief 創建矩形攻擊的模式
     * @param position 攻擊位置
     * @param width 矩形寬度
     * @param height 矩形高度
     * @param rotation 矩形旋轉角度
     * @param delay 倒數時間
     * @return 配置好的攻擊模式
     */
    std::shared_ptr<AttackPattern> CreateRectanglePattern(
        const glm::vec2& position,
        float width = 200.0f,
        float height = 100.0f,
        float rotation = 0.0f,
        float delay = 2.0f);

    /**
     * @brief 創建雷射攻擊的模式
     * @param position 攻擊起始位置
     * @param direction 雷射方向
     * @param width 雷射寬度
     * @param length 雷射長度
     * @param delay 倒數時間
     * @return 配置好的攻擊模式
     */
    std::shared_ptr<AttackPattern> CreateLaserPattern(
        const glm::vec2& position,
        LaserAttack::Direction direction,
        float width = 80.0f,
        float length = 2000.0f,
        float delay = 2.0f);

    /**
     * @brief 創建多個雷射攻擊的模式
     * @param positions 多個攻擊起始位置
     * @param directions 多個雷射方向
     * @param width 雷射寬度
     * @param length 雷射長度
     * @param delay 倒數時間
     * @param interval 攻擊間隔時間
     * @return 配置好的攻擊模式
     */
    std::shared_ptr<AttackPattern> CreateMultiLaserPattern(
        const std::vector<glm::vec2>& positions,
        const std::vector<LaserAttack::Direction>& directions,
        float width = 80.0f,
        float length = 2000.0f,
        float delay = 2.0f,
        float interval = 0.5f);

    /**
     * @brief 創建環形圓形攻擊的模式
     * @param centerPosition 中心位置
     * @param radius 環的半徑
     * @param attackRadius 每個攻擊的半徑
     * @param count 環上攻擊的數量
     * @param delay 倒數時間
     * @param interval 攻擊間隔時間
     * @return 配置好的攻擊模式
     */
    std::shared_ptr<AttackPattern> CreateCircularPattern(
        const glm::vec2& centerPosition,
        float radius = 300.0f,
        float attackRadius = 80.0f,
        int count = 8,
        float delay = 2.0f,
        float interval = 0.2f);

    // BATTLE 1 特殊攻擊模式
    std::shared_ptr<AttackPattern> CreateBattle1Pattern();

    // BATTLE 2 特殊攻擊模式
    std::shared_ptr<AttackPattern> CreateBattle2Pattern();

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