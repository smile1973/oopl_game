#ifndef PHASE_MANAGER_HPP
#define PHASE_MANAGER_HPP

#include "Util/GameObject.hpp"
#include "Background.hpp"

/**
 * @class PhaseManager
 * @brief 管理遊戲階段的類別。
 */
class PhaseManager {
public:
    PhaseManager();

    /**
     * @brief 取得場景中的子物件。
     * @return 共享指標陣列，包含背景物件。
     */
    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        return {m_Background};
    }

    /**
     * @brief 進入下一小關。
     * @return bool 如果所有小關都完成則返回true，否則返回false。
     */
    bool NextSubPhase();

    /**
     * @brief 進入下一大關。
     */
    void NextMainPhase();

    /**
     * @brief 獲取當前大關索引。
     * @return 當前大關索引 (0-5)。
     */
    [[nodiscard]] int GetCurrentMainPhase() const { return m_MainPhase; }

    /**
     * @brief 獲取當前小關索引。
     * @return 當前小關索引 (0-4)。
     */
    [[nodiscard]] int GetCurrentSubPhase() const { return m_SubPhase; }

    /**
     * @brief 獲取背景對象。
     * @return 背景對象的共享指針。
     */
    [[nodiscard]] std::shared_ptr<BackgroundImage> GetBackground() const { return m_Background; }

private:
    std::shared_ptr<BackgroundImage> m_Background; ///< 背景物件
    int m_MainPhase = 0; ///< 當前大關索引 (0-5)
    int m_SubPhase = 0;  ///< 當前小關索引 (0-4)
    static constexpr int MAX_SUB_PHASES = 6; // 最大小關數量常數
};

#endif // PHASE_MANAGER_HPP
