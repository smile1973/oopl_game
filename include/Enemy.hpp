#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"
#include "Util/Renderer.hpp"
#include "Util/Time.hpp"
#include "Util/Animation.hpp"

#include <set>

// Enemy 類別，繼承自 Character，代表遊戲中的敵人角色
class Enemy : public Character {
public:
    Enemy(std::string name, float health, const std::vector<std::string>& ImageSet);    // 構造函數，初始化敵人的血量與影像集

    [[nodiscard]] bool IfAlive() const{ return m_Health > 0.0f; }   // 檢查敵人是否仍然存活

    void TakeDamage(float damage);  // 讓敵人受到傷害，減少生命值

    void SetHealth(float Health = -1.0f);   // 更改敵人血量，默認原血量

    void MovePosition(const glm::vec2& Position, float totalTime = 0.0f);  //平移位置
    void MoveToPosition(const glm::vec2& targetPosition, float totalTime = 0.0f); //平移到某位置
    void ShakePosition(const glm::vec2& Position, float totalTime = 0.0f);

    [[nodiscard]] glm::vec2& GetTargetPosition() override { return m_TargetPosition; }

    [[nodiscard]] std::string& GetName() { return m_Name; }

    // 新增：切換敵人圖片集的函數
    void SwitchImageSet(const std::vector<std::string>& newImageSet);
    void SwitchImageSetByIndex(int imageSetIndex);
    void AddImageSetCollection(const std::vector<std::vector<std::string>>& imageSets);
    void SetImageSetCollection(int index, const std::vector<std::string>& imageSet);
    [[nodiscard]] int GetCurrentImageSetIndex() const { return m_CurrentImageSetIndex; }
    [[nodiscard]] size_t GetImageSetCollectionSize() const { return m_ImageSetCollection.size(); }

    virtual void SetProgressIcon(const std::string &ImagePath) { m_Drawable = std::make_shared<Util::Image>(ImagePath); }

    void Update() override;
    void Reset() override;

    static std::set<float> s_HealthBarYPositions;
    void DrawHealthBar(const glm::vec2& position = glm::vec2 (0.9f, 0.9)) const;    // 繪製敵人的血條

    void InitHealthRing();
    void UpdateHealthRing();
    void SetShowHealthRing(bool show) { m_ShowHealthRing = show; }
    bool GetShowHealthRing() const { return m_ShowHealthRing; }
private:

    static void InitProgram();  // 初始化著色程序（Shader Program）
    static void InitVertexArray();  // 初始化頂點陣列（Vertex Array Object）
    void InitUniforms();    // 初始化 Uniform 變數（著色器中的全域變數）

    // 重建動畫的私有函數
    void RebuildAnimation(const std::vector<std::string>& newImageSet);

    static std::unique_ptr<Core::Program> s_Program;    // 靜態成員變數：共享的著色程序
    static std::unique_ptr<Core::VertexArray> s_VertexArray;    // 靜態成員變數：共享的頂點數據

    std::string m_Name;
    float m_Health;
    float m_MaxHealth;

    // 圖片集管理
    std::vector<std::vector<std::string>> m_ImageSetCollection;
    int m_CurrentImageSetIndex = 0;

    bool m_IsMoving = false;
    float m_Speed = 0.0f;
    float m_MaxDistance = 0.0f;
    float m_DistanceTraveled = 0.0f;
    float m_TotalTime = 0.0f;
    glm::vec2 m_Direction = glm::vec2(0.0f, 0.0f);
    glm::vec2 m_TargetPosition = glm::vec2(0.0f, 0.0f);

    // Uniform 變數位置（顏色與血條寬度）
    GLint m_ColorLocation;
    GLint m_WidthLocation;

    bool m_ShowHealthRing = false;  // 是否顯示血條環
    std::shared_ptr<Util::GameObject> m_HealthRingBackground;  // 半透明背景
    std::vector<std::shared_ptr<Util::GameObject>> m_HealthDots;  // 血條點
    int m_TotalDots = 80;  // 環形血條上的點數量
    float m_RingRadius = 150.0f;  // 環形半徑
};

#endif // ENEMY_HPP
